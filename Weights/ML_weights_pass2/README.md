# ML_weights_pass2

Trained BDT and MLP weight files, one folder per dataset (`<Period><pos|neg>`), each with the
6-variable and 9-variable version of both methods:

- `TMVAClassification_BDT_6.weights.xml` / `TMVAClassification_BDT_9.weights.xml`
- `TMVAClassification_MLP_6.weights.xml` / `TMVAClassification_MLP_9.weights.xml`

These are the same trained models used throughout this repo (`Timing_Results.md`,
`Timing_Benchmark/`) — the BDT files were originally packaged here for ifarm use without their
MLP counterparts; the MLP files were added from the original training output
(`6-BDT-MLP`/`9-BDT-MLP` on the training machine) to complete the set with matching naming.
