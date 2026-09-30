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

: "${ROOTTRAINING_DIR:=/Users/mariana/Documents/LeptonID/rootTraining}"
: "${WEIGHTS6_DIR:=/Users/mariana/Work/6-BDT-MLP}"          # pattern: $WEIGHTS6_DIR/dataset_<name>_mod/weights/
: "${WEIGHTS9_DIR:=/Users/mariana/Documents/LeptonID/9-BDT-MLP}"  # pattern: $WEIGHTS9_DIR/dataset_<name>/weights/
: "${DATA_VALIDATION_FILE:=}"                                 # optional: real-Data file for the Data inference leg (F18in_positives schema: tree "results", branches *_D)
: "${DATASETS:=F18in_positives F18in_negatives F18out_positives F18out_negatives}"
: "${OUT_DIR:=$(pwd)/benchmark_run}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEMPLATES="$SCRIPT_DIR/templates"

command -v root >/dev/null 2>&1 || { echo "ERROR: 'root' not found on PATH. Load ROOT first (see README_ifarm.md)."; exit 1; }

mkdir -p "$OUT_DIR"/{train,infer,infer_data,logs}
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
  local fn="infer_${name}_${nv}var" traindir weightdir blockfile
  if [ "$nv" = "6" ]; then
    traindir="6-Variables"; weightdir="${WEIGHTS6_DIR}/dataset_${name}_mod/weights/"
    blockfile="$OUT_DIR/reader_block_6.txt"
  else
    traindir="9-Variables"; weightdir="${WEIGHTS9_DIR}/dataset_${name}/weights/"
    blockfile="$OUT_DIR/reader_block_9.txt"
  fi
  sed \
    -e "s/INFERFUNC/${fn}/g" \
    -e "s/__NAME__/${name}/g" \
    -e "s/__NVARS__/${nv}/g" \
    -e "s#__TRAINDIR__#${ROOTTRAINING_DIR}/${traindir}#g" \
    -e "s#__WEIGHTDIR__#${weightdir}#g" \
    -e "s/%NVARS%/${nv}/g" \
    "$TEMPLATES/infer_template.C" \
    > "$OUT_DIR/infer/${fn}.C.tmp"
  sed -e "/__READERVARBLOCK__/r ${blockfile}" -e "/__READERVARBLOCK__/d" "$OUT_DIR/infer/${fn}.C.tmp" > "$OUT_DIR/infer/${fn}.C"
  rm -f "$OUT_DIR/infer/${fn}.C.tmp"
}

# ---------- generate inference (Data) macro, F18in_positives only ----------
gen_infer_data() {
  local nv=$1
  local fn="infer_data_F18in_positives_${nv}var" weightdir blockfile
  if [ "$nv" = "6" ]; then
    weightdir="${WEIGHTS6_DIR}/dataset_F18in_positives_mod/weights/"; blockfile="$OUT_DIR/reader_block_6.txt"
  else
    weightdir="${WEIGHTS9_DIR}/dataset_F18in_positives/weights/"; blockfile="$OUT_DIR/reader_block_9.txt"
  fi
  sed \
    -e "s/INFERFUNC/${fn}/g" \
    -e "s/__NAME__/F18in_positives/g" \
    -e "s/__NVARS__/${nv}/g" \
    -e "s#__WEIGHTDIR__#${weightdir}#g" \
    -e "s#__DATAFILE__#${DATA_VALIDATION_FILE}#g" \
    "$TEMPLATES/infer_data_template.C" > "$OUT_DIR/infer_data/${fn}.C.tmp"
  sed -e "/__READERVARBLOCK__/r ${blockfile}" -e "/__READERVARBLOCK__/d" "$OUT_DIR/infer_data/${fn}.C.tmp" > "$OUT_DIR/infer_data/${fn}.C"
  rm -f "$OUT_DIR/infer_data/${fn}.C.tmp"
}

echo "=== Generating macros ==="
for name in $DATASETS; do
  for nv in 6 9; do
    gen_train "$name" "$nv"
    gen_infer "$name" "$nv"
  done
done
if [ -n "$DATA_VALIDATION_FILE" ]; then
  gen_infer_data 6
  gen_infer_data 9
fi
echo "done."
echo

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

if [ -n "$DATA_VALIDATION_FILE" ]; then
  echo "=== Running inference (Data, F18in_positives) ==="
  for nv in 6 9; do
    fn="infer_data_F18in_positives_${nv}var"
    ( cd "$OUT_DIR/infer_data" && root -l -b -q "${fn}.C" ) > "$OUT_DIR/logs/${fn}.log" 2>&1
  done
fi

echo
echo "=== RESULTS ==="
echo "--- Training: elapsed time per method (order booked: MLP then BDT) ---"
for name in $DATASETS; do
  for nv in 6 9; do
    fn="train_${name}_${nv}var"
    echo "$fn:"
    grep "Elapsed time for training" "$OUT_DIR/logs/${fn}.log" | sed 's/^/    /'
  done
done
echo
echo "--- Inference (MC): speed report ---"
for name in $DATASETS; do
  for nv in 6 9; do
    fn="infer_${name}_${nv}var"
    echo "$fn:"
    grep -E "Total Events|Total Time" "$OUT_DIR/logs/${fn}.log" | sed 's/^/    /'
  done
done
if [ -n "$DATA_VALIDATION_FILE" ]; then
  echo
  echo "--- Inference (Data): speed report ---"
  for nv in 6 9; do
    fn="infer_data_F18in_positives_${nv}var"
    echo "$fn:"
    grep -E "Total Events|Total Time" "$OUT_DIR/logs/${fn}.log" | sed 's/^/    /'
  done
fi
echo
echo "Full logs are in: $OUT_DIR/logs/"
