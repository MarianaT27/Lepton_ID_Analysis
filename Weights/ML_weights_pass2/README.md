# ML_weights_pass2

BDT-only retraining round (pass2), one folder per dataset (`<Period><pos|neg>`), each with the
6-variable (`TMVAClassification_BDT_6.weights.xml`) and 9-variable
(`TMVAClassification_BDT_9.weights.xml`) BDT models.

**No MLP weights exist for this round** — pass2 only retrained BDT. If/when MLP weights for
pass2 are trained, add them here as `TMVAClassification_MLP_6.weights.xml` /
`TMVAClassification_MLP_9.weights.xml` alongside the corresponding BDT files, matching this same
naming convention.
