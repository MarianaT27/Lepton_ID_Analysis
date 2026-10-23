#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include <cstdio>
#include <iostream>

// Computes TPR/FPR from the MC-scored Final.root trees (resP=signal/lepton, resN=background/pion)
// at score cut = 0.0, matching Table2_TPR_FPR.txt's cut_n=10 -> cuts[10]=0.0 convention, for all
// 6 datasets x 2 variable counts (BDT only, matching Table 2's model scope).
struct Row { TString label; TString name; };

// Binomial standard error on a proportion: sigma = sqrt(p*(1-p)/n). With these samples sitting
// in the hundreds of thousands of events, expect sub-percent (often sub-0.1%) errors.
void get_tpr_fpr(TString name, int nvar, Double_t &tpr, Double_t &tpr_err, Double_t &fpr, Double_t &fpr_err)
{
    TString dir = (nvar==6) ? "6-BDT" : "9-BDT";
    TString file = "/Users/mariana/Documents/LeptonID/rootValidation/" + dir + "/" + name + "_Final.root";
    TFile *f = TFile::Open(file);
    TTree *resP = (TTree*)f->Get("resP");
    TTree *resN = (TTree*)f->Get("resN");

    Long64_t nP = resP->GetEntries();
    Long64_t nN = resN->GetEntries();
    Long64_t TP = resP->GetEntries("scoreBDT_P>=0.0");
    Long64_t FP = resN->GetEntries("scoreBDT_N>=0.0");

    tpr = (Double_t)TP / nP;
    fpr = (Double_t)FP / nN;
    tpr_err = TMath::Sqrt(tpr*(1.0-tpr)/nP);
    fpr_err = TMath::Sqrt(fpr*(1.0-fpr)/nN);
    f->Close();
}

void table3_simulation_tpr_fpr()
{
    std::vector<Row> rows = {
        {"Spring 2019", "S19_positives"},
        {"Fall 2018 Inbending", "F18in_positives"},
        {"Fall 2018 Outbending", "F18out_positives"},
    };
    std::vector<Row> rows_neg = {
        {"Spring 2019", "S19_negatives"},
        {"Fall 2018 Inbending", "F18in_negatives"},
        {"Fall 2018 Outbending", "F18out_negatives"},
    };

    // Relative path -- run this from inside Lepton_ID_Analysis/ so output lands in
    // Training_Diagnostics/ alongside this script.
    FILE *fptr = fopen("Training_Diagnostics/Table3_TPR_FPR_Simulation.txt", "w");

    fprintf(fptr, "%% Positron (positives) configurations\n");
    fprintf(fptr, "\\begin{tabular}{|c|c|c|c|}\n");
    fprintf(fptr, "    \\hline\n");
    fprintf(fptr, "    Model & Configuration & TPR (Sim.) & FPR (Sim.) \\\\\n");
    fprintf(fptr, "    \\hline\n");
    fprintf(fptr, "    \\hline\n");
    for (int nv : {6, 9}) {
        fprintf(fptr, "    \\multirow{3}{*}{BDT-%d}", nv);
        for (auto &r : rows) {
            Double_t tpr, tpr_err, fpr, fpr_err;
            get_tpr_fpr(r.name, nv, tpr, tpr_err, fpr, fpr_err);
            fprintf(fptr, " & %s & %.2f\\%% \\pm %.2f\\%% & %.2f\\%% \\pm %.2f\\%% \\\\\n",
                    r.label.Data(), tpr*100, tpr_err*100, fpr*100, fpr_err*100);
            std::cout << "BDT-" << nv << " " << r.name << " : TPR=" << tpr*100 << "+-" << tpr_err*100
                      << "%  FPR=" << fpr*100 << "+-" << fpr_err*100 << "%" << std::endl;
        }
        fprintf(fptr, "    \\hline\n");
    }
    fprintf(fptr, "\\end{tabular}\n\n\n");

    fprintf(fptr, "%% Electron (negatives) configurations\n");
    fprintf(fptr, "\\begin{tabular}{|c|c|c|c|}\n");
    fprintf(fptr, "    \\hline\n");
    fprintf(fptr, "    Model & Configuration & TPR (Sim.) & FPR (Sim.) \\\\\n");
    fprintf(fptr, "    \\hline\n");
    fprintf(fptr, "    \\hline\n");
    for (int nv : {6, 9}) {
        fprintf(fptr, "    \\multirow{3}{*}{BDT-%d}", nv);
        for (auto &r : rows_neg) {
            Double_t tpr, tpr_err, fpr, fpr_err;
            get_tpr_fpr(r.name, nv, tpr, tpr_err, fpr, fpr_err);
            fprintf(fptr, " & %s & %.2f\\%% \\pm %.2f\\%% & %.2f\\%% \\pm %.2f\\%% \\\\\n",
                    r.label.Data(), tpr*100, tpr_err*100, fpr*100, fpr_err*100);
            std::cout << "BDT-" << nv << " " << r.name << " : TPR=" << tpr*100 << "+-" << tpr_err*100
                      << "%  FPR=" << fpr*100 << "+-" << fpr_err*100 << "%" << std::endl;
        }
        fprintf(fptr, "    \\hline\n");
    }
    fprintf(fptr, "\\end{tabular}\n");

    fclose(fptr);
    std::cout << "\nWrote table to scratchpad/Table3_TPR_FPR_Simulation.txt" << std::endl;
}
