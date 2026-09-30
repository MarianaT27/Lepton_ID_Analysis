# Reproducing the timing benchmark on ifarm

This folder is self-contained: `templates/*.C` are parameterized ROOT/TMVA macros,
`run_benchmark.sh` fills them in and runs everything, and `../Timing_Results.md` documents the
numbers already measured on a local Mac (Apple M1). Here's how to redo it on JLab's ifarm.

## 1. Get the code and data onto ifarm

From your Mac:

```bash
# The analysis folder itself (code + already-copied validation ROOT files)
rsync -avz /Users/mariana/Work/Lepton_ID_Analysis/ \
    <user>@ifarm.jlab.org:/path/to/Lepton_ID_Analysis/

# The training samples (needed for the training leg and the MC inference leg).
# These are large (~34 MB x2 x4 datasets x2 variable-sets, ~500 MB total) — check first
# whether they already exist on ifarm/lustre under something like
# /lustre24/expphy/volatile/clas12/<you>/... (the code that produces them,
# toroot_v2MC.C, already writes there), since that would be much faster than
# re-uploading from your Mac.
rsync -avz /Users/mariana/Documents/LeptonID/rootTraining/ \
    <user>@ifarm.jlab.org:/path/to/rootTraining/

# The production weight files for the inference leg
rsync -avz /Users/mariana/Work/6-BDT-MLP/ <user>@ifarm.jlab.org:/path/to/6-BDT-MLP/
rsync -avz /Users/mariana/Documents/LeptonID/9-BDT-MLP/ <user>@ifarm.jlab.org:/path/to/9-BDT-MLP/
```

Prefer `/work/clas12/<user>/...` or `/volatile/clas12/<user>/...` on ifarm over `$HOME` — home
directories have small quotas and these files are not small.

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

## 3. Run on a dedicated allocation, not the login node

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

export ROOTTRAINING_DIR=/path/to/rootTraining
export WEIGHTS6_DIR=/path/to/6-BDT-MLP
export WEIGHTS9_DIR=/path/to/9-BDT-MLP
export DATA_VALIDATION_FILE=/path/to/Lepton_ID_Analysis/Files/Result_v2/pos_F18in_BDT_modFC.root
export OUT_DIR=$PWD/benchmark_run

bash /path/to/Lepton_ID_Analysis/Timing_Benchmark/run_benchmark.sh
EOF

sbatch submit_benchmark.slurm
```

Adjust `--partition`/`--account` to whatever your ifarm allocation actually uses (`sacctmgr show
associations user=$USER` will show what you have access to if unsure). `--cpus-per-task=1` matters
for comparability with the Mac numbers, which were also single-threaded (ROOT's
`EnableImplicitMT()` was never called).

Expect roughly **1.5–2 hours** for the full 4-dataset x 2-variable-set training sweep (the
MLP-9var runs were the slowest locally, ~14 min each) — the `--time=04:00:00` above gives margin.
Inference (MC + Data) takes a few minutes total and could be split into its own short job if you
want training and inference results sooner.

## 4. Read the results

`run_benchmark.sh` prints a results summary to stdout (captured in the SLURM `.log` file) and
leaves full per-run logs in `$OUT_DIR/logs/`. To pull just the numbers you'd want to compare
against `../Timing_Results.md`:

```bash
grep "Elapsed time for training" $OUT_DIR/logs/train_*.log
grep -E "Total Events|Total Time" $OUT_DIR/logs/infer_*.log $OUT_DIR/logs/infer_data_*.log
```

## Notes on what this does and doesn't touch

- Training writes into `$OUT_DIR/train/` only — it does **not** overwrite the production weight
  files in `6-BDT-MLP/`/`9-BDT-MLP/` (TMVA's train/test split is randomized per run, so a rerun
  produces different weights, not a reproduction of the paper's actual trained model).
- Inference reads the production weight files read-only and writes small scratch output into
  `$OUT_DIR/infer/` and `$OUT_DIR/infer_data/`.
- `DATA_VALIDATION_FILE` is optional — only `F18in_positives` currently has a real-Data validation
  sample; omit the variable (or leave it unset) to skip the Data leg and only run MC training +
  inference for all 4 datasets.
