# Training Diagnostics (Referee Response)

Everything below was recovered from the **original TMVA output files** (`TrainTree`/`TestTree`
and `Method_BDT` histograms), not re-derived or guessed — these are the exact training/test
splits, event counts, and overtraining histograms TMVA produced when `ML_weights_pass2` was
actually trained. No retraining was needed for any of this.

**Provenance:** matched to the production weight files in `Weights/ML_weights_pass2/` by exact
training-event count *and* timestamp (the weight XML's embedded `Date` field matches the source
`.root` file's mtime to the second in every case). Source files live outside this repo at
`/Users/mariana/Documents/LeptonID/rootTMVA/{v1,v2}/*.root` (10 files, ~24–52 MB each, not copied
in — see "What's not included" below).

| Config | Source file | Dataset dir |
|---|---|---|
| 6var F18in_positives  | `rootTMVA/v1/F18in_positives_TMVA.root` | `dataset_F18in_positives_mod` |
| 6var F18in_negatives  | `rootTMVA/v1/F18in_negatives_TMVA.root` | `dataset_F18in_negatives_mod` |
| 6var F18out_positives | `rootTMVA/v1/F18out_positives_TMVA.root` | `dataset_F18out_positives_mod` |
| 6var F18out_negatives | `rootTMVA/v1/F18out_negatives_TMVA.root` | `dataset_F18out_negatives_mod` |
| 6var S19_positives    | `rootTMVA/v2/S19_positives_mod_TMVA.root` | `dataset_S19_positives_mod` |
| 6var S19_negatives    | `rootTMVA/v1/S19_negatives_TMVA.root` | `dataset_S19_negatives_mod` |
| 9var F18in_positives  | `rootTMVA/v2/F18in_positives_TMVA.root` | `dataset_F18in_positives` |
| 9var F18in_negatives  | `rootTMVA/v2/F18in_negatives_TMVA.root` | `dataset_F18in_negatives` |
| 9var S19_positives    | `rootTMVA/v2/S19_positives_TMVA.root` | `dataset_S19_positives` |
| 9var S19_negatives    | `rootTMVA/v2/S19_negatives_TMVA.root` | `dataset_S19_negatives` |

**9var F18out_positives/F18out_negatives: no saved TMVA output file exists anywhere on this
machine.** Their weight XMLs give dates (Jan 20 2024 11:56 and Jan 18 2024 14:41) that don't match
the mtime of any `.root` file found — the full training/test diagnostic output for those two
specific configs appears to not have been kept. Everything below for 9var is 4/6 configs; 6var is
complete (6/6).

## 1. Train/test split

**Exactly 50/50 in every config**, with no exceptions — `TrainTree` and `TestTree` have identical
total entries, and identical signal/background breakdowns, in all 10 configs checked:

| Config | Train (S / B / total) | Test (S / B / total) |
|---|---|---|
| 6var F18in_positives  | 326,950 / 51,813 / 378,763 | 326,950 / 51,813 / 378,763 |
| 6var F18in_negatives  | 306,070 / 47,418 / 353,488 | 306,070 / 47,418 / 353,488 |
| 6var F18out_positives | 306,600 / 45,835 / 352,435 | 306,600 / 45,835 / 352,435 |
| 6var F18out_negatives | 326,248 / 53,262 / 379,510 | 326,248 / 53,262 / 379,510 |
| 6var S19_positives    | 326,721 / 48,754 / 375,475 | 326,721 / 48,754 / 375,475 |
| 6var S19_negatives    | 305,426 / 44,693 / 350,119 | 305,426 / 44,693 / 350,119 |
| 9var F18in_positives  | 326,950 / 51,813 / 378,763 | 326,950 / 51,813 / 378,763 |
| 9var F18in_negatives  | 306,070 / 47,418 / 353,488 | 306,070 / 47,418 / 353,488 |
| 9var S19_positives    | 326,721 / 48,754 / 375,475 | 326,721 / 48,754 / 375,475 |
| 9var S19_negatives    | 305,426 / 44,693 / 350,119 | 305,426 / 44,693 / 350,119 |

**This corrects `Timing_Results.md`, which currently says "~90/10"** — that was an unverified
guess written before this file existed; the actual split (from `PrepareTrainingAndTestTree("", "",
"SplitMode=Random:NormMode=NumEvents:!V")` with no `nTrain_Signal`/`nTrain_Background` specified)
is TMVA's 50/50 default. Fixing that line is a to-do, tracked separately from this file.

Note 6var and 9var give identical counts per dataset (e.g. both F18in_positives configs show
326,950/51,813) — expected, since the train/test split operates on *events*, not variables, so
using more input variables doesn't change which events land in which split.

## 2. Signal/background balance and reweighting

**Not balanced, not reweighted.** Across all 10 configs, signal:background ratio ranges from
**6.1:1 to 6.8:1** (combined train+test pool):

| Config | Signal (train+test) | Background (train+test) | S:B ratio |
|---|---|---|---|
| F18in_positives  | 653,900 | 103,626 | 6.31 |
| F18in_negatives  | 612,140 | 94,836  | 6.45 |
| F18out_positives | 613,200 | 91,670  | 6.69 |
| F18out_negatives | 652,496 | 106,524 | 6.13 |
| S19_positives    | 653,442 | 97,508  | 6.70 |
| S19_negatives    | 610,852 | 89,386  | 6.83 |

(6var and 9var give identical pool sizes per dataset, as above.)

The per-event `weight` branch in every `TrainTree` checked is uniformly **1.0** (verified
min=max=1 across all 10 files) — confirming `TMVAClassification.C`'s hardcoded
`signalWeight=1.0`/`backgroundWeight=1.0` was used exactly as written, with **no class-balance
correction or reweighting applied** to compensate for the ~6:1 imbalance. (TMVA's `UseBaggedBoost`
+ `BaggedSampleFraction=0.5` does randomly subsample each boosting iteration, and AdaBoost itself
upweights misclassified events during training — but neither of those is a deliberate class-ratio
correction at the input-weighting level; they're generic boosting mechanics applied on top of the
as-is imbalanced pool.)

## 3. Whether flat generated distributions were reweighted

**Could not be determined from anything in this repo or on this machine.** This is a question
about the Monte Carlo *generation* stage (kinematic reweighting applied before/during event
generation), which is upstream of and separate from TMVA's `signalWeight`/`backgroundWeight`
(confirmed above to be flat 1.0, which only rules out reweighting *at the TMVA training-input
stage*, not at generation). Answering this needs the generator/GEMC-side code or documentation,
which isn't part of this repository — would need to come from you or whoever ran the generation.

## 4. Overtraining check

KS test (test vs. train, via `TH1::KolmogorovTest`) on the BDT output distributions, for all 10
configs:

| Config | KS (signal) | KS (background) |
|---|---|---|
| 6var F18in_positives  | 0.887 | 0.955 |
| 6var F18in_negatives  | 0.792 | 0.933 |
| 6var F18out_positives | 0.838 | 0.639 |
| 6var F18out_negatives | 0.999 | 1.000 |
| 6var S19_positives    | 0.187 | 0.965 |
| 6var S19_negatives    | 0.089 | 0.883 |
| 9var F18in_positives  | 0.999 | 0.972 |
| 9var F18in_negatives  | 0.652 | 0.909 |
| 9var S19_positives    | 0.113 | 0.360 |
| 9var S19_negatives    | 0.727 | 0.804 |

No config falls below the conventional ~0.01 red-flag threshold TMVA documentation uses for
"likely overtrained" — the lowest values (6var S19_negatives signal, 0.089; 9var S19_positives
background, 0.360) are still comfortably above it, though visibly lower than most other configs
and worth a one-line mention if the response discusses this table.

**Representative plots** (train vs. test, signal vs. background, BDT response, same
config/numbers as the table above): `overtraining_F18in_positives_6var.pdf` and
`overtraining_F18in_positives_9var.pdf` in this folder.

## 5. Simulated vs. real event counts per configuration (R1's proportionality question)

**Not resolved — need your input.** I have the simulated/training counts (table above), but no
authoritative real-data event count per run period to compare against. What I have access to:
- A small, tightly-cut F18in_positives real-Data validation sample (2,165 events,
  `Files/Result_v2/pos_F18in_BDT_modFC.root`) — not representative of total real statistics.
- Real production files on ifarm (`F18in_All.root`, `F18out_All.root`, etc.) confirmed to have
  at least 1,000,000+ events each (we didn't determine their exact totals, since the inference
  benchmark converged well before reaching the end of the file).

Neither tells us the actual analyzed real-event count per period the referee is asking to compare
against. If you have this from the main analysis's cutflow/yield tables, that's the number to use;
otherwise I can get exact `GetEntries()` counts from the ifarm `_All.root` files if that's the
right comparison point — let me know which.

## 6. Performance per configuration (R1)

Per the original instructions, this is already covered by Table 2 (TPR/FPR in data) and the
per-config AUCs — nothing new generated here.

## What's not included in this repo

The 10 original `rootTMVA/v{1,2}/*.root` files (~350 MB total) that this analysis was extracted
from are **not** copied into `Lepton_ID_Analysis` — only the two representative overtraining PDFs
and this summary are included. Say if you want the full source `.root` files added too (for
someone else to independently rerun this same extraction).
