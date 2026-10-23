#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TString.h"
#include <cstdio>
#include <iostream>

// Computes AUC for BDT and MLP, 6var and 9var, for all 6 datasets, from the MC-scored Final.root
// trees -- same score-threshold-scan methodology ROC_All_Article uses (start/end ranges, 1-FPR
// y-axis, Integral()+0.5 convention), just looped over every config instead of one.
struct Row { TString label; TString name; };

Double_t compute_auc(TTree *resP, TTree *resN, TString scoreBranchP, TString scoreBranchN,
                      Double_t start, Double_t end, int npoints)
{
    Double_t x[2000], y[2000];
    Double_t segments = (end - start) / npoints;
    Double_t score_cut = start;
    Long64_t nP = resP->GetEntries();
    Long64_t nN = resN->GetEntries();
    for (int i = 0; i < npoints; i++) {
        Long64_t TP = resP->GetEntries(Form("%s>=%g", scoreBranchP.Data(), score_cut));
        Long64_t FP = resN->GetEntries(Form("%s>=%g", scoreBranchN.Data(), score_cut));
        Double_t TPR = (Double_t)TP / nP;
        Double_t FPR = (Double_t)FP / nN;
        x[i] = TPR; y[i] = 1.0 - FPR;
        score_cut = start + (i+1) * segments;
    }
    TGraph *g = new TGraph(npoints, x, y);
    Double_t auc = g->Integral() + 0.5;
    delete g;
    return auc;
}

void table4_auc_simulation()
{
    std::vector<Row> rows = {
        {"S19 positives",  "S19_positives"},
        {"S19 negatives",  "S19_negatives"},
        {"F18in positives", "F18in_positives"},
        {"F18in negatives", "F18in_negatives"},
        {"F18out positives","F18out_positives"},
        {"F18out negatives","F18out_negatives"},
    };

    const int NPTS = 200;
    Double_t startMLP=-1.25, endMLP=1.5, startBDT=-0.8, endBDT=0.8;

    FILE *fptr = fopen("Training_Diagnostics/Table4_AUC_Simulation.txt", "w");
    fprintf(fptr, "Config, nvar, BDT_AUC, MLP_AUC\n");

    for (auto &r : rows) {
        for (int nv : {6, 9}) {
            TString dir = (nv==6) ? "6-BDT" : "9-BDT";
            TString file = "/Users/mariana/Documents/LeptonID/rootValidation/" + dir + "/" + r.name + "_Final.root";
            TFile *f = TFile::Open(file);
            TTree *resP = (TTree*)f->Get("resP");
            TTree *resN = (TTree*)f->Get("resN");

            Double_t aucBDT = compute_auc(resP, resN, "scoreBDT_P", "scoreBDT_N", startBDT, endBDT, NPTS);
            Double_t aucMLP = compute_auc(resP, resN, "scoreMLP_P", "scoreMLP_N", startMLP, endMLP, NPTS);

            fprintf(fptr, "%s, %d, %.4f, %.4f\n", r.label.Data(), nv, aucBDT, aucMLP);
            std::cout << r.label << " " << nv << "var : BDT_AUC=" << aucBDT << " MLP_AUC=" << aucMLP << std::endl;
            f->Close();
        }
    }
    fclose(fptr);
    std::cout << "\nWrote Training_Diagnostics/Table4_AUC_Simulation.txt" << std::endl;
}
