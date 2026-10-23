// Regenerates the Figure_8_Outbending plots (F18out_positives Data/MC comparison) by calling
// plot.C's existing Variable_Plots_DataMC_Article function directly -- plot.C's own main()
// (plot()) is never touched or run, only this one function is borrowed from it.
//
// Run from inside Lepton_ID_Analysis/ (plot.C and Files/ are referenced with relative paths):
//   root -l -b -q Plots/Results/Article/Figure_8_Outbending/generate_figure8_outbending.C
//
// Output lands in /Users/mariana/Work/Results/Article/ (plot.C's hardcoded save_dir, same as
// every other plot it produces) -- copy the relevant F18outpos_modFC_* files back into this
// folder / the Article/ root afterward if you want them refreshed here too.
void generate_figure8_outbending()
{
    gROOT->ProcessLine(".L plot.C");
    gROOT->ProcessLine(
        "Variable_Plots_DataMC_Article("
        "\"Files/9-BDT/F18out_positives_Final\", "
        "\"/Users/mariana/Documents/LeptonID/Electron_AI_PID/Validation/pos_F18out_BDT_modFC\", "
        "\"Files/Result/B_posit_F18out_9_BDT\", "
        "\"BDT\", 0.0, \"F18outpos_modFC\")"
    );
}
