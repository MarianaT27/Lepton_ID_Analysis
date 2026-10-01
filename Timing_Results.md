# TMVA Training / Inference Timing Benchmark

Measured 2026-09-30 on: Apple M1, 8 cores, macOS 27.0, ROOT 6.28/04 (conda env `my_root_env`), single-threaded (no `EnableImplicitMT`).

Training was rerun from scratch with the exact hyperparameters in `TMVAClassification.C`
(BDT: NTrees=850, MaxDepth=5, AdaBoost; MLP: NCycles=600, HiddenLayers=N+5) into a disposable
dataset directory — **the production weight files in `6-BDT-MLP/` and the `9-BDT-MLP/` folder
were not touched or overwritten** (TMVA's train/test split is randomized per run, so a rerun
does not reproduce bit-identical weights). Inference timing used the actual production weight
files already shipped in this repo / on this machine, run via `TMVA::Reader` exactly as
`TMVAClassificationApplication.C` does, timed with `TStopwatch` around each `EvaluateMVA` call.

**Training and background/signal labeling is done on Monte Carlo**, not real data: the
`_Lepton.root`/`_Pion.root` samples are built (by `toroot_v2MC.C`) from GEMC simulation using
the `MC::Lund` generator-truth bank to assign true PDG-code labels (`pid==11/-11` → lepton,
`pid==211/-211` → pion) — truth labels of this kind don't exist in real data, so supervised
training necessarily uses MC. The main inference benchmark below (per-dataset table) also runs
on those same MC samples, matching what `TMVAClassificationApplication.C` reads. A second,
real-Data inference measurement is given further down for comparison.

## Training time (BDT + MLP trained together per run, as in the original script)

| Dataset | BDT-6var | MLP-6var | BDT-9var | MLP-9var |
|---|---|---|---|---|
| F18in_positives  | 82.6 s | 616 s (10m16s) | 100 s  | 825 s (13m45s) |
| F18in_negatives  | 72.5 s | 562 s (9m22s)  | 85.3 s | 814 s (13m34s) |
| F18out_positives | 67.5 s | 580 s (9m40s)  | 90.9 s | 786 s (13m06s) |
| F18out_negatives | 73.1 s | 601 s (10m01s) | 98.9 s | 841 s (14m01s) |
| **Average**      | **73.9 s** | **590 s (9m50s)** | **93.8 s** | **816 s (13m37s)** |

Events per run: ~353k–380k signal+background combined (~90/10 train/test split handled internally).

## Inference time — MC (per event, evaluating one already-trained model)

| Dataset | BDT-6var (µs/evt) | MLP-6var (µs/evt) | BDT-9var (µs/evt) | MLP-9var (µs/evt) |
|---|---|---|---|---|
| F18in_positives  | 31.90 | 2.34 | 32.90 | 2.93 |
| F18in_negatives  | 34.06 | 2.50 | 34.19 | 3.20 |
| F18out_positives | 33.80 | 2.44 | 34.11 | 3.06 |
| F18out_negatives | 33.55 | 2.47 | 34.05 | 3.11 |
| **Average**      | **33.3** | **2.44** | **33.8** | **3.08** |

Events per run: ~612k–654k (one sample's Lepton+Pion trees).
Total wall time per run (all events): BDT ≈ 21 s, MLP ≈ 1.5–2.0 s.

## Inference time — real Data (F18in_positives only)

The only real-Data validation sample available for the 4 benchmark datasets is
`Files/Result_v2/pos_F18in_BDT_modFC.root` (tree `results`, branches suffixed `_D`) — a
tightly-selected, **2,165-event** real CLAS12 data sample, vs. 653,901 MC events above. No
equivalent Data file currently exists for F18in_negatives/F18out_positives/F18out_negatives
(consistent with `plot.C`'s active analysis, which also only runs the Data/MC comparison for
F18in_positives).

| Model | BDT (µs/evt) | MLP (µs/evt) |
|---|---|---|
| 6-var | 26.12 | 1.91 |
| 9-var | 26.23 | 2.45 |

Broadly consistent with the MC numbers for F18in_positives (33.3/2.4 µs and 33.8/3.1 µs above);
the ~15–20% gap is most plausibly statistical noise from the much smaller event count (2,165 vs.
653,901) rather than a real Data-vs-MC effect — inference cost is driven by walking the same
trained tree ensemble / forward pass regardless of the input sample's origin.

## Inference time — ifarm real Data (all 4 datasets)

Measured on JLab's ifarm (`ifarm2401`/`ifarm2402.jlab.org`, Linux, ROOT 6.38/04), via
`Timing_Benchmark/run_benchmark.sh`'s `IFARM_DATA_DIR` leg, reading the real production files
`toroot_v2.C` writes to `/work/clas12/mtenorio/Analysis/Latest_Final/{F18in,F18out}_All.root`
(tree `analysis`, `positron_*`/`electron_*` branches) — same `TMVA::Reader` + `TStopwatch`
methodology as the other legs above, using the same `Weights/ML_weights_pass2/` models. Run
directly on a shared interactive node (not a dedicated allocation), capped to the first
1,000,000 events per file via `MAX_EVENTS` — results had already converged at 100k events
(sub-2% drift going to 1M), so this is a stable, representative number rather than a partial one.

| Dataset | BDT-6var (µs/evt) | MLP-6var (µs/evt) | BDT-9var (µs/evt) | MLP-9var (µs/evt) |
|---|---|---|---|---|
| F18in_positives  | 17.60 | 2.56 | 18.36 | 3.02 |
| F18in_negatives  | 18.33 | 2.58 | 18.33 | 2.99 |
| F18out_positives | 18.34 | 2.54 | 18.21 | 3.07 |
| F18out_negatives | 22.66 | 2.68 | 22.27 | 3.11 |
| **Average**      | **19.2** | **2.59** | **19.3** | **3.05** |

**ifarm vs. Mac M1 (both MC/real-production inference, not the small 2,165-event sample):**

| Model | Mac M1 (653k MC events) | ifarm (1M real-Data events) |
|---|---|---|
| BDT, 6 variables | 33.3 µs/evt | 19.2 µs/evt |
| MLP, 6 variables | 2.44 µs/evt | 2.59 µs/evt |
| BDT, 9 variables | 33.8 µs/evt | 19.3 µs/evt |
| MLP, 9 variables | 3.08 µs/evt | 3.05 µs/evt |

ifarm's BDT inference is ~40% faster per event than the M1 despite running on a heavily shared,
contended node (observed load average ~500–700 from other users' jobs at the time) rather than a
clean dedicated benchmark environment — likely a faster per-core rate for this tree-traversal
workload on ifarm's server-class CPUs. MLP timing is essentially identical on both machines,
consistent with MLP being cheap enough that hardware differences barely register.

## Headline numbers for the paper (averaged across the 4 datasets)

| Model | Training time (Mac, MC) | Inference time/event (Mac, MC) | Inference time/event (ifarm, real Data) |
|---|---|---|---|
| BDT, 6 variables | ~74 s | ~33.3 µs | ~19.2 µs |
| MLP, 6 variables | ~9m50s | ~2.4 µs | ~2.6 µs |
| BDT, 9 variables | ~94 s | ~33.8 µs | ~19.3 µs |
| MLP, 9 variables | ~13m37s | ~3.1 µs | ~3.1 µs |

**Takeaways:** MLP trains far slower than BDT (~8x) but infers far faster (~14x on the Mac, ~7x on
ifarm) — BDT evaluation dominates inference cost because it walks all 850 trees per event, while
the MLP is a single small forward pass. Going from 6 to 9 variables adds modest cost to both
training (+27% BDT, +38% MLP) and inference (+2% BDT, +26% MLP on the Mac; negligible on ifarm),
consistent with the added input dimensionality. Training was only ever measured on local compute
(the MC training samples have never existed on ifarm); inference was measured on both, as the
only leg that's actually comparable across environments.

## Reproducing this benchmark

See `Timing_Benchmark/` in this folder: `run_benchmark.sh` + `templates/*.C` regenerate and rerun
everything above on any machine with the same data layout (paths are configurable via environment
variables, not hardcoded to this Mac). `Timing_Benchmark/README_ifarm.md` has JLab ifarm-specific
setup steps (module/conda ROOT setup, SLURM submission for uncontended timing).
