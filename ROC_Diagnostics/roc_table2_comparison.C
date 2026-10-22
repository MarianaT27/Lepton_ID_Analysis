#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"

void roc_table2_comparison()
{
    gROOT->SetBatch(kTRUE);
    gStyle->SetOptStat(0);

    // ---- 1. Pull the existing BDT MC curves from ROC_F18in.root (plot.C's hardcoded Data
    // points -- BDT6_data/BDT9_data -- and the MLP curves are deliberately NOT pulled in here;
    // only BDT MC curves + the Table2-sourced BDT Data points below are shown) ----
    TFile *fin = TFile::Open("/Users/mariana/Work/Results/Article/ROC_F18in.root");
    TGraph *gr_BDT6  = (TGraph*)fin->Get("BDT6")->Clone("BDT6");
    TGraph *gr_BDT9  = (TGraph*)fin->Get("BDT9")->Clone("BDT9");
    fin->Close();

    // ---- 2. Build equivalent Data points from ratios.C's Table2 arrays (F18in_positives) ----
    // x = TPR (signal efficiency), used as-is.
    // y = 1 - FPR: the raw arrays below store plain FPR, matching ratios.C's EqualArray(..., bgk=true)
    // convention (NEWArray[i] = 1.0 - Array[i]), so we apply the same flip here to match the
    // "1 - False Positives Rate" y-axis convention the existing ROC plot already uses.
    Double_t x_6bdt[15]    ={1.000000, 0.999838, 1.000340, 0.999756, 0.998804, 0.997805, 0.992298, 0.985255, 0.980048, 0.972718, 0.964648, 0.929359, 0.851448, 0.433276, 0.014856};
    Double_t xerr_6bdt[15] ={0.000103, 0.000150, 0.000107, 0.000301, 0.000366, 0.000461, 0.000891, 0.001226, 0.001419, 0.001651, 0.001873, 0.002596, 0.003594, 0.004996, 0.001199};
    Double_t fpr_6bdt[15]    ={1.000000, 0.992761, 0.919888, 0.773758, 0.579520, 0.431408, 0.354105, 0.274470, 0.233413, 0.156612, 0.114380, 0.044340, 0.019166, 0.002459, 0.002442};
    Double_t fprerr_6bdt[15] ={0.000466, 0.001953, 0.006082, 0.009359, 0.011039, 0.011075, 0.010695, 0.009981, 0.009463, 0.008134, 0.007127, 0.004626, 0.003103, 0.001214, 0.001211};

    Double_t x_9bdt[15]    ={1.000000, 1.000034, 1.000316, 1.001474, 1.000314, 0.996122, 0.988208, 0.977088, 0.969052, 0.957489, 0.942895, 0.879423, 0.750369, 0.263628, 0.004083};
    Double_t xerr_9bdt[15] ={0.000107, 0.000069, 0.000273, 0.000156, 0.000201, 0.000651, 0.001120, 0.001548, 0.001788, 0.002078, 0.002387, 0.003336, 0.004424, 0.004500, 0.000660};
    Double_t fpr_9bdt[15]    ={1.000000, 0.997262, 0.937316, 0.749899, 0.449542, 0.333106, 0.223482, 0.116092, 0.082696, 0.058521, 0.050196, 0.024687, 0.004065, 0.003549, 0.001242};
    Double_t fprerr_9bdt[15] ={0.000466, 0.001266, 0.005440, 0.009685, 0.011123, 0.010541, 0.009319, 0.007173, 0.006173, 0.005267, 0.004903, 0.003502, 0.001507, 0.001420, 0.000932};

    Double_t y_6bdt[15], y_9bdt[15];
    for (int i=0;i<15;i++){ y_6bdt[i] = 1.0 - fpr_6bdt[i]; y_9bdt[i] = 1.0 - fpr_9bdt[i]; }

    TGraphErrors *gr_BDT6_table2 = new TGraphErrors(15, x_6bdt, y_6bdt, xerr_6bdt, fprerr_6bdt);
    TGraphErrors *gr_BDT9_table2 = new TGraphErrors(15, x_9bdt, y_9bdt, xerr_9bdt, fprerr_9bdt);

    // ---- 3. Style everything ----
    gr_BDT9->SetLineColor(kRed);              gr_BDT9->SetLineWidth(4);
    gr_BDT6->SetLineColor(kOrange);           gr_BDT6->SetLineWidth(4);

    gr_BDT6_table2->SetMarkerStyle(25); gr_BDT6_table2->SetMarkerColor(kGreen+2);  gr_BDT6_table2->SetLineColor(kGreen+2);
    gr_BDT9_table2->SetMarkerStyle(25); gr_BDT9_table2->SetMarkerColor(kMagenta+2);gr_BDT9_table2->SetLineColor(kMagenta+2);

    // ---- 4. Draw ----
    TCanvas *c1 = new TCanvas("c1","c1",1100,850);
    TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle("F18in_positives: BDT MC ROC curves vs. Table2_TPR_FPR.txt data points ; True Positives Rate ; 1 - False Positives Rate");
    mg->Add(gr_BDT9, "L");
    mg->Add(gr_BDT6, "L");
    mg->Add(gr_BDT6_table2, "P");
    mg->Add(gr_BDT9_table2, "P");
    mg->Draw("A");
    mg->GetXaxis()->SetRangeUser(0.75, 1.025);
    mg->GetYaxis()->SetRangeUser(0.75, 1.01);

    Float_t areaBDT6=gr_BDT6->Integral()+0.5, areaBDT9=gr_BDT9->Integral()+0.5;

    TLegend *leg = new TLegend(0.12, 0.12, 0.55, 0.42);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.025);
    leg->SetLineWidth(0);
    leg->AddEntry(gr_BDT9, Form("BDT 9var MC (AUC=%.3f)", areaBDT9), "l");
    leg->AddEntry(gr_BDT6, Form("BDT 6var MC (AUC=%.3f)", areaBDT6), "l");
    leg->AddEntry(gr_BDT6_table2, "BDT6 Data (Table2_TPR_FPR.txt / ratios.C array)", "p");
    leg->AddEntry(gr_BDT9_table2, "BDT9 Data (Table2_TPR_FPR.txt / ratios.C array)", "p");
    leg->Draw();

    // Relative paths -- run this from inside Lepton_ID_Analysis/ so output lands in
    // ROC_Diagnostics/ alongside this script (e.g. `root -l -b -q ROC_Diagnostics/roc_table2_comparison.C`).
    TString outpdf = "ROC_Diagnostics/ROC_F18in_vs_Table2.pdf";
    c1->Print(outpdf);
    std::cout << "Saved: " << outpdf << std::endl;

    TString outroot = "ROC_Diagnostics/ROC_F18in_vs_Table2.root";
    TFile *fout = new TFile(outroot, "RECREATE");
    gr_BDT6->Write("BDT6");
    gr_BDT9->Write("BDT9");
    gr_BDT6_table2->Write("BDT6_data_Table2");
    gr_BDT9_table2->Write("BDT9_data_Table2");
    mg->Write("MultiGraph");
    c1->Write("Canvas");
    fout->Close();
    std::cout << "Saved: " << outroot << std::endl;
}
