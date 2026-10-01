# Reproducing the inference-timing benchmark on ifarm

This folder is self-contained: `templates/*.C` are parameterized ROOT/TMVA macros,
`run_benchmark.sh` fills them in and runs everything, and `../Timing_Results.md` documents the
numbers already measured on a local Mac (Apple M1). Here's how to get **inference** timing on
JLab's ifarm as a comparison point.

**Training time is Mac-only and stays that way.** A weight file has no record of how long
training took — the only way to measure training time is to actually run
`TMVA::Factory::TrainAllMethods()` on the real MC samples (`rootTraining/`), and that data has
never existed on ifarm and isn't going there. So this page covers inference only.

## 1. Get the code onto ifarm

```bash
git clone https://github.com/MarianaT27/Lepton_ID_Analysis.git
```

That's it — **no separate weight-file transfer needed**. `Weights/ML_weights_pass2/` (BDT + MLP,
6-var and 9-var, all datasets) is committed in the repo, so it comes down with the clone, and
`run_benchmark.sh` finds it automatically (see step 3).

**Real Data needs no transfer either** — `toroot_v2.C` already writes the production real-Data
files to `/work/clas12/mtenorio/Analysis/Latest_Final/{F18in,F18out,S18in,S18out,S19}_All.root`
on ifarm. Each file's `analysis` tree holds both `positron_*` branches (feeds the `*_positives`
models) and `electron_*` branches (feeds the `*_negatives` models) for that run period — so
`F18in_All.root` alone covers both `F18in_positives` and `F18in_negatives`. (Note: `S18in`/`S18out`
have Data files but no trained weights in `ML_weights_pass2` — they can't be used for this
benchmark.)

## 2. Get ROOT (with TMVA) on PATH

ifarm uses environment modules. From an ifarm login node:

```bash
module avail root          # or: module spider root
module load root/<version> # pick one built with TMVA (most CLAS12 builds are)
root-config --has-tmva     # sanity check — should print "yes"
```

If no suitable module exists, JLab also provides `conda`/`mamba` on ifarm — you can mirror what
was used locally:

```bash
module load anaconda3     # or however conda is exposed on ifarm
conda create -n my_root_env -c conda-forge root
conda activate my_root_env
```

`run_benchmark.sh` doesn't load ROOT itself — activate whichever of the above works *before*
invoking it, in the same shell.

## 3. Run it

Only `IFARM_DATA_DIR` needs to be set — `WEIGHTS_DIR` already defaults to the repo's own
`Weights/ML_weights_pass2/`, and `DATASETS` already defaults to the 4 datasets that match the
Mac numbers (so the two are directly comparable in one table):

```bash
export IFARM_DATA_DIR=/work/clas12/mtenorio/Analysis/Latest_Final
export OUT_DIR=$PWD/benchmark_run

bash Lepton_ID_Analysis/Timing_Benchmark/run_benchmark.sh
```

This generates and runs `infer_ifarm_<dataset>_{6,9}var.C` for all 4 datasets — 16 runs total,
each reading the `analysis` tree's `positron_*` or `electron_*` branches through the same
`TMVA::Reader` + `TStopwatch` timing used for the Mac numbers.

**Test on a small subset first.** The real `<Period>_All.root` files can be far larger than
anything benchmarked locally, so there's no good way to predict runtime in advance. Set
`MAX_EVENTS` to cap each file to its first N events for a fast sanity check before committing to
the full run:

```bash
export MAX_EVENTS=10000   # ~seconds, not hours
bash Lepton_ID_Analysis/Timing_Benchmark/run_benchmark.sh
```

This confirms the weights load and the real schema reads correctly, and gives you a real
per-event rate to extrapolate from (unset `MAX_EVENTS`, or set it to `0`, to run the full file).
Each run also now prints progress every ~5% of its events to its log file — tail a log to tell
"slow but working" apart from "actually hung":

```bash
tail -f benchmark_run/logs/infer_ifarm_F18in_positives_6var.log
```

**Caveat:** `toroot_v2.C` computes `theta`/`phi` in **degrees**; if training used radians, the
BDT/MLP *scores* from this leg won't be physically meaningful — but the timing numbers are
unaffected either way (`TMVA::Reader::EvaluateMVA` does the same fixed amount of work regardless
of the input values).

## 3a. Keep it running after you log out (no SLURM)

If you're running this interactively on a login node rather than submitting it as a job, closing
your terminal sends SIGHUP to everything running in it — including the benchmark. `tmux` (or
`screen`) detaches the session from your terminal entirely, so logging out doesn't touch it:

```bash
tmux new -s timing          # starts a new persistent session
# ... load ROOT, export IFARM_DATA_DIR, run the script as above ...
```

Detach any time with `Ctrl-b` then `d` (the job keeps running) and close your terminal freely.
Reattach later from any login to check progress:

```bash
tmux attach -t timing
```

If `tmux` isn't available, `nohup ... &` works too, just without the ability to reattach and
watch it live — only `tail -f` the log:

```bash
nohup bash Lepton_ID_Analysis/Timing_Benchmark/run_benchmark.sh > run.log 2>&1 &
disown
```

## 3b. Run on a dedicated allocation instead, if you change your mind about SLURM

ifarm login nodes are shared across many users; running the benchmark there makes the timing
numbers noisier (you're partly measuring contention, not the model) — usually not enough to
matter for a sanity-check comparison point, but worth knowing. If you'd rather have a clean,
dedicated run, submit it as a SLURM job with a fixed core count instead:

```bash
cat > submit_benchmark.slurm << 'EOF'
#!/bin/bash
#SBATCH --job-name=tmva_timing
#SBATCH --partition=production
#SBATCH --account=clas12
#SBATCH --cpus-per-task=1
#SBATCH --mem=4G
#SBATCH --time=01:00:00
#SBATCH --output=tmva_timing_%j.log

module load root/<version>   # or: conda activate my_root_env

export IFARM_DATA_DIR=/work/clas12/mtenorio/Analysis/Latest_Final
export OUT_DIR=$PWD/benchmark_run

bash Lepton_ID_Analysis/Timing_Benchmark/run_benchmark.sh
EOF

sbatch submit_benchmark.slurm
```

Adjust `--partition`/`--account` to whatever your ifarm allocation actually uses (`sacctmgr show
associations user=$USER` will show what you have access to if unsure). `--cpus-per-task=1` matters
for comparability with the Mac numbers, which were also single-threaded (ROOT's
`EnableImplicitMT()` was never called).

## 4. Read the results

`run_benchmark.sh` prints a results summary to stdout (captured in the SLURM `.log` file) and
leaves full per-run logs in `$OUT_DIR/logs/`. To pull just the numbers to compare against the Mac
inference numbers already in `../Timing_Results.md`:

```bash
grep -E "Total Events|Total Time" $OUT_DIR/logs/infer_ifarm_*.log
```

## Notes on what this does and doesn't touch

- `WEIGHTS_DIR` defaults to `Timing_Benchmark/../Weights/ML_weights_pass2` — a repo-relative
  default, so it resolves correctly as long as you run the script from inside the clone. Override
  it only if you want to benchmark a different weight export.
- Inference reads the weight files and the `<Period>_All.root` real-Data files read-only, and
  writes small scratch output into `$OUT_DIR/infer_ifarm_data/` — nothing outside `$OUT_DIR` is
  touched.
- The other two legs this script can run (`ROOTTRAINING_DIR` for training + MC inference,
  `DATA_VALIDATION_FILE` for the small Mac-only Data sample) are **not applicable on ifarm** —
  neither input exists there. They're how the Mac numbers in `Timing_Results.md` were produced;
  leave both unset here.
