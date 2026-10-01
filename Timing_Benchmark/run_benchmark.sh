#!/bin/bash
# TMVA training/inference timing benchmark — reproducible driver.
#
# Assumes `root` (ROOT with TMVA) is already on PATH — load it yourself first
# (module load / conda activate / thisroot.sh), this script doesn't touch your
# environment. See ../README_ifarm.md for ifarm-specific setup.
#
# All paths below are configurable via environment variables so this runs
# unchanged on any machine that has the same directory layout as the original
# analysis (rootTraining/{6,9}-Variables, 6-BDT-MLP/, 9-BDT-MLP/).

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEMPLATES="$SCRIPT_DIR/templates"

# WEIGHTS_DIR defaults to this repo's own committed weights (Weights/ML_weights_pass2/, a
# sibling of Timing_Benchmark/) — a repo-relative default, not a machine-specific one, so
# cloning the repo just works with no config. Override only if pointing at a different export.
# Everything else has NO default on purpose: it must be set explicitly by the caller, so this
# script never silently points at a path that only exists on the machine it was written on.
: "${WEIGHTS_DIR:=$SCRIPT_DIR/../Weights/ML_weights_pass2}"   # pattern: $WEIGHTS_DIR/<ShortName>/TMVAClassification_{BDT,MLP}_{6,9}.weights.xml
: "${ROOTTRAINING_DIR:=}"        # set to run training + MC inference (local compute only — this data has never existed on ifarm). pattern: $ROOTTRAINING_DIR/{6,9}-Variables/<name>_{Lepton,Pion}.root
: "${DATA_VALIDATION_FILE:=}"    # optional: small real-Data file for the Data inference leg, e.g. Files/Result_v2/pos_F18in_BDT_modFC.root (F18in_positives only; tree "results", branches *_D) — exists on the Mac only, not ifarm
: "${IFARM_DATA_DIR:=}"          # optional: dir with <Period>_All.root real-Data files from toroot_v2.C (tree "analysis", branches positron_*/electron_*) — this is the ifarm leg, e.g. /work/clas12/mtenorio/Analysis/Latest_Final
: "${DATASETS:=F18in_positives F18in_negatives F18out_positives F18out_negatives}"
: "${OUT_DIR:=$(pwd)/benchmark_run}"

command -v root >/dev/null 2>&1 || { echo "ERROR: 'root' not found on PATH. Load ROOT first (see README_ifarm.md)."; exit 1; }

if [ -z "$ROOTTRAINING_DIR" ] && [ -z "$DATA_VALIDATION_FILE" ] && [ -z "$IFARM_DATA_DIR" ]; then
  echo "ERROR: nothing to do — set at least one of ROOTTRAINING_DIR (training + MC inference)," >&2
  echo "       DATA_VALIDATION_FILE (small real-Data inference), or IFARM_DATA_DIR (ifarm real-Data" >&2
  echo "       inference). See README_ifarm.md." >&2
  exit 1
fi
if [ ! -d "$WEIGHTS_DIR" ]; then
  echo "ERROR: WEIGHTS_DIR ('$WEIGHTS_DIR') doesn't exist. Every inference leg needs it — either" >&2
  echo "       run this from inside a clone of the repo (so the default resolves), or set" >&2
  echo "       WEIGHTS_DIR explicitly." >&2
  exit 1
fi

# Maps a benchmark dataset name to ML_weights_pass2's short folder name.
pass2_short_name() {
  local name=$1
  name="${name/_positives/pos}"
  name="${name/_negatives/neg}"
  echo "$name"
}

# Full path to one weight file: weight_file <name> <nv> <BDT|MLP>
weight_file() {
  local name=$1 nv=$2 method=$3
  echo "$WEIGHTS_DIR/$(pass2_short_name "$name")/TMVAClassification_${method}_${nv}.weights.xml"
}

mkdir -p "$OUT_DIR"/{train,infer,infer_data,infer_ifarm_data,logs}
cat > "$OUT_DIR/reader_block_6.txt" << 'EOF'
EOF
cat > "$OUT_DIR/reader_block_9.txt" << 'EOF'
    reader->AddVariable( "P", &P );
    reader->AddVariable( "Theta", &Theta );
    reader->AddVariable( "Phi", &Phi );
EOF

echo "=== Environment ==="
root-config --version 2>/dev/null || true
uname -a
echo "OUT_DIR=$OUT_DIR"
echo

# ---------- generate training macros ----------
gen_train() {
  local name=$1 nv=$2
  local fn="train_${name}_${nv}var" vardir="${nv}-Variables"
  local blockfile
  if [ "$nv" = "6" ]; then
    blockfile=$(mktemp)
    cat > "$blockfile" << 'EOF'
   dataloader->AddVariable( "SFPCAL","PCAL","", 'F' );
   dataloader->AddVariable( "SFECIN","ECIN", "", 'F' );
   dataloader->AddVariable( "SFECOUT","ECOUT", "", 'F' );
   dataloader->AddVariable( "m2PCAL","m2PCAL","", 'F' );
   dataloader->AddVariable( "m2ECIN","m2ECIN", "", 'F' );
   dataloader->AddVariable( "m2ECOUT","m2ECOUT", "", 'F' );
EOF
  else
    blockfile=$(mktemp)
    cat > "$blockfile" << 'EOF'
   dataloader->AddVariable( "P","P","", 'F' );
   dataloader->AddVariable( "Theta","Theta","", 'F' );
   dataloader->AddVariable( "Phi","Phi","", 'F' );
   dataloader->AddVariable( "SFPCAL","PCAL","", 'F' );
   dataloader->AddVariable( "SFECIN","ECIN", "", 'F' );
   dataloader->AddVariable( "SFECOUT","ECOUT", "", 'F' );
   dataloader->AddVariable( "m2PCAL","m2PCAL","", 'F' );
   dataloader->AddVariable( "m2ECIN","m2ECIN", "", 'F' );
   dataloader->AddVariable( "m2ECOUT","m2ECOUT", "", 'F' );
EOF
  fi
  sed \
    -e "s/TRAINFUNC/${fn}/g" \
    -e "s/__NAME__/${name}/g" \
    -e "s/__NVARS__/${nv}/g" \
    -e "s#__VARDIR__#${ROOTTRAINING_DIR}/${vardir}#g" \
    -e "s/%NVARS%/${nv}/g" \
    "$TEMPLATES/train_template.C" \
    > "$OUT_DIR/train/${fn}.C.tmp"
  sed -e "/__VARBLOCK__/r ${blockfile}" -e "/__VARBLOCK__/d" "$OUT_DIR/train/${fn}.C.tmp" > "$OUT_DIR/train/${fn}.C"
  rm -f "$OUT_DIR/train/${fn}.C.tmp" "$blockfile"
}

# ---------- generate inference (MC) macros ----------
gen_infer() {
  local name=$1 nv=$2
  local fn="infer_${name}_${nv}var" traindir blockfile
  if [ "$nv" = "6" ]; then traindir="6-Variables"; blockfile="$OUT_DIR/reader_block_6.txt"
  else traindir="9-Variables"; blockfile="$OUT_DIR/reader_block_9.txt"
  fi
  sed \
    -e "s/INFERFUNC/${fn}/g" \
    -e "s/__NAME__/${name}/g" \
    -e "s/__NVARS__/${nv}/g" \
    -e "s#__TRAINDIR__#${ROOTTRAINING_DIR}/${traindir}#g" \
    -e "s#__MLP_WEIGHTS__#$(weight_file "$name" "$nv" MLP)#g" \
    -e "s#__BDT_WEIGHTS__#$(weight_file "$name" "$nv" BDT)#g" \
    -e "s/%NVARS%/${nv}/g" \
    "$TEMPLATES/infer_template.C" \
    > "$OUT_DIR/infer/${fn}.C.tmp"
  sed -e "/__READERVARBLOCK__/r ${blockfile}" -e "/__READERVARBLOCK__/d" "$OUT_DIR/infer/${fn}.C.tmp" > "$OUT_DIR/infer/${fn}.C"
  rm -f "$OUT_DIR/infer/${fn}.C.tmp"
}

# ---------- generate inference (Data) macro, F18in_positives only ----------
gen_infer_data() {
  local nv=$1
  local fn="infer_data_F18in_positives_${nv}var" blockfile
  if [ "$nv" = "6" ]; then blockfile="$OUT_DIR/reader_block_6.txt"; else blockfile="$OUT_DIR/reader_block_9.txt"; fi
  sed \
    -e "s/INFERFUNC/${fn}/g" \
    -e "s/__NAME__/F18in_positives/g" \
    -e "s/__NVARS__/${nv}/g" \
    -e "s#__MLP_WEIGHTS__#$(weight_file F18in_positives "$nv" MLP)#g" \
    -e "s#__BDT_WEIGHTS__#$(weight_file F18in_positives "$nv" BDT)#g" \
    -e "s#__DATAFILE__#${DATA_VALIDATION_FILE}#g" \
    "$TEMPLATES/infer_data_template.C" > "$OUT_DIR/infer_data/${fn}.C.tmp"
  sed -e "/__READERVARBLOCK__/r ${blockfile}" -e "/__READERVARBLOCK__/d" "$OUT_DIR/infer_data/${fn}.C.tmp" > "$OUT_DIR/infer_data/${fn}.C"
  rm -f "$OUT_DIR/infer_data/${fn}.C.tmp"
}

# ---------- generate inference (ifarm real Data) macros ----------
# Maps a dataset name to its (file, branch-prefix) pair, per toroot_v2.C's output:
# one <Period>_All.root file holds both positron_* (positives model) and electron_*
# (negatives model) branches in the same "analysis" tree.
gen_infer_ifarm_data() {
  local name=$1 nv=$2
  local fn="infer_ifarm_${name}_${nv}var" period species blockfile
  period="${name%_*}"                 # F18in_positives -> F18in
  case "$name" in
    *_positives) species="positron" ;;
    *_negatives) species="electron" ;;
    *) echo "ERROR: don't know species for dataset '$name' (expected *_positives or *_negatives)"; return 1 ;;
  esac
  if [ "$nv" = "6" ]; then blockfile="$OUT_DIR/reader_block_6.txt"; else blockfile="$OUT_DIR/reader_block_9.txt"; fi
  sed \
    -e "s/INFERFUNC/${fn}/g" \
    -e "s/__NAME__/${name}/g" \
    -e "s/__NVARS__/${nv}/g" \
    -e "s/__SPECIES__/${species}/g" \
    -e "s#__MLP_WEIGHTS__#$(weight_file "$name" "$nv" MLP)#g" \
    -e "s#__BDT_WEIGHTS__#$(weight_file "$name" "$nv" BDT)#g" \
    -e "s#__DATAFILE__#${IFARM_DATA_DIR}/${period}_All.root#g" \
    "$TEMPLATES/infer_ifarm_data_template.C" > "$OUT_DIR/infer_ifarm_data/${fn}.C.tmp"
  sed -e "/__READERVARBLOCK__/r ${blockfile}" -e "/__READERVARBLOCK__/d" "$OUT_DIR/infer_ifarm_data/${fn}.C.tmp" > "$OUT_DIR/infer_ifarm_data/${fn}.C"
  rm -f "$OUT_DIR/infer_ifarm_data/${fn}.C.tmp"
}

echo "=== Generating macros ==="
if [ -n "$ROOTTRAINING_DIR" ]; then
  for name in $DATASETS; do
    for nv in 6 9; do
      gen_train "$name" "$nv"
      gen_infer "$name" "$nv"
    done
  done
fi
if [ -n "$IFARM_DATA_DIR" ]; then
  for name in $DATASETS; do
    for nv in 6 9; do
      gen_infer_ifarm_data "$name" "$nv"
    done
  done
fi
if [ -n "$DATA_VALIDATION_FILE" ]; then
  gen_infer_data 6
  gen_infer_data 9
fi
echo "done."
echo

if [ -n "$ROOTTRAINING_DIR" ]; then
  echo "=== Running training (this is the slow part — MLP-9var takes ~15 min/dataset) ==="
  for name in $DATASETS; do
    for nv in 6 9; do
      fn="train_${name}_${nv}var"
      echo "--- $fn : $(date) ---"
      ( cd "$OUT_DIR/train" && { time root -l -b -q "${fn}.C" ; } > "$OUT_DIR/logs/${fn}.log" 2>&1 )
    done
  done

  echo "=== Running inference (MC) ==="
  for name in $DATASETS; do
    for nv in 6 9; do
      fn="infer_${name}_${nv}var"
      ( cd "$OUT_DIR/infer" && root -l -b -q "${fn}.C" ) > "$OUT_DIR/logs/${fn}.log" 2>&1
    done
  done
fi

if [ -n "$DATA_VALIDATION_FILE" ]; then
  echo "=== Running inference (Data, F18in_positives) ==="
  for nv in 6 9; do
    fn="infer_data_F18in_positives_${nv}var"
    ( cd "$OUT_DIR/infer_data" && root -l -b -q "${fn}.C" ) > "$OUT_DIR/logs/${fn}.log" 2>&1
  done
fi

if [ -n "$IFARM_DATA_DIR" ]; then
  echo "=== Running inference (ifarm real Data, all datasets) ==="
  for name in $DATASETS; do
    for nv in 6 9; do
      fn="infer_ifarm_${name}_${nv}var"
      ( cd "$OUT_DIR/infer_ifarm_data" && root -l -b -q "${fn}.C" ) > "$OUT_DIR/logs/${fn}.log" 2>&1
    done
  done
fi

echo
echo "=== RESULTS ==="

report() {
  local fn=$1 pattern=$2
  echo "$fn:"
  grep -E "$pattern" "$OUT_DIR/logs/${fn}.log" 2>/dev/null | sed 's/^/    /' \
    || echo "    (no matching output — check $OUT_DIR/logs/${fn}.log, this run may have failed)"
}

if [ -n "$ROOTTRAINING_DIR" ]; then
  echo "--- Training: elapsed time per method (order booked: MLP then BDT) ---"
  for name in $DATASETS; do
    for nv in 6 9; do
      report "train_${name}_${nv}var" "Elapsed time for training"
    done
  done
  echo
  echo "--- Inference (MC): speed report ---"
  for name in $DATASETS; do
    for nv in 6 9; do
      report "infer_${name}_${nv}var" "Total Events|Total Time"
    done
  done
fi
if [ -n "$DATA_VALIDATION_FILE" ]; then
  echo
  echo "--- Inference (Data): speed report ---"
  for nv in 6 9; do
    report "infer_data_F18in_positives_${nv}var" "Total Events|Total Time"
  done
fi
if [ -n "$IFARM_DATA_DIR" ]; then
  echo
  echo "--- Inference (ifarm real Data): speed report ---"
  for name in $DATASETS; do
    for nv in 6 9; do
      report "infer_ifarm_${name}_${nv}var" "Total Events|Total Time"
    done
  done
fi
echo
echo "Full logs are in: $OUT_DIR/logs/"
