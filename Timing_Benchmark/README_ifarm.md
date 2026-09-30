# Reproducing the timing benchmark on ifarm

This folder is self-contained: `templates/*.C` are parameterized ROOT/TMVA macros,
`run_benchmark.sh` fills them in and runs everything, and `../Timing_Results.md` documents the
numbers already measured on a local Mac (Apple M1). Here's how to redo it on JLab's ifarm.

## 1. Get the code and data onto ifarm

**Important:** training for this analysis was done on local compute, not on ifarm — the MC
training samples (`rootTraining/`) do **not** already exist there, unlike the real-Data files
(see below, which *are* already on ifarm and need no transfer at all).

From your Mac:

```bash
# The analysis folder itself (code + already-copied MC-validation ROOT files)
rsync -avz /Users/mariana/Work/Lepton_ID_Analysis/ \
    <user>@ifarm.jlab.org:/path/to/Lepton_ID_Analysis/

# The production weight files for the inference leg (small, ~73 MB total)
rsync -avz /Users/mariana/Work/6-BDT-MLP/ <user>@ifarm.jlab.org:/path/to/6-BDT-MLP/
rsync -avz /Users/mariana/Documents/LeptonID/9-BDT-MLP/ <user>@ifarm.jlab.org:/path/to/9-BDT-MLP/

# Only needed if you also want to reproduce TRAINING time on ifarm (~438 MB, not required
# for inference-only benchmarking — see step 3b below, which needs none of this):
rsync -avz /Users/mariana/Documents/LeptonID/rootTraining/ \
    <user>@ifarm.jlab.org:/path/to/rootTraining/
```

Prefer `/work/clas12/<user>/...` or `/volatile/clas12/<user>/...` on ifarm over `$HOME` — home
directories have small quotas and these files are not small.

**Real Data needs no transfer at all** — `toroot_v2.C` already writes the production real-Data
files to `/work/clas12/mtenorio/Analysis/Latest_Final/{F18in,F18out,S18in,S18out,S19}_All.root`
on ifarm. Each file's `analysis` tree holds both `positron_*` branches (feeds the `*_positives`
models) and `electron_*` branches (feeds the `*_negatives` models) for that run period — so
`F18in_All.root` alone covers both `F18in_positives` and `F18in_negatives`.

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

## 3a. Inference-only run (recommended — needs no MC training data at all)

Since the MC training samples aren't on ifarm and inference is the fast, cheap part anyway, the
most practical ifarm run is inference-only: production weight files (uploaded in step 1) plus
the real-Data files already sitting at `/work/clas12/mtenorio/Analysis/Latest_Final/`. Skip
`ROOTTRAINING_DIR` and `DATA_VALIDATION_FILE`/training entirely — the script only runs the legs
whose inputs are set:

```bash
export WEIGHTS6_DIR=/path/to/6-BDT-MLP
export WEIGHTS9_DIR=/path/to/9-BDT-MLP
export IFARM_DATA_DIR=/work/clas12/mtenorio/Analysis/Latest_Final
export DATASETS="F18in_positives F18in_negatives F18out_positives F18out_negatives"
export OUT_DIR=$PWD/benchmark_run

bash /path/to/Lepton_ID_Analysis/Timing_Benchmark/run_benchmark.sh
```

This generates and runs `infer_ifarm_<dataset>_{6,9}var.C` for all 4 datasets — 16 runs total,
each reading the `analysis` tree's `positron_*` or `electron_*` branches (matching which half of
`<Period>_All.root` corresponds to `*_positives`/`*_negatives`) through the same `TMVA::Reader` +
`TStopwatch` timing as the MC/small-Data legs. Since it's pure inference on files that can be
large (real production statistics, not a small 2,165-event validation sample), this should
finish in well under an hour even for all 16 combinations — no SLURM job strictly required, but
still better run off the login node if possible (see below).

**Caveat:** `toroot_v2.C` computes `theta`/`phi` in **degrees**; if the original MC training used
radians, the resulting BDT/MLP *scores* from this leg won't be physically meaningful — but the
timing numbers are unaffected either way (`TMVA::Reader::EvaluateMVA` does the same fixed amount
of work regardless of the input values).

## 3b. Full run including training, if you also upload the MC samples

Same idea, but also set `ROOTTRAINING_DIR` (from the optional rsync in step 1) and optionally
`DATA_VALIDATION_FILE` for the small Data-validation leg. All four legs (training, MC inference,
small-Data inference, ifarm real-Data inference) run in one pass if all their inputs are set.

## 3c. Run on a dedicated allocation, not the login node

ifarm login nodes are shared across many users; running the benchmark there makes the timing
numbers noisy (or misleading — you'd be measuring contention, not the model). Submit it as a
SLURM job with a fixed core count instead:

```bash
cat > submit_benchmark.slurm << 'EOF'
#!/bin/bash
#SBATCH --job-name=tmva_timing
#SBATCH --partition=production
#SBATCH --account=clas12
#SBATCH --cpus-per-task=1
#SBATCH --mem=8G
#SBATCH --time=04:00:00
#SBATCH --output=tmva_timing_%j.log

module load root/<version>   # or: conda activate my_root_env

export WEIGHTS6_DIR=/path/to/6-BDT-MLP
export WEIGHTS9_DIR=/path/to/9-BDT-MLP
export IFARM_DATA_DIR=/work/clas12/mtenorio/Analysis/Latest_Final
# Only set these two if you uploaded the MC training samples (step 3b):
# export ROOTTRAINING_DIR=/path/to/rootTraining
# export DATA_VALIDATION_FILE=/path/to/Lepton_ID_Analysis/Files/Result_v2/pos_F18in_BDT_modFC.root
export OUT_DIR=$PWD/benchmark_run

bash /path/to/Lepton_ID_Analysis/Timing_Benchmark/run_benchmark.sh
EOF

sbatch submit_benchmark.slurm
```

Adjust `--partition`/`--account` to whatever your ifarm allocation actually uses (`sacctmgr show
associations user=$USER` will show what you have access to if unsure). `--cpus-per-task=1` matters
for comparability with the Mac numbers, which were also single-threaded (ROOT's
`EnableImplicitMT()` was never called).

As written above (inference-only, `ROOTTRAINING_DIR` unset), expect this to finish in well under
an hour. If you uncomment the training leg (3b), expect roughly **1.5–2 hours** for the full
4-dataset x 2-variable-set training sweep instead (the MLP-9var runs were the slowest locally,
~14 min each) — the `--time=04:00:00` above gives margin either way.

## 4. Read the results

`run_benchmark.sh` prints a results summary to stdout (captured in the SLURM `.log` file) and
leaves full per-run logs in `$OUT_DIR/logs/`. To pull just the numbers you'd want to compare
against `../Timing_Results.md`:

```bash
grep "Elapsed time for training" $OUT_DIR/logs/train_*.log
grep -E "Total Events|Total Time" $OUT_DIR/logs/infer_*.log $OUT_DIR/logs/infer_data_*.log $OUT_DIR/logs/infer_ifarm_*.log
```

## Notes on what this does and doesn't touch

- Training writes into `$OUT_DIR/train/` only — it does **not** overwrite the production weight
  files in `6-BDT-MLP/`/`9-BDT-MLP/` (TMVA's train/test split is randomized per run, so a rerun
  produces different weights, not a reproduction of the paper's actual trained model).
- Inference reads the production weight files and the `<Period>_All.root` real-Data files
  read-only, and writes small scratch output into `$OUT_DIR/infer/`, `$OUT_DIR/infer_data/`, and
  `$OUT_DIR/infer_ifarm_data/`.
- `ROOTTRAINING_DIR`/`DATA_VALIDATION_FILE` (MC-based legs) and `IFARM_DATA_DIR` (real-Data leg)
  are all independently optional — the script only runs the legs whose required variable(s) are
  set. On ifarm specifically, `IFARM_DATA_DIR=/work/clas12/mtenorio/Analysis/Latest_Final` needs
  no upload at all (see 3a), while `ROOTTRAINING_DIR` does (see 3b) since training happened on
  local compute, not ifarm.
- `DATA_VALIDATION_FILE` covers only `F18in_positives` (the one small validation sample that
  exists); `IFARM_DATA_DIR` covers all 4 datasets, since each `<Period>_All.root` has both the
  `positron_*` and `electron_*` branches needed for its `_positives`/`_negatives` pair.
