#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TF1.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TH3F.h"
#include "THStack.h"
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;



void setupCanvas(TCanvas* canvas){
    
    canvas->SetFillColor(0);
    canvas->SetBorderMode(0);
    canvas->SetBorderSize(0);
    canvas->SetFrameFillColor(0);
    canvas->SetFrameBorderMode(0);
    //canvas->SetGrid();
    
}

void Variable_Plots_TMVA(TString infile,TString name_model,Float_t score_cut, TString outfile)
{
    gROOT->SetBatch(kTRUE);
    
    gStyle->SetPaintTextFormat("4.1f");
    gStyle->SetPalette(kLightTemperature);
    gStyle->SetOptStat(0);
    gStyle->SetLabelSize(.04, "xyz");
    gStyle->SetTitleSize(.052, "xyz");
    gStyle->SetTitleSize(.07, "t");
    gStyle->SetFrameLineWidth(1);
    gStyle->SetLineWidth(1);
    gStyle->SetHistLineWidth(1);
    gStyle->SetMarkerStyle(13);
    gStyle->SetTitleW(0.8); // per cent of the pad width
    gStyle->SetTitleH(0.1); // per cent of the pad height
    
    
    std::vector<vector<TString>> labels1D{
        {"P", "P", "custom_range", "0.", "14.",
            "custom_binning", "100", "", "legend", "P"},
        
        {"Theta", "#theta", "custom_range", "0.", "1",
            "custom_binning", "100", "", "legend", "electron_SF"},
        
        {"Phi", "#phi", "custom_range", "-3.2", "3.2",
            "custom_binning", "100", "", "legend", "electron_SF"},
        
        {"SFPCAL", "SF PCAL", "custom_range", "0.", "0.3",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"SFECIN", "SF ECIN", "custom_range", "0.", "0.25",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"SFECOUT", "SF ECOUT", "custom_range", "0.", "0.25",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"m2PCAL", "m2 PCAL", "custom_range", "0.", "200",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"m2ECIN", "m2 ECIN", "custom_range", "0.", "500",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"m2ECOUT", "m2 ECOUT", "custom_range", "0.", "500",
            "custom_binning", "100", "", "legend", "positron_SF_4"}
    };
    
    //TFile *Data_file = new TFile("Documents/Lepton_ID_root/"+name+"_Pion.root");
    
    TFile *Data_file = new TFile(infile+".root");;
    TTree *Data_tree = (TTree *)Data_file->Get("resP");
    TTree *Data_tree_1 = (TTree *)Data_file->Get("resN");
    cout<<"HERE1"<<endl;
    int entries_tree = Data_tree->GetEntries();
    int entries_tree_1 = Data_tree_1->GetEntries();
    cout<<"HERE2"<<endl;
    TCut cut_0 = "";
    TCut cut_1 = "";
    
    
    
    Float_t P_P, Theta_P, Phi_P,SFPCAL_P,SFECIN_P,SFECOUT_P, m2PCAL_P, m2ECIN_P, m2ECOUT_P;
    Float_t P_N, Theta_N, Phi_N,SFPCAL_N,SFECIN_N,SFECOUT_N, m2PCAL_N, m2ECIN_N, m2ECOUT_N;
    
    Float_t scoreBDT_P,scoreBDT_N,score,scoreMLP_P,scoreMLP_N;
    
    //---------------Tree 1-------------------------------
    //TMVAPos.root, has true positives and false negatives
    //--------------------------------------------------
    //Data: #positives, #True positives, #False Negatives #score
    Data_tree->SetBranchAddress("scoreBDT_P",&scoreBDT_P);
    Data_tree->SetBranchAddress("scoreMLP_P",&scoreMLP_P);
    //positives sample variables
    //Data_tree->SetBranchAddress("P_P", &P_P);
    Data_tree->SetBranchAddress("P_P", &P_P);
    Data_tree->SetBranchAddress("Theta_P", &Theta_P);
    Data_tree->SetBranchAddress("Phi_P", &Phi_P);
    Data_tree->SetBranchAddress("SFPCAL_P", &SFPCAL_P);
    Data_tree->SetBranchAddress("SFECIN_P", &SFECIN_P);
    Data_tree->SetBranchAddress("SFECOUT_P", &SFECOUT_P);
    Data_tree->SetBranchAddress("m2PCAL_P", &m2PCAL_P);
    Data_tree->SetBranchAddress("m2ECIN_P", &m2ECIN_P);
    Data_tree->SetBranchAddress("m2ECOUT_P", &m2ECOUT_P);
    
    
    //---------------Tree 1-------------------------------
    //TMVAPos.root, has true positives and false negatives
    //--------------------------------------------------
    //Data: #negatives, #True negatives, #False positives #score
    Data_tree_1->SetBranchAddress("scoreBDT_N",&scoreBDT_N);
    Data_tree_1->SetBranchAddress("scoreMLP_N",&scoreMLP_N);
    //negatives sample variables
    //Data_tree->SetBranchAddress("P_N", &P_N);
    Data_tree_1->SetBranchAddress("P_N", &P_N);
    Data_tree_1->SetBranchAddress("Theta_N", &Theta_N);
    Data_tree_1->SetBranchAddress("Phi_N", &Phi_N);
    Data_tree_1->SetBranchAddress("SFPCAL_N", &SFPCAL_N);
    Data_tree_1->SetBranchAddress("SFECIN_N", &SFECIN_N);
    Data_tree_1->SetBranchAddress("SFECOUT_N", &SFECOUT_N);
    Data_tree_1->SetBranchAddress("m2PCAL_N", &m2PCAL_N);
    Data_tree_1->SetBranchAddress("m2ECIN_N", &m2ECIN_N);
    Data_tree_1->SetBranchAddress("m2ECOUT_N", &m2ECOUT_N);
    
    
    TString name_pdf=outfile+"_Plots";
    TString n1="_P";
    TString n2="_N";
    TString label = "True Positives";
    TString label_1 = "Negative Sample";
    TString label_2 = "True Negatives";
    TString label_3 = "Predicted Negative";
    
    int PS=entries_tree;
    int NS=entries_tree_1;
    int PP=0;
    int PN=0;
    
    TCanvas *cancG0 = new TCanvas("cancG0", "cancG0", 1500, 1200);
    cancG0->Divide(3,3,0.006,0.014,0);
    //cancG0->Divide(3,3);
    
    TCanvas *cancG1 = new TCanvas("cancG1", "cancG1", 1500, 1200);
    cancG1->Divide(3,3,0.01,0.01,0);
    
    //TCanvas *cancG2 = new TCanvas("cancG2", "cancG2", 1500, 1200);
    //cancG2->Divide(3,3,0.01,0.01,0);
    
    //TCanvas *cancG3 = new TCanvas("cancG3", "cancG3", 1500, 1200);
    //cancG3->Divide(3,3,0.01,0.01,0);
    
    //gStyle->SetLabelSize(0.04, "x");
    
    cout << "-------------------------------------------------" << endl;
    cout << "Plotting TP and TN" << endl;
    cout << "-------------------------------------------------" << endl;
    
    for (int i = 0; i < labels1D.size(); i++){
        int PP=0;
        int PN=0;
        cancG0->cd(i+1);
        gPad->SetLeftMargin(0.18);
        
        //////////////////////////////////////////////////
        // Set options for each label
        TString label1 = labels1D[i][0];
        TString xAxis_label = labels1D[i][1];
        TString range_x_option = labels1D[i][2];
        TString min_x_option = labels1D[i][3];
        TString max_x_option = labels1D[i][4];
        TString binning_x_option = labels1D[i][5];
        TString nb_x_bins = labels1D[i][6];
        TString string_cut = labels1D[i][7];
        TString legend_option = labels1D[i][8];
        TString output_string = labels1D[i][9];
        //////////////////////////////////////////////////
        cout << "Doing 1D " << label1 << " plot" << endl;
        
        
        TCut cut = string_cut.Data();
        
        TString cut_string = cut.GetTitle();
        
        float min_X_histo_ini;
        float max_X_histo_ini;
        
        int nBins_X = 20;
        //Extract custom range options
        if (range_x_option == "custom_range"){
            //cout << "Reading range " << endl;
            min_X_histo_ini = stof((string)min_x_option.Data());
            max_X_histo_ini = stof((string)max_x_option.Data());
        }
        
        //Extract custom bins options
        if (binning_x_option == "custom_binning"){
            //cout << "Reading # of data bins: " << nb_x_bins.Data() << endl;
            nBins_X = stoi((string)nb_x_bins.Data());
        }
        
        //True Positives:Positives from the positive sample
        TH1F *Data_hist = new TH1F("Data_hist", "Data_hist", nBins_X, min_X_histo_ini, max_X_histo_ini);
        if(name_model=="MLP")
            Data_tree->Draw(label1 +n1+ ">>Data_hist", Form(cut_1 * cut&&"scoreMLP_P>=%g",score_cut));
        if(name_model=="BDT")
            Data_tree->Draw(label1 +n1+ ">>Data_hist",Form( cut_1 * cut&&"scoreBDT_P>=%g",score_cut));
        
        
        
        
        //True Negatives: Negatives from the negative sample
        TH1F *Data_hist_2 = new TH1F("Data_hist_2", "Data_hist_2", nBins_X, min_X_histo_ini, max_X_histo_ini);
        if(name_model=="MLP")
            Data_tree_1->Draw(label1 +n2+ ">>Data_hist_2", Form(cut_1 * cut&&"scoreMLP_N<%g",score_cut));
        if(name_model=="BDT")
            Data_tree_1->Draw(label1 +n2+ ">>Data_hist_2",Form( cut_1 * cut&&"scoreBDT_N<%g",score_cut));
        
        
        Data_hist->Scale(1./Data_hist->Integral());
        Data_hist_2->Scale(1./Data_hist_2->Integral());
        
        
        Data_hist->SetLineColorAlpha(kCyan+2,0.35);
        Data_hist->SetMarkerColor(kCyan-3);
        Data_hist->SetFillColorAlpha(kCyan+2,0.35);
        Data_hist->SetMarkerStyle(20);
        Data_hist->SetFillStyle(1001);
        Data_hist->SetTitle(";" + xAxis_label+"; Normalized Counts");
        
        
        //Data_hist_1->SetLineWidth(2);
        Data_hist_2->SetLineColor(kAzure-9);
        Data_hist_2->SetMarkerColor(kRed-7);
        //Data_hist_2->SetMarkerStyle(5);
        Data_hist_2->SetFillStyle(1);
        Data_hist_2->SetFillColor(kRed-7);
        Data_hist_2->SetTitle(";" + xAxis_label+"; Normalized Counts");
        if(i==2)
            Data_hist->SetMaximum(std::max(Data_hist_2->GetMaximum(),Data_hist->GetMaximum()) * 1.5);
        else
            Data_hist->SetMaximum(std::max(Data_hist_2->GetMaximum(),Data_hist->GetMaximum()) * 1.2);
        
        
        
        gStyle->SetLegendTextSize(0.045);
        Data_hist->Draw("PL ");
        Data_hist_2->SetMarkerStyle(20);
        Data_hist_2->Draw("PEL same");
        
        int positive=Data_hist->GetEntries();
        int negative=Data_hist_2->GetEntries();
        
        
        if(i==0){
            //auto legend = new TLegend(0.65, 0.87, 0.87, 0.67);
            auto legend = new TLegend();
            legend->AddEntry(Data_hist, "True Positives", "p");
            legend->AddEntry(Data_hist_2,"True Negatives", "p");
            legend->SetFillStyle(0);
            legend->SetLineWidth(0);
            legend->Draw("same ");
        }
        
    }
    
    
    
    cout<<"Saving in "<<name_pdf<<".pdf"<<endl;
    cancG0->SaveAs(name_pdf+"_TPosNeg.pdf");
    
    cout << "-------------------------------------------------" << endl;
    cout << "Plotting TP and FP" << endl;
    cout << "-------------------------------------------------" << endl;
    
    for (int i = 0; i < labels1D.size(); i++){
        int PP=0;
        int PN=0;
        cancG1->cd(i+1);
        
        //////////////////////////////////////////////////
        // Set options for each label
        TString label1 = labels1D[i][0];
        TString xAxis_label = labels1D[i][1];
        TString range_x_option = labels1D[i][2];
        TString min_x_option = labels1D[i][3];
        TString max_x_option = labels1D[i][4];
        TString binning_x_option = labels1D[i][5];
        TString nb_x_bins = labels1D[i][6];
        TString string_cut = labels1D[i][7];
        TString legend_option = labels1D[i][8];
        TString output_string = labels1D[i][9];
        //////////////////////////////////////////////////
        cout << "Doing 1D " << label1 << " plot" << endl;
        
        
        TCut cut = string_cut.Data();
        
        TString cut_string = cut.GetTitle();
        
        float min_X_histo_ini;
        float max_X_histo_ini;
        
        int nBins_X = 20;
        //Extract custom range options
        if (range_x_option == "custom_range"){
            //cout << "Reading range " << endl;
            min_X_histo_ini = stof((string)min_x_option.Data());
            max_X_histo_ini = stof((string)max_x_option.Data());
        }
        
        //Extract custom bins options
        if (binning_x_option == "custom_binning"){
            //cout << "Reading # of data bins: " << nb_x_bins.Data() << endl;
            nBins_X = stoi((string)nb_x_bins.Data());
        }
        
        
        //True Positives
        TH1F *Data_hist_1 = new TH1F("Data_hist_1", "Data_hist_1", nBins_X, min_X_histo_ini, max_X_histo_ini);
        if(name_model=="MLP")
            Data_tree->Draw(label1 +n1+ ">>Data_hist_1", Form(cut_1 * cut&&"scoreMLP_P>=%g",score_cut));
        if(name_model=="BDT")
            Data_tree->Draw(label1 +n1+ ">>Data_hist_1",Form( cut_1 * cut&&"scoreBDT_P>=%g",score_cut));
        
        //False Positives
        TH1F *Data_hist_3 = new TH1F("Data_hist_3", "Data_hist_3", nBins_X, min_X_histo_ini, max_X_histo_ini);
        if(name_model=="MLP")
            Data_tree_1->Draw(label1 +n2+ ">>Data_hist_3", Form(cut_1 * cut&&"scoreMLP_N>=%g",score_cut));
        if(name_model=="BDT")
            Data_tree_1->Draw(label1 +n2+ ">>Data_hist_3",Form( cut_1 * cut&&"scoreBDT_N>=%g",score_cut));
        
        
        
        Data_hist_1->Scale(1./Data_hist_1->Integral());
        Data_hist_3->Scale(1./Data_hist_3->Integral());
        
        
        
        Data_hist_1->SetMarkerStyle(20);
        Data_hist_1->SetLineColorAlpha(kCyan+2,0.35);
        Data_hist_1->SetMarkerColor(kCyan-3);
        //Data_hist_1->SetLineColor(kAzure-6);
        Data_hist_1->SetTitle(";" + xAxis_label+";counts");
        
        
        //Data_hist_1->SetLineWidth(2);
        Data_hist_3->SetLineColor(0);
        Data_hist_3->SetFillStyle(0);
        Data_hist_3->SetFillColor(0);
        Data_hist_3->SetMarkerColor(kOrange-3);
        Data_hist_3->SetTitle(";" + xAxis_label+";counts");
        
        Data_hist_1->SetMaximum(std::max(Data_hist_3->GetMaximum(),Data_hist_1->GetMaximum()) * 1.5);
        
        //Data_hist_1->Draw();
        
        int TP=Data_hist_1->GetEntries();
        int FP=Data_hist_3->GetEntries();
        
        TRatioPlot *rp = new TRatioPlot(Data_hist_1, Data_hist_3);
        cancG1->SetTicks(0, 1);
        rp->Draw();
        rp->GetLowerRefGraph()->SetMinimum(0);
        rp->GetLowerRefGraph()->SetMaximum(2);
        rp->GetLowYaxis()->SetNdivisions(505);
        //rp->SetSeparationMargin(0.0);
        rp->GetUpperPad()->cd();
        Data_hist_1->Draw("P same");
        
        Data_hist_3->SetMarkerStyle(20);
        Data_hist_3->Draw("PE same");
        
        auto legend = new TLegend(0.65, 0.87, 0.87, 0.67);
        legend->AddEntry(Data_hist_1, Form("True Positives (%d)", TP), "p");
        legend->AddEntry(Data_hist_3, Form("False Positives (%d)", FP), "p");
        legend->SetFillStyle(0);
        legend->SetLineWidth(0);
        
        
        legend->Draw("same ");
        
    }
    
    cout<<"Saving in "<<name_pdf<<".pdf"<<endl;
    cancG1->SaveAs(name_pdf + "_EXTRA.pdf");
    
    
    
    cout << "-------------------------------------------------" << endl;
    cout << "Plotting NS and FP" << endl;
    cout << "-------------------------------------------------" << endl;
    TCanvas *cancG2 = new TCanvas("cancG2", "cancG2", 1500, 1200);
    gStyle->SetTitleSize(.04, "xyz");
    for (int i = 0; i < labels1D.size(); i++){
      
        
        //////////////////////////////////////////////////
        // Set options for each label
        TString label1 = labels1D[i][0];
        TString xAxis_label = labels1D[i][1];
        TString range_x_option = labels1D[i][2];
        TString min_x_option = labels1D[i][3];
        TString max_x_option = labels1D[i][4];
        TString binning_x_option = labels1D[i][5];
        TString nb_x_bins = labels1D[i][6];
        TString string_cut = labels1D[i][7];
        TString legend_option = labels1D[i][8];
        TString output_string = labels1D[i][9];
        //////////////////////////////////////////////////
        cout << "Doing 1D " << label1 << " plot" << endl;
        
        
        TCut cut = string_cut.Data();
        
        TString cut_string = cut.GetTitle();
        
        float min_X_histo_ini;
        float max_X_histo_ini;
        
        int nBins_X = 20;
        //Extract custom range options
        if (range_x_option == "custom_range"){
            //cout << "Reading range " << endl;
            min_X_histo_ini = stof((string)min_x_option.Data());
            max_X_histo_ini = stof((string)max_x_option.Data());
        }
        
        //Extract custom bins options
        if (binning_x_option == "custom_binning"){
            //cout << "Reading # of data bins: " << nb_x_bins.Data() << endl;
            nBins_X = stoi((string)nb_x_bins.Data());
        }
        
        
        //Negative Sample
        TH1F *Data_hist_1 = new TH1F("Data_hist_1", "Data_hist_1", nBins_X, min_X_histo_ini, max_X_histo_ini);
        Data_tree_1->Draw(label1 +n2+ ">>Data_hist_1", cut_1 * cut);
        
        //False Positives
        TH1F *Data_hist_3 = new TH1F("Data_hist_3", "Data_hist_3", nBins_X, min_X_histo_ini, max_X_histo_ini);
        if(name_model=="MLP")
            Data_tree_1->Draw(label1 +n2+ ">>Data_hist_3", Form(cut_1 * cut&&"scoreMLP_N>=%g",score_cut));
        if(name_model=="BDT")
            Data_tree_1->Draw(label1 +n2+ ">>Data_hist_3",Form( cut_1 * cut&&"scoreBDT_N>=%g",score_cut));
        
        
        
        //Data_hist_1->Scale(1./Data_hist_1->Integral());
        //Data_hist_3->Scale(1./Data_hist_3->Integral());
        
        
        
        //Data_hist->SetLineWidth(2);
        Data_hist_1->SetLineColor(kRed);
        Data_hist_1->SetLineWidth(2);
        Data_hist_1->SetFillColor(0);
        Data_hist_1->SetFillStyle(0);
        Data_hist_1->SetTitle(";" + xAxis_label+";");
        
        
        //Data_hist_1->SetLineWidth(2);
        Data_hist_3->SetLineColor(0);
        Data_hist_3->SetFillStyle(0);
        Data_hist_3->SetFillColor(0);
        Data_hist_3->SetMarkerColor(kOrange-3);
        Data_hist_3->SetTitle(";" + xAxis_label+";");
        
        Data_hist_3->SetMaximum(std::max(Data_hist_3->GetMaximum(),Data_hist_1->GetMaximum()) * 1.2);
        
        
        
        int FP=Data_hist_3->GetEntries();
        
        TRatioPlot *rp = new TRatioPlot(Data_hist_3, Data_hist_1);
        cancG2->SetTicks(0, 1);
        rp->Draw();
        rp->GetLowerRefGraph()->SetMinimum(0.0);
        rp->GetLowerRefGraph()->SetMaximum(0.2);
        rp->GetLowYaxis()->SetNdivisions(505);
        rp->GetUpperPad()->SetLeftMargin(0.14);
        rp->GetLowerPad()->SetLeftMargin(0.14);
        rp->GetUpperRefYaxis()->SetTitleOffset(1.8);
        rp->GetLowerRefYaxis()->SetTitleOffset(1.6);
        rp->GetUpperRefYaxis()->SetTitle("Counts");
        rp->GetLowerRefYaxis()->SetTitle("Ratio");
        //rp->SetSeparationMargin(0.0);
        rp->GetUpperPad()->cd();
        
        //Data_hist_1->SetLineColor(20);
        Data_hist_1->Draw("same");
        Data_hist_3->SetMarkerStyle(20);
        Data_hist_3->SetMarkerSize(3);
        Data_hist_3->Draw("PE same");
        
        if(i==0){
            auto legend = new TLegend(0.66, 0.89, 0.87, 0.67);
            legend->AddEntry(Data_hist_1,"Negative Sample", "l");
            legend->AddEntry(Data_hist_3,"False Positives", "p");
            legend->SetFillStyle(0);
            legend->SetLineWidth(0);
            
            
            legend->Draw("same ");
        }
        cout<<"Saving in "<<name_pdf<<".pdf"<<endl;
        cancG2->SaveAs(name_pdf +"_"+label1+ "_NSFP.pdf");
        cancG2->Clear();
        
    }
    
    
    
    
    
    cout << "-------------------------------------------------" << endl;
    cout << "Plotting Positive Sample and TP" << endl;
    cout << "-------------------------------------------------" << endl;
    gStyle->SetTitleSize(.04, "xyz");
    TCanvas *cancG3 = new TCanvas("cancG3", "cancG3", 1500, 1200);
    
    for (int i = 0; i < labels1D.size(); i++){
        int PP=0;
        int PN=0;
        //cancG3->cd(i+1);
        
        //////////////////////////////////////////////////
        // Set options for each label
        TString label1 = labels1D[i][0];
        TString xAxis_label = labels1D[i][1];
        TString range_x_option = labels1D[i][2];
        TString min_x_option = labels1D[i][3];
        TString max_x_option = labels1D[i][4];
        TString binning_x_option = labels1D[i][5];
        TString nb_x_bins = labels1D[i][6];
        TString string_cut = labels1D[i][7];
        TString legend_option = labels1D[i][8];
        TString output_string = labels1D[i][9];
        //////////////////////////////////////////////////
        cout << "Doing 1D " << label1 << " plot" << endl;
        
        
        TCut cut = string_cut.Data();
        
        TString cut_string = cut.GetTitle();
        
        float min_X_histo_ini;
        float max_X_histo_ini;
        
        int nBins_X = 20;
        //Extract custom range options
        if (range_x_option == "custom_range"){
            //cout << "Reading range " << endl;
            min_X_histo_ini = stof((string)min_x_option.Data());
            max_X_histo_ini = stof((string)max_x_option.Data());
        }
        
        //Extract custom bins options
        if (binning_x_option == "custom_binning"){
            //cout << "Reading # of data bins: " << nb_x_bins.Data() << endl;
            nBins_X = stoi((string)nb_x_bins.Data());
        }
        
        //Positive Sample
        TH1F *Data_hist = new TH1F("Data_hist", "Data_hist", nBins_X, min_X_histo_ini, max_X_histo_ini);
        Data_tree->Draw(label1 +n1+">>Data_hist", cut_0 * cut);
        
        int positive=Data_tree->GetEntries();
        
        
        
        //True Positive
        TH1F *Data_hist_2 = new TH1F("Data_hist_2", "Data_hist_2", nBins_X, min_X_histo_ini, max_X_histo_ini);
        if(name_model=="MLP")
            Data_tree->Draw(label1 +n1+ ">>Data_hist_2", Form(cut_1 * cut&&"scoreMLP_P>=%g",score_cut));
        if(name_model=="BDT")
            Data_tree->Draw(label1 +n1+ ">>Data_hist_2",Form( cut_1 * cut&&"scoreBDT_P>=%g",score_cut));
        
        
        //Data_hist->Scale(1./Data_hist->Integral());
        //Data_hist_2->Scale(1./Data_hist_2->Integral());
        
        
        Data_hist->SetLineWidth(2);
        Data_hist->SetLineColor(kAzure-6);
        Data_hist->SetFillColor(0);
        Data_hist->SetFillStyle(0);
        Data_hist->SetTitle(";" + xAxis_label+";");
        
        
        //Data_hist_1->SetLineWidth(2);
        //Data_hist_2->SetLineColor(kAzure-9);
        Data_hist_2->SetMarkerColor(kCyan-3);
        //Data_hist_2->SetMarkerStyle(5);
        Data_hist_2->SetFillStyle(0);
        Data_hist_2->SetFillColor(0);
        Data_hist_2->SetTitle(" ;" + xAxis_label+";");
        //if(i==2)
        //    Data_hist->SetMaximum(std::max(Data_hist_2->GetMaximum(),Data_hist->GetMaximum()) * 1.5);
        //else
            Data_hist->SetMaximum(std::max(Data_hist_2->GetMaximum(),Data_hist->GetMaximum()) * 1.1);
        
        
        Data_hist->Draw("same");
        //Data_hist_2->Draw("same");
        
        TRatioPlot *rp = new TRatioPlot(Data_hist_2, Data_hist);
        //cancG1->SetTicks(0, 1);
        rp->Draw();
        rp->GetLowerRefGraph()->SetMinimum(0.8);
        rp->GetLowerRefGraph()->SetMaximum(1.2);
        rp->GetUpperRefYaxis()->SetRangeUser(0,std::max(Data_hist_2->GetMaximum(), Data_hist->GetMaximum()) * 1.1);
        rp->GetLowYaxis()->SetNdivisions(505);
        rp->GetUpperPad()->SetLeftMargin(0.14);
        rp->GetLowerPad()->SetLeftMargin(0.14);
        rp->GetUpperRefYaxis()->SetTitleOffset(1.8);
        rp->GetLowerRefYaxis()->SetTitleOffset(1.6);
        rp->GetUpperRefYaxis()->SetTitle("Counts");
        rp->GetLowerRefYaxis()->SetTitle("Ratio");
        
        rp->GetUpperPad()->cd();
        
        Data_hist_2->SetMarkerStyle(20);
        Data_hist_2->SetMarkerSize(3);
        Data_hist_2->Draw("PE same");
        
        int TP=Data_hist_2->GetEntries();
        
        if(i==0){
            auto legend = new TLegend(0.66, 0.88, 0.87, 0.67);
            //auto legend = new TLegend();
            legend->AddEntry(Data_hist, "Positives Sample", "l");
            legend->AddEntry(Data_hist_2,"True Positives", "p");
            legend->SetFillStyle(0);
            legend->SetLineWidth(0);
             legend->Draw("same ");
        }
        cout<<"Saving in "<<name_pdf<<".pdf"<<endl;
        cancG3->SaveAs(name_pdf +"_"+label1+ "_PSTP.pdf");
        cancG3->Clear();
        
        
       
        
    }
    

    
}

void Variable_Plots_TMVA_Article(TString infile,TString name_model,Float_t score_cut, TString outfile)
{
    gROOT->SetBatch(kTRUE);

    gStyle->SetPaintTextFormat("4.1f");
    gStyle->SetPalette(kLightTemperature);
    gStyle->SetOptStat(0);
    gStyle->SetLabelSize(.035, "xyz");
    gStyle->SetTitleSize(.042, "xyz");
    gStyle->SetTitleSize(.07, "t");
    gStyle->SetFrameLineWidth(1);
    gStyle->SetLineWidth(1);
    gStyle->SetHistLineWidth(1);
    gStyle->SetMarkerStyle(13);
    gStyle->SetTitleW(0.8);
    gStyle->SetTitleH(0.1);

    std::vector<vector<TString>> labels1D{
        {"P", "p", "custom_range", "0.", "14.",
            "custom_binning", "100", "", "legend", "P"},

        {"Theta", "#theta", "custom_range", "0.", "1",
            "custom_binning", "100", "", "legend", "electron_SF"},

        {"Phi", "#phi", "custom_range", "-3.2", "3.2",
            "custom_binning", "100", "", "legend", "electron_SF"},

        {"SFPCAL", "SF_{PCAL}", "custom_range", "0.", "0.3",
            "custom_binning", "100", "", "legend", "positron_SF_4"},

        {"SFECIN", "SF_{ECIN}", "custom_range", "0.", "0.25",
            "custom_binning", "100", "", "legend", "positron_SF_4"},

        {"SFECOUT", "SF_{ECOUT}", "custom_range", "0.", "0.25",
            "custom_binning", "100", "", "legend", "positron_SF_4"},

        {"m2PCAL", "m^{2}_{PCAL}", "custom_range", "0.", "200",
            "custom_binning", "100", "", "legend", "positron_SF_4"},

        {"m2ECIN", "m^{2}_{ECIN}", "custom_range", "0.", "500",
            "custom_binning", "100", "", "legend", "positron_SF_4"},

        {"m2ECOUT", "m^{2}_{ECOUT}", "custom_range", "0.", "500",
            "custom_binning", "100", "", "legend", "positron_SF_4"}
    };

    TFile *Data_file = new TFile(infile+".root");
    TTree *Data_tree = (TTree *)Data_file->Get("resP");
    TTree *Data_tree_1 = (TTree *)Data_file->Get("resN");
    cout<<"HERE1"<<endl;
    int entries_tree = Data_tree->GetEntries();
    int entries_tree_1 = Data_tree_1->GetEntries();
    cout<<"HERE2"<<endl;
    TCut cut_0 = "";
    TCut cut_1 = "";

    Float_t P_P, Theta_P, Phi_P,SFPCAL_P,SFECIN_P,SFECOUT_P, m2PCAL_P, m2ECIN_P, m2ECOUT_P;
    Float_t P_N, Theta_N, Phi_N,SFPCAL_N,SFECIN_N,SFECOUT_N, m2PCAL_N, m2ECIN_N, m2ECOUT_N;
    Float_t scoreBDT_P,scoreBDT_N,score,scoreMLP_P,scoreMLP_N;

    Data_tree->SetBranchAddress("scoreBDT_P",&scoreBDT_P);
    Data_tree->SetBranchAddress("scoreMLP_P",&scoreMLP_P);
    Data_tree->SetBranchAddress("P_P", &P_P);
    Data_tree->SetBranchAddress("Theta_P", &Theta_P);
    Data_tree->SetBranchAddress("Phi_P", &Phi_P);
    Data_tree->SetBranchAddress("SFPCAL_P", &SFPCAL_P);
    Data_tree->SetBranchAddress("SFECIN_P", &SFECIN_P);
    Data_tree->SetBranchAddress("SFECOUT_P", &SFECOUT_P);
    Data_tree->SetBranchAddress("m2PCAL_P", &m2PCAL_P);
    Data_tree->SetBranchAddress("m2ECIN_P", &m2ECIN_P);
    Data_tree->SetBranchAddress("m2ECOUT_P", &m2ECOUT_P);

    Data_tree_1->SetBranchAddress("scoreBDT_N",&scoreBDT_N);
    Data_tree_1->SetBranchAddress("scoreMLP_N",&scoreMLP_N);
    Data_tree_1->SetBranchAddress("P_N", &P_N);
    Data_tree_1->SetBranchAddress("Theta_N", &Theta_N);
    Data_tree_1->SetBranchAddress("Phi_N", &Phi_N);
    Data_tree_1->SetBranchAddress("SFPCAL_N", &SFPCAL_N);
    Data_tree_1->SetBranchAddress("SFECIN_N", &SFECIN_N);
    Data_tree_1->SetBranchAddress("SFECOUT_N", &SFECOUT_N);
    Data_tree_1->SetBranchAddress("m2PCAL_N", &m2PCAL_N);
    Data_tree_1->SetBranchAddress("m2ECIN_N", &m2ECIN_N);
    Data_tree_1->SetBranchAddress("m2ECOUT_N", &m2ECOUT_N);

    TString save_dir = "/Users/mariana/Work/Results/Article/";
    gSystem->mkdir(save_dir, kTRUE);
    TString name_pdf = save_dir + outfile + "_Plots";
    TString n1="_P";
    TString n2="_N";

    int PS=entries_tree;
    int NS=entries_tree_1;

    // --- True Positives vs True Negatives: one canvas per variable ---
    cout << "-------------------------------------------------" << endl;
    cout << "Plotting TP and TN (Article)" << endl;
    cout << "-------------------------------------------------" << endl;

    for (int i = 0; i < (int)labels1D.size(); i++){
        TString label1       = labels1D[i][0];
        TString xAxis_label  = labels1D[i][1];
        TString range_x_option  = labels1D[i][2];
        TString min_x_option    = labels1D[i][3];
        TString max_x_option    = labels1D[i][4];
        TString binning_x_option= labels1D[i][5];
        TString nb_x_bins       = labels1D[i][6];
        TString string_cut      = labels1D[i][7];

        cout << "Doing 1D " << label1 << " plot" << endl;

        TCut cut = string_cut.Data();

        float min_X_histo_ini = 0., max_X_histo_ini = 1.;
        int nBins_X = 20;
        if (range_x_option == "custom_range"){
            min_X_histo_ini = stof((string)min_x_option.Data());
            max_X_histo_ini = stof((string)max_x_option.Data());
        }
        if (binning_x_option == "custom_binning")
            nBins_X = stoi((string)nb_x_bins.Data());

        TH1F *Data_hist = new TH1F("Data_hist", "Data_hist", nBins_X, min_X_histo_ini, max_X_histo_ini);
        if(name_model=="MLP")
            Data_tree->Draw(label1+n1+">>Data_hist", Form(cut_1*cut&&"scoreMLP_P>=%g",score_cut));
        if(name_model=="BDT")
            Data_tree->Draw(label1+n1+">>Data_hist", Form(cut_1*cut&&"scoreBDT_P>=%g",score_cut));

        TH1F *Data_hist_2 = new TH1F("Data_hist_2", "Data_hist_2", nBins_X, min_X_histo_ini, max_X_histo_ini);
        if(name_model=="MLP")
            Data_tree_1->Draw(label1+n2+">>Data_hist_2", Form(cut_1*cut&&"scoreMLP_N<%g",score_cut));
        if(name_model=="BDT")
            Data_tree_1->Draw(label1+n2+">>Data_hist_2", Form(cut_1*cut&&"scoreBDT_N<%g",score_cut));

        Data_hist->Scale(1./Data_hist->Integral());
        Data_hist_2->Scale(1./Data_hist_2->Integral());

        Data_hist->SetLineColorAlpha(kCyan+2,0.35);
        Data_hist->SetMarkerColor(kCyan-3);
        Data_hist->SetFillColorAlpha(kCyan+2,0.35);
        Data_hist->SetMarkerStyle(20);
        Data_hist->SetFillStyle(1001);
        Data_hist->SetTitle(";"+xAxis_label+";Normalized counts");

        Data_hist_2->SetLineColor(kAzure-9);
        Data_hist_2->SetMarkerColor(kRed-7);
        Data_hist_2->SetFillStyle(1);
        Data_hist_2->SetFillColor(kRed-7);
        Data_hist_2->SetTitle(";"+xAxis_label+";Normalized counts");

        if(i==2)
            Data_hist->SetMaximum(std::max(Data_hist_2->GetMaximum(),Data_hist->GetMaximum()) * 1.5);
        else
            Data_hist->SetMaximum(std::max(Data_hist_2->GetMaximum(),Data_hist->GetMaximum()) * 1.2);

        TCanvas *cancTPosNeg = new TCanvas("cancTPosNeg","cancTPosNeg",800,600);
        cancTPosNeg->SetLeftMargin(0.14);
        cancTPosNeg->SetRightMargin(0.03);
        gStyle->SetLegendTextSize(0.03);
        Data_hist->GetXaxis()->SetTitleOffset(0.85);
        Data_hist->Draw("PL");
        Data_hist_2->SetMarkerStyle(20);
        Data_hist_2->Draw("PEL same");

        TLegend *legend;
        if(i >= 3)
            legend = new TLegend(0.62, 0.72, 0.88, 0.88);
        else
            legend = new TLegend();
        legend->AddEntry(Data_hist, "True Positives", "p");
        legend->AddEntry(Data_hist_2,"True Negatives", "p");
        legend->SetFillStyle(0);
        legend->SetLineWidth(0);
        legend->SetTextSize(0.04);
        legend->Draw("same");

        TString outname = name_pdf+"_TPosNeg_"+label1+".pdf";
        cout<<"Saving in "<<outname<<endl;
        cancTPosNeg->SaveAs(outname);

        gSystem->mkdir(save_dir+"Figure_5", kTRUE);
        TString outroot = outname;
        outroot.ReplaceAll(save_dir, save_dir+"Figure_5/");
        outroot.ReplaceAll(".pdf", ".root");
        TFile *fout = new TFile(outroot, "RECREATE");
        Data_hist->Write("TruePositives");
        Data_hist_2->Write("TrueNegatives");
        cancTPosNeg->Write("Canvas");
        fout->Close();
        delete fout;

        delete cancTPosNeg;
        delete Data_hist;
        delete Data_hist_2;
    }

    // --- True Positives vs False Positives: one canvas per variable ---
    cout << "-------------------------------------------------" << endl;
    cout << "Plotting TP and FP (Article)" << endl;
    cout << "-------------------------------------------------" << endl;

    for (int i = 0; i < (int)labels1D.size(); i++){
        TString label1       = labels1D[i][0];
        TString xAxis_label  = labels1D[i][1];
        TString range_x_option  = labels1D[i][2];
        TString min_x_option    = labels1D[i][3];
        TString max_x_option    = labels1D[i][4];
        TString binning_x_option= labels1D[i][5];
        TString nb_x_bins       = labels1D[i][6];
        TString string_cut      = labels1D[i][7];

        cout << "Doing 1D " << label1 << " plot" << endl;

        TCut cut = string_cut.Data();

        float min_X_histo_ini = 0., max_X_histo_ini = 1.;
        int nBins_X = 20;
        if (range_x_option == "custom_range"){
            min_X_histo_ini = stof((string)min_x_option.Data());
            max_X_histo_ini = stof((string)max_x_option.Data());
        }
        if (binning_x_option == "custom_binning")
            nBins_X = stoi((string)nb_x_bins.Data());

        TH1F *Data_hist_1 = new TH1F("Data_hist_1","Data_hist_1",nBins_X,min_X_histo_ini,max_X_histo_ini);
        if(name_model=="MLP")
            Data_tree->Draw(label1+n1+">>Data_hist_1", Form(cut_1*cut&&"scoreMLP_P>=%g",score_cut));
        if(name_model=="BDT")
            Data_tree->Draw(label1+n1+">>Data_hist_1", Form(cut_1*cut&&"scoreBDT_P>=%g",score_cut));

        TH1F *Data_hist_3 = new TH1F("Data_hist_3","Data_hist_3",nBins_X,min_X_histo_ini,max_X_histo_ini);
        if(name_model=="MLP")
            Data_tree_1->Draw(label1+n2+">>Data_hist_3", Form(cut_1*cut&&"scoreMLP_N>=%g",score_cut));
        if(name_model=="BDT")
            Data_tree_1->Draw(label1+n2+">>Data_hist_3", Form(cut_1*cut&&"scoreBDT_N>=%g",score_cut));

        Data_hist_1->Scale(1./Data_hist_1->Integral());
        Data_hist_3->Scale(1./Data_hist_3->Integral());

        Data_hist_1->SetMarkerStyle(20);
        Data_hist_1->SetLineColorAlpha(kCyan+2,0.35);
        Data_hist_1->SetMarkerColor(kCyan-3);
        Data_hist_1->SetTitle(";"+xAxis_label+";Normalized counts");

        Data_hist_3->SetLineColor(0);
        Data_hist_3->SetFillStyle(0);
        Data_hist_3->SetFillColor(0);
        Data_hist_3->SetMarkerColor(kOrange-3);
        Data_hist_3->SetTitle(";"+xAxis_label+";Normalized counts");

        Data_hist_1->SetMaximum(std::max(Data_hist_3->GetMaximum(),Data_hist_1->GetMaximum()) * 1.5);

        int TP=Data_hist_1->GetEntries();
        int FP=Data_hist_3->GetEntries();

        TCanvas *cancTPFP = new TCanvas("cancTPFP","cancTPFP",800,600);
        cancTPFP->SetTicks(0,1);
        TRatioPlot *rp = new TRatioPlot(Data_hist_1, Data_hist_3);
        rp->Draw();
        rp->GetLowerRefGraph()->SetMinimum(0);
        rp->GetLowerRefGraph()->SetMaximum(2);
        rp->GetLowYaxis()->SetNdivisions(505);
        rp->GetUpperPad()->cd();
        Data_hist_1->Draw("P same");
        Data_hist_3->SetMarkerStyle(20);
        Data_hist_3->Draw("PE same");

        auto legend2 = new TLegend(0.65, 0.87, 0.87, 0.67);
        legend2->AddEntry(Data_hist_1, Form("True Positives (%d)", TP), "p");
        legend2->AddEntry(Data_hist_3, Form("False Positives (%d)", FP), "p");
        legend2->SetFillStyle(0);
        legend2->SetLineWidth(0);
        legend2->Draw("same");

        TString outname = name_pdf+"_TPFP_"+label1+".pdf";
        cout<<"Saving in "<<outname<<endl;
        cancTPFP->SaveAs(outname);
        delete cancTPFP;
        delete Data_hist_1;
        delete Data_hist_3;
    }

    // --- Negative Sample vs False Positives: one canvas per variable ---
    cout << "-------------------------------------------------" << endl;
    cout << "Plotting NS and FP (Article)" << endl;
    cout << "-------------------------------------------------" << endl;
    gStyle->SetTitleSize(.04, "xyz");

    for (int i = 0; i < (int)labels1D.size(); i++){
        TString label1       = labels1D[i][0];
        TString xAxis_label  = labels1D[i][1];
        TString range_x_option  = labels1D[i][2];
        TString min_x_option    = labels1D[i][3];
        TString max_x_option    = labels1D[i][4];
        TString binning_x_option= labels1D[i][5];
        TString nb_x_bins       = labels1D[i][6];
        TString string_cut      = labels1D[i][7];

        cout << "Doing 1D " << label1 << " plot" << endl;

        TCut cut = string_cut.Data();

        float min_X_histo_ini = 0., max_X_histo_ini = 1.;
        int nBins_X = 20;
        if (range_x_option == "custom_range"){
            min_X_histo_ini = stof((string)min_x_option.Data());
            max_X_histo_ini = stof((string)max_x_option.Data());
        }
        if (binning_x_option == "custom_binning")
            nBins_X = stoi((string)nb_x_bins.Data());

        TH1F *Data_hist_1 = new TH1F("Data_hist_1","Data_hist_1",nBins_X,min_X_histo_ini,max_X_histo_ini);
        Data_tree_1->Draw(label1+n2+">>Data_hist_1", cut_1*cut);

        TH1F *Data_hist_3 = new TH1F("Data_hist_3","Data_hist_3",nBins_X,min_X_histo_ini,max_X_histo_ini);
        if(name_model=="MLP")
            Data_tree_1->Draw(label1+n2+">>Data_hist_3", Form(cut_1*cut&&"scoreMLP_N>=%g",score_cut));
        if(name_model=="BDT")
            Data_tree_1->Draw(label1+n2+">>Data_hist_3", Form(cut_1*cut&&"scoreBDT_N>=%g",score_cut));

        Data_hist_1->SetLineColor(kRed);
        Data_hist_1->SetLineWidth(2);
        Data_hist_1->SetFillColor(0);
        Data_hist_1->SetFillStyle(0);
        Data_hist_1->SetTitle(";"+xAxis_label+";Normalized counts");

        Data_hist_3->SetLineColor(0);
        Data_hist_3->SetFillStyle(0);
        Data_hist_3->SetFillColor(0);
        Data_hist_3->SetMarkerColor(kOrange-3);
        Data_hist_3->SetTitle(";"+xAxis_label+";Normalized counts");

        Data_hist_3->SetMaximum(std::max(Data_hist_3->GetMaximum(),Data_hist_1->GetMaximum()) * 1.2);

        int FP=Data_hist_3->GetEntries();

        TCanvas *cancNSFP = new TCanvas("cancNSFP","cancNSFP",800,600);
        cancNSFP->SetTicks(0,1);
        TRatioPlot *rp = new TRatioPlot(Data_hist_3, Data_hist_1);
        rp->Draw();
        rp->GetLowerRefGraph()->SetMinimum(0.0);
        rp->GetLowerRefGraph()->SetMaximum(0.2);
        rp->GetLowYaxis()->SetNdivisions(505);
        rp->GetUpperPad()->SetLeftMargin(0.14);
        rp->GetLowerPad()->SetLeftMargin(0.14);
        rp->GetUpperRefYaxis()->SetTitleOffset(1.8);
        rp->GetLowerRefYaxis()->SetTitleOffset(1.6);
        rp->GetUpperRefYaxis()->SetTitle("Normalized counts");
        rp->GetLowerRefYaxis()->SetTitle("Ratio");
        rp->GetUpperPad()->cd();
        Data_hist_1->Draw("same");
        Data_hist_3->SetMarkerStyle(20);
        Data_hist_3->SetMarkerSize(3);
        Data_hist_3->Draw("PE same");

        auto legend3 = new TLegend(0.66, 0.89, 0.87, 0.67);
        legend3->AddEntry(Data_hist_1,"Negative Sample","l");
        legend3->AddEntry(Data_hist_3,"False Positives","p");
        legend3->SetFillStyle(0);
        legend3->SetLineWidth(0);
        legend3->Draw("same");

        TString outname = name_pdf+"_"+label1+"_NSFP.pdf";
        cout<<"Saving in "<<outname<<endl;
        cancNSFP->SaveAs(outname);

        if (label1 == "P"){
            gSystem->mkdir(save_dir+"Figure_7", kTRUE);
            TString outroot = outname;
            outroot.ReplaceAll(save_dir, save_dir+"Figure_7/");
            outroot.ReplaceAll(".pdf", ".root");
            TFile *fout = new TFile(outroot, "RECREATE");
            Data_hist_1->Write("NegativeSample");
            Data_hist_3->Write("FalsePositives");
            cancNSFP->Write("Canvas");
            fout->Close();
            delete fout;
        }

        delete cancNSFP;
        delete Data_hist_1;
        delete Data_hist_3;
    }

    // --- Positive Sample vs True Positives: one canvas per variable ---
    cout << "-------------------------------------------------" << endl;
    cout << "Plotting Positive Sample and TP (Article)" << endl;
    cout << "-------------------------------------------------" << endl;

    for (int i = 0; i < (int)labels1D.size(); i++){
        TString label1       = labels1D[i][0];
        TString xAxis_label  = labels1D[i][1];
        TString range_x_option  = labels1D[i][2];
        TString min_x_option    = labels1D[i][3];
        TString max_x_option    = labels1D[i][4];
        TString binning_x_option= labels1D[i][5];
        TString nb_x_bins       = labels1D[i][6];
        TString string_cut      = labels1D[i][7];

        cout << "Doing 1D " << label1 << " plot" << endl;

        TCut cut = string_cut.Data();

        float min_X_histo_ini = 0., max_X_histo_ini = 1.;
        int nBins_X = 20;
        if (range_x_option == "custom_range"){
            min_X_histo_ini = stof((string)min_x_option.Data());
            max_X_histo_ini = stof((string)max_x_option.Data());
        }
        if (binning_x_option == "custom_binning")
            nBins_X = stoi((string)nb_x_bins.Data());

        TH1F *Data_hist = new TH1F("Data_hist","Data_hist",nBins_X,min_X_histo_ini,max_X_histo_ini);
        Data_tree->Draw(label1+n1+">>Data_hist", cut_0*cut);

        TH1F *Data_hist_2 = new TH1F("Data_hist_2","Data_hist_2",nBins_X,min_X_histo_ini,max_X_histo_ini);
        if(name_model=="MLP")
            Data_tree->Draw(label1+n1+">>Data_hist_2", Form(cut_1*cut&&"scoreMLP_P>=%g",score_cut));
        if(name_model=="BDT")
            Data_tree->Draw(label1+n1+">>Data_hist_2", Form(cut_1*cut&&"scoreBDT_P>=%g",score_cut));

        Data_hist->SetLineWidth(2);
        Data_hist->SetLineColor(kAzure-6);
        Data_hist->SetFillColor(0);
        Data_hist->SetFillStyle(0);
        Data_hist->SetTitle(";"+xAxis_label+";Normalized counts");

        Data_hist_2->SetMarkerColor(kCyan-3);
        Data_hist_2->SetFillStyle(0);
        Data_hist_2->SetFillColor(0);
        Data_hist_2->SetTitle(";"+xAxis_label+";Normalized counts");
        Data_hist->SetMaximum(std::max(Data_hist_2->GetMaximum(),Data_hist->GetMaximum()) * 1.1);

        int TP=Data_hist_2->GetEntries();

        TCanvas *cancPSTP = new TCanvas("cancPSTP","cancPSTP",800,600);
        TRatioPlot *rp = new TRatioPlot(Data_hist_2, Data_hist);
        rp->Draw();
        rp->GetLowerRefGraph()->SetMinimum(0.8);
        rp->GetLowerRefGraph()->SetMaximum(1.2);
        rp->GetUpperRefYaxis()->SetRangeUser(0,std::max(Data_hist_2->GetMaximum(),Data_hist->GetMaximum())*1.1);
        rp->GetLowYaxis()->SetNdivisions(505);
        rp->GetUpperPad()->SetLeftMargin(0.14);
        rp->GetLowerPad()->SetLeftMargin(0.14);
        rp->GetUpperRefYaxis()->SetTitleOffset(1.8);
        rp->GetLowerRefYaxis()->SetTitleOffset(1.6);
        rp->GetUpperRefYaxis()->SetTitle("Normalized counts");
        rp->GetLowerRefYaxis()->SetTitle("Ratio");
        rp->GetUpperPad()->cd();
        Data_hist_2->SetMarkerStyle(20);
        Data_hist_2->SetMarkerSize(3);
        Data_hist_2->Draw("PE same");

        auto legend4 = new TLegend(0.66, 0.88, 0.87, 0.67);
        legend4->AddEntry(Data_hist, "Positives Sample","l");
        legend4->AddEntry(Data_hist_2,"True Positives","p");
        legend4->SetFillStyle(0);
        legend4->SetLineWidth(0);
        legend4->Draw("same");

        TString outname = name_pdf+"_"+label1+"_PSTP.pdf";
        cout<<"Saving in "<<outname<<endl;
        cancPSTP->SaveAs(outname);

        if (label1 == "P"){
            gSystem->mkdir(save_dir+"Figure_7", kTRUE);
            TString outroot = outname;
            outroot.ReplaceAll(save_dir, save_dir+"Figure_7/");
            outroot.ReplaceAll(".pdf", ".root");
            TFile *fout = new TFile(outroot, "RECREATE");
            Data_hist->Write("PositivesSample");
            Data_hist_2->Write("TruePositives");
            cancPSTP->Write("Canvas");
            fout->Close();
            delete fout;
        }

        delete cancPSTP;
        delete Data_hist;
        delete Data_hist_2;
    }
}

void Variable_Plots_DataMC(TString infile_MC,TString infile_signal,TString infile_background,TString name_model,Float_t score_cut, TString outfile)
{


    gROOT->SetBatch(kTRUE);
    
    gStyle->SetPaintTextFormat("4.1f");
    gStyle->SetPalette(kLightTemperature);
    gStyle->SetOptStat(0);
    gStyle->SetLabelSize(.04, "xyz");
    gStyle->SetTitleSize(.048, "xyz");
    gStyle->SetTitleSize(.07, "t");
    gStyle->SetFrameLineWidth(1);
    gStyle->SetLineWidth(1);
    gStyle->SetHistLineWidth(1);
    gStyle->SetMarkerStyle(13);
    gStyle->SetTitleW(0.8); // per cent of the pad width
    gStyle->SetTitleH(0.1); // per cent of the pad height
    
    
    std::vector<vector<TString>> labels_sig{
        {"P", "P", "custom_range", "0.", "14.",
            "custom_binning", "100", "", "legend", "P"},
        
        {"Theta", "#theta", "custom_range", "0.", "0.5",
            "custom_binning", "100", "", "legend", "electron_SF"},
        
        {"Phi", "#phi", "custom_range", "-3.2", "3.2",
            "custom_binning", "100", "", "legend", "electron_SF"},
        
        {"SFPCAL", "SF PCAL", "custom_range", "0.", "0.3",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"SFECIN", "SF ECIN", "custom_range", "0.", "0.25",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"SFECOUT", "SF ECOUT", "custom_range", "0.", "0.1",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"m2PCAL", "m2 PCAL", "custom_range", "0.", "80",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"m2ECIN", "m2 ECIN", "custom_range", "0.", "350",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"m2ECOUT", "m2 ECOUT", "custom_range", "0.", "200",
            "custom_binning", "100", "", "legend", "positron_SF_4"}
    };

    std::vector<vector<TString>> labels_bkg{
        {"P", "P", "custom_range", "0.", "14.",
            "custom_binning", "100", "", "legend", "P"},
        
        {"Theta", "#theta", "custom_range", "0.", "1",
            "custom_binning", "100", "", "legend", "electron_SF"},
        
        {"Phi", "#phi", "custom_range", "-3.2", "3.2",
            "custom_binning", "100", "", "legend", "electron_SF"},
        
        {"SFPCAL", "SF PCAL", "custom_range", "0.", "0.3",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"SFECIN", "SF ECIN", "custom_range", "0.", "0.25",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"SFECOUT", "SF ECOUT", "custom_range", "0.", "0.25",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"m2PCAL", "m2 PCAL", "custom_range", "0.", "200",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"m2ECIN", "m2 ECIN", "custom_range", "0.", "500",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"m2ECOUT", "m2 ECOUT", "custom_range", "0.", "500",
            "custom_binning", "100", "", "legend", "positron_SF_4"}
    };
    
    TFile *Data_file_MC = new TFile(infile_MC+".root");
    TTree *Data_tree_MC_resP = (TTree *)Data_file_MC->Get("resP");
    //TTree *Data_tree_MC_resN = (TTree *)Data_file_MC->Get("resN");


    int entries_tree_MC_resP = Data_tree_MC_resP->GetEntries();
    //int entries_tree_MC_resN = Data_tree_MC_resN->GetEntries();

    
    TCut cut_0 = "";
    TCut cut_1 = "";
    
    //For MC
    Float_t P, Theta, Phi,SFPCAL,SFECIN,SFECOUT, m2PCAL, m2ECIN, m2ECOUT;
    Float_t P_N, Theta_N, Phi_N,SFPCAL_N,SFECIN_N,SFECOUT_N, m2PCAL_N, m2ECIN_N, m2ECOUT_N;
    Float_t P_P, Theta_P, Phi_P,SFPCAL_P,SFECIN_P,SFECOUT_P, m2PCAL_P, m2ECIN_P, m2ECOUT_P;
    Double_t P_D, Theta_D, Phi_D,SFPCAL_D,SFECIN_D,SFECOUT_D, m2PCAL_D, m2ECIN_D, m2ECOUT_D;
    Float_t scoreBDT_P,scoreBDT_N,score,scoreMLP_P,scoreMLP_N;
    Double_t scoreBDT_D;
    cout<<"Reading MC"<<endl;
    
    //---------------Tree 1-------------------------------
    //TMVAPos.root, has true positives and false negatives
    //--------------------------------------------------
    //Data: #positives, #True positives, #False Negatives #score
    Data_tree_MC_resP->SetBranchAddress("scoreBDT_P",&scoreBDT_P);
    //Data_tree_MC_resP->SetBranchAddress("scoreMLP_P",&scoreMLP_P);
    //positives sample variables
    Data_tree_MC_resP->SetBranchAddress("P_P", &P_P);
    Data_tree_MC_resP->SetBranchAddress("Theta_P", &Theta_P);
    Data_tree_MC_resP->SetBranchAddress("Phi_P", &Phi_P);
    Data_tree_MC_resP->SetBranchAddress("SFPCAL_P", &SFPCAL_P);
    Data_tree_MC_resP->SetBranchAddress("SFECIN_P", &SFECIN_P);
    Data_tree_MC_resP->SetBranchAddress("SFECOUT_P", &SFECOUT_P);
    Data_tree_MC_resP->SetBranchAddress("m2PCAL_P", &m2PCAL_P);
    Data_tree_MC_resP->SetBranchAddress("m2ECIN_P", &m2ECIN_P);
    Data_tree_MC_resP->SetBranchAddress("m2ECOUT_P", &m2ECOUT_P);
    
    
    //---------------Tree 1-------------------------------
    //TMVAPos.root, has true positives and false negatives
    //--------------------------------------------------
    //Data: #negatives, #True negatives, #False positives #score
    /*Data_tree_MC_resN->SetBranchAddress("scoreBDT_N",&scoreBDT_N);
    Data_tree_MC_resN->SetBranchAddress("scoreMLP_N",&scoreMLP_N);
    //negatives sample variables
    Data_tree_MC_resN->SetBranchAddress("P_N", &P_N);
    Data_tree_MC_resN->SetBranchAddress("Theta_N", &Theta_N);
    Data_tree_MC_resN->SetBranchAddress("Phi_N", &Phi_N);
    Data_tree_MC_resN->SetBranchAddress("SFPCAL_N", &SFPCAL_N);
    Data_tree_MC_resN->SetBranchAddress("SFECIN_N", &SFECIN_N);
    Data_tree_MC_resN->SetBranchAddress("SFECOUT_N", &SFECOUT_N);
    Data_tree_MC_resN->SetBranchAddress("m2PCAL_N", &m2PCAL_N);
    Data_tree_MC_resN->SetBranchAddress("m2ECIN_N", &m2ECIN_N);
    Data_tree_MC_resN->SetBranchAddress("m2ECOUT_N", &m2ECOUT_N);*/

    cout<<"Reading: "<<infile_signal<<endl;

    TFile *Data_file_signal = new TFile(infile_signal+".root");
    TTree *Data_tree_signal = (TTree *)Data_file_signal->Get("results");

    Data_tree_signal->SetBranchAddress("scoreBDT_D",&scoreBDT_D);
    //negatives sample variables
    Data_tree_signal->SetBranchAddress("P_D", &P_D);
    Data_tree_signal->SetBranchAddress("Theta_D", &Theta_D);
    Data_tree_signal->SetBranchAddress("Phi_D", &Phi_D);
    Data_tree_signal->SetBranchAddress("SFPCAL_D", &SFPCAL_D);
    Data_tree_signal->SetBranchAddress("SFECIN_D", &SFECIN_D);
    Data_tree_signal->SetBranchAddress("SFECOUT_D", &SFECOUT_D);
    Data_tree_signal->SetBranchAddress("m2PCAL_D", &m2PCAL_D);
    Data_tree_signal->SetBranchAddress("m2ECIN_D", &m2ECIN_D);
    Data_tree_signal->SetBranchAddress("m2ECOUT_D", &m2ECOUT_D);
    cout<<"Branches set"<<endl;

    //TFile *Data_file_background = new TFile(infile_background+".root");


    //TTree *Data_tree_background = (TTree *)Data_file_background->Get("results");


    
    TString Data_names[9]={"P","theta","phi","SFPCAL","SFECIN","SFECOUT","m2PCAL","m2ECIN","m2ECOUT"};


    TString n1="_P";
    TString n2="_N";
    TString d1="_D";

    int PS=entries_tree_MC_resP;
    //int NS=entries_tree_MC_resN;
    int PP=0;
    int PN=0;
    
    TCanvas *cancG0 = new TCanvas("cancG0", "cancG0", 1500, 1200);
    cancG0->Divide(3,3,0.01,0.01,0);
    
    TCanvas *cancG1 = new TCanvas("cancG1", "cancG1", 1500, 1200);
    cancG1->Divide(3,3,0.01,0.01,0);
    
    TCanvas *cancG2 = new TCanvas("cancG2", "cancG2", 1500, 1200);
    cancG2->Divide(4,3,0.01,0.01,0);
    
    TCanvas *cancG3 = new TCanvas("cancG3", "cancG3", 1500, 1200);
    cancG3->Divide(3,3,0.01,0.01,0);
    
    gStyle->SetLabelSize(0.04, "x");
    
    cout << "-------------------------------------------------" << endl;
    cout << "Plotting DatavsMC" << endl;
    cout << "-------------------------------------------------" << endl;


     //Positive MC Sample
    TH2F *MC_pSF = new TH2F("MC_pSF", "MC_pSF", 100, 0.0, 0.2,100,4.5,8.0);
    TH2F *MC_pm2  = new TH2F("MC_pm2", "MC_pm2", 100,0.0,200.0,100,4.5,8.0);
    TH2F *Data_pSF = new TH2F("Data_pSF", "Data_pSF", 100, 0.0, 0.2,100,4.5,8.0);
    TH2F *Data_pm2  = new TH2F("Data_pm2", "Data_pm2", 100,0.0,200.0,100,4.5,8.0);

    TH2F *MC_thetaSF = new TH2F("MC_thetaSF", "MC_thetaSF", 100, 0.0, 0.2,100,0.08,0.30);//0.08,0.15
    TH2F *MC_thetam2  = new TH2F("MC_thetam2", "MC_thetam2", 100,0.0,200.0,100,0.08,0.30);
    TH2F *Data_thetaSF = new TH2F("Data_thetaSF", "Data_thetaSF", 100, 0.0, 0.2,100,0.08,0.30);
    TH2F *Data_thetam2  = new TH2F("Data_thetam2", "Data_thetam2", 100,0.0,200.0,100,0.08,0.30);



    TH2F *MC_thetap = new TH2F("MC_thetap", "MC_thetap", 100, 0.08,0.30,100,4.5,8.0);
   
    TH2F *Data_thetap= new TH2F("Data_thetap", "Data_thetap", 100, 0.08,0.30,100,4.5,8.0);




    for (int t_i = 0; t_i < Data_tree_MC_resP->GetEntries(); t_i++){
        Data_tree_MC_resP->GetEntry(t_i);
        
        MC_pSF->Fill(SFECIN_P,P_P);
        MC_pm2->Fill(m2ECIN_P,P_P);
        MC_thetaSF->Fill(SFECIN_P,Theta_P);
        MC_thetam2->Fill(m2ECIN_P,Theta_P);
        MC_thetap->Fill(Theta_P,P_P);
        

    }
    for (int t_i = 0; t_i < Data_tree_signal->GetEntries(); t_i++){
        Data_tree_signal->GetEntry(t_i);

        Data_pSF->Fill(SFECIN_D,P_D);
        Data_pm2->Fill(m2ECIN_D,P_D);
        Data_thetaSF->Fill(SFECIN_D,Theta_D);
        Data_thetam2->Fill(m2ECIN_D,Theta_D);
        Data_thetap->Fill(Theta_D,P_D);


    }
        
    //gStyle->SetOptStat(1);
    cancG2->cd(1);
    MC_pSF->Draw("colz");
     cancG2->cd(2);
    MC_pm2->Draw("colz");
     cancG2->cd(3);
    Data_pSF->Draw("colz");
     cancG2->cd(4);
    Data_pm2->Draw("colz");

    cancG2->cd(5);
    MC_thetaSF->Draw("colz");
     cancG2->cd(6);
    MC_thetam2->Draw("colz");
     cancG2->cd(7);
    Data_thetaSF->Draw("colz");
     cancG2->cd(8);
    Data_thetam2->Draw("colz");


    cancG2->cd(9);
    MC_thetap->Draw("colz");
     cancG2->cd(10);
    Data_thetap->Draw("colz");
 
    
    cout<<"Saving in "<<outfile<<".pdf"<<endl;
    cancG2->SaveAs("./Results/"+outfile + ".pdf");

    cout << "-------------------------------------------------" << endl;
    cout << "Plotting Signal" << endl;
    cout << "-------------------------------------------------" << endl;
    
    for (int i = 0; i < labels_sig.size(); i++){
        int PP=0;
        int PN=0;
        cancG0->cd(i+1);
        //////////////////////////////////////////////////
        //////////////////////////////////////////////////
        // Set options for each label
        TString label1 = labels_sig[i][0];
        TString xAxis_label = labels_sig[i][1];
        TString range_x_option = labels_sig[i][2];
        TString min_x_option = labels_sig[i][3];
        TString max_x_option = labels_sig[i][4];
        TString binning_x_option = labels_sig[i][5];
        TString nb_x_bins = labels_sig[i][6];
        TString string_cut = labels_sig[i][7];
        TString legend_option = labels_sig[i][8];
        TString output_string = labels_sig[i][9];
        //////////////////////////////////////////////////
        
        cout << "Doing 1D " << label1 << " plot" << endl;
        
        
        TCut cut = string_cut.Data();
        
        TString cut_string = cut.GetTitle();
        
        float min_X_histo_ini;
        float max_X_histo_ini;
        
        int nBins_X = 20;
        //Extract custom range options
        if (range_x_option == "custom_range"){
            //cout << "Reading range " << endl;
            min_X_histo_ini = stof((string)min_x_option.Data());
            max_X_histo_ini = stof((string)max_x_option.Data());
        }
        
        //Extract custom bins options
        if (binning_x_option == "custom_binning"){
            //cout << "Reading # of data bins: " << nb_x_bins.Data() << endl;
            nBins_X = stoi((string)nb_x_bins.Data());
        }
        //////////////////////////////////////////////////

        //Positive MC Sample
        TH1F *MC_hist = new TH1F("MC_hist", "MC_hist", nBins_X, min_X_histo_ini, max_X_histo_ini);
        TH1F *Data_hist  = new TH1F("Data_hist", "Data_hist", nBins_X, min_X_histo_ini, max_X_histo_ini);
        TH1F *Data_cut_hist  = new TH1F("Data_cut_hist", "Data_cut_hist", nBins_X, min_X_histo_ini, max_X_histo_ini);
        //*0.02 for F18out pos
        //*0.80 for S19 pos and F18in pos
        //*1 for F18in ele


        Data_tree_MC_resP->Draw(label1+"_P>>MC_hist", Form(cut_1 * cut&&"P_P>=%g"&&"P_P<%g"&&"Theta_P>%g"&&"Theta_P<=%g",4.5,8.0,0.05,0.15),"",Data_tree_MC_resP->GetEntries());
        //Data_tree_MC_resP->Draw(label1+">>MC_hist",cut_1 * cut,"");
        
        TString corr_SF,corr_m2;
        corr_SF="+0.00";
        corr_m2="*1.0";
        //corr_m2="+0.0";

        //F18in corr_SF="+0.02"; corr_m2="-21.0";
        //F18in corr_SF="+0.05"; corr_m2="+20.0";

        if(label1=="SFECIN")
            Data_tree_signal->Draw(label1 +d1+corr_SF+">>Data_hist","","",Data_tree_MC_resP->GetEntries()*1);
        else if(label1=="m2ECIN")
            Data_tree_signal->Draw(label1 +d1+corr_m2+">>Data_hist","","",Data_tree_MC_resP->GetEntries()*1);
        else
        Data_tree_signal->Draw(label1 +d1+">>Data_hist","","",Data_tree_MC_resP->GetEntries()*1);

        if(label1=="SFECIN")
             Data_tree_signal->Draw(label1 +d1+corr_SF+">>Data_cut_hist", Form(cut_1 * cut&&"scoreBDT_D>=%g",0.0),"",Data_tree_MC_resP->GetEntries()*0.2);
        else if(label1=="m2ECIN")
             Data_tree_signal->Draw(label1 +d1+corr_m2+">>Data_cut_hist", Form(cut_1 * cut&&"scoreBDT_D>=%g",0.0),"",Data_tree_MC_resP->GetEntries()*0.2);
        else
        Data_tree_signal->Draw(label1 +d1+">>Data_cut_hist", Form(cut_1 * cut&&"scoreBDT_D>=%g",0.0),"",Data_tree_MC_resP->GetEntries()*0.2);
       

         /*bool sameBinSize = (Data_cut_hist->GetXaxis()->GetBinWidth(1) == MC_hist->GetXaxis()->GetBinWidth(1));

        if (sameBinSize) {
            std::cout << "The histograms have the same bin size." << std::endl;
        } else {
            std::cout << "The histograms do not have the same bin size." << std::endl;
        }



        


        MC_hist->SetBins(Data_cut_hist->GetNbinsX(), Data_cut_hist->GetXaxis()->GetXmin(), Data_cut_hist->GetXaxis()->GetXmax());

        
        bool sameBinSize = (Data_cut_hist->GetXaxis()->GetBinWidth(1) == MC_hist->GetXaxis()->GetBinWidth(1));

        if (sameBinSize) {
            std::cout << "The histograms have the same bin size." << std::endl;
        } else {
            std::cout << "The histograms do not have the same bin size." << std::endl;
        }*/
        MC_hist->Scale(1./MC_hist->Integral());
        Data_hist->Scale(1./Data_hist->Integral());
        Data_cut_hist->Scale(1./Data_cut_hist->Integral());

        MC_hist->Sumw2(kFALSE);
        Data_hist->Sumw2(kFALSE);
        Data_cut_hist->Sumw2(kFALSE);

        MC_hist->SetLineColorAlpha(kCyan-5,0.35);
        //MC_hist->SetMarkerColor(kCyan-5);
        MC_hist->SetFillColorAlpha(kCyan-5,0.35);
        MC_hist->SetMarkerStyle(20);
        MC_hist->SetFillStyle(1001);
        MC_hist->SetTitle(";" + xAxis_label+";");
        
        
        Data_hist->SetLineColor(kGreen+3);
        Data_hist->SetLineWidth(1);
        Data_hist->SetLineStyle(1);
        Data_hist->SetFillColor(0);
        Data_hist->SetTitle(";" + xAxis_label+";");

        Data_cut_hist->SetLineColor(kGreen+2);
        Data_cut_hist->SetLineStyle(1);
        Data_cut_hist->SetLineWidth(1);
        Data_cut_hist->SetFillColorAlpha(kGreen-10,0.20);
        Data_cut_hist->SetTitle(";" + xAxis_label+";");


        
        if(i==2)
            MC_hist->SetMaximum(std::max(Data_hist->GetMaximum(),MC_hist->GetMaximum()) * 1.5);
        else
            MC_hist->SetMaximum(std::max(Data_hist->GetMaximum(),MC_hist->GetMaximum()) * 1.2);
        
        
        
        MC_hist->Draw();
        
        /*TRatioPlot *rp = new TRatioPlot(MC_hist, Data_cut_hist);
        //cancG1->SetTicks(0, 1);
        rp->Draw();
        rp->GetLowerRefGraph()->SetMinimum(0);
        rp->GetLowerRefGraph()->SetMaximum(2);
        rp->GetLowYaxis()->SetTitle("ratio MC/Data");
        rp->GetLowYaxis()->SetNdivisions(505);
        
        rp->GetUpperPad()->cd();*/

        //MC_hist->Draw("same");
        Data_hist->Draw("same");
        Data_cut_hist->Draw("same");

        gStyle->SetLegendTextSize(0.05);

        
        if(i==1){
            TPaveLabel *t = new TPaveLabel(0.3, 0.92, 0.6, 1,outfile, "brNDC"); // left-up
            t->Draw();
            auto legend = new TLegend(0.50, 0.87, 0.87, 0.67);
            //auto legend = new TLegend();
            legend->AddEntry(MC_hist, "MC Signal", "f");
            legend->AddEntry(Data_hist,"Data", "l");
            legend->AddEntry(Data_cut_hist,"Data after 0.0 cut", "f");
            legend->SetFillStyle(0);
            legend->SetLineWidth(0);
            legend->Draw("same ");
        }
        
    }
    cout<<"Saving in "<<outfile<<".pdf"<<endl;
    cancG0->SaveAs("./"+outfile + ".pdf");
    /*
    
    cout << "-------------------------------------------------" << endl;
    cout << "Plotting Background" << endl;
    cout << "-------------------------------------------------" << endl;
    
    for (int i = 0; i < labels_bkg.size(); i++){
        int PP=0;
        int PN=0;
        cancG1->cd(i+1);
        
        //////////////////////////////////////////////////
        //////////////////////////////////////////////////
        // Set options for each label
        TString label1 = labels_bkg[i][0];
        TString xAxis_label = labels_bkg[i][1];
        TString range_x_option = labels_bkg[i][2];
        TString min_x_option = labels_bkg[i][3];
        TString max_x_option = labels_bkg[i][4];
        TString binning_x_option = labels_bkg[i][5];
        TString nb_x_bins = labels_bkg[i][6];
        TString string_cut = labels_bkg[i][7];
        TString legend_option = labels_bkg[i][8];
        TString output_string = labels_bkg[i][9];
        //////////////////////////////////////////////////
        
        cout << "Doing 1D " << label1 << " plot" << endl;
        
        
        TCut cut = string_cut.Data();
        
        TString cut_string = cut.GetTitle();
        
        float min_X_histo_ini;
        float max_X_histo_ini;
        
        int nBins_X = 100;
        //Extract custom range options
        if (range_x_option == "custom_range"){
            //cout << "Reading range " << endl;
            min_X_histo_ini = stof((string)min_x_option.Data());
            max_X_histo_ini = stof((string)max_x_option.Data());
        }
        
        //Extract custom bins options
        if (binning_x_option == "custom_binning"){
            //cout << "Reading # of data bins: " << nb_x_bins.Data() << endl;
            nBins_X = stoi((string)nb_x_bins.Data());
        }
        //////////////////////////////////////////////////

        //Positive MC Sample

        TH1F *MC_hist = new TH1F("MC_hist", "MC_hist", nBins_X, min_X_histo_ini, max_X_histo_ini);
        TH1F *Data_hist  = new TH1F("Data_hist", "Data_hist", nBins_X, min_X_histo_ini, max_X_histo_ini);
        TH1F *Data_cut_hist  = new TH1F("Data_cut_hist", "Data_cut_hist", nBins_X, min_X_histo_ini, max_X_histo_ini);

        

        Data_hist=(TH1F*)Data_file_background->Get("h_e_"+Data_names[i]);
        Data_cut_hist=(TH1F*)Data_file_background->Get("h_e_"+Data_names[i]+"_cut");

        Data_tree_MC_resN->Draw(label1 +n2+">>MC_hist", Form(cut_1 * cut&&"P_N<=%g"&&"Theta_N<=%g",8.5,0.4));


        //MC_hist->SetBins(Data_cut_hist->GetNbinsX(), Data_cut_hist->GetXaxis()->GetXmin(), Data_cut_hist->GetXaxis()->GetXmax());

        
        bool sameBinSize = (Data_cut_hist->GetXaxis()->GetBinWidth(1) == MC_hist->GetXaxis()->GetBinWidth(1));

        if (sameBinSize) {
            std::cout << "The histograms have the same bin size." << std::endl;
        } else {
            std::cout << "The histograms do not have the same bin size." << std::endl;
        }

        //MC_hist->Scale(1./MC_hist->Integral());
        //Data_hist->Scale(1./Data_hist->Integral());
        //Data_cut_hist->Scale(1./Data_cut_hist->Integral());
        
        MC_hist->Sumw2(kFALSE);
        Data_hist->Sumw2(kFALSE);
        Data_cut_hist->Sumw2(kFALSE);

        MC_hist->SetLineColorAlpha(kCyan-5,0.35);
        MC_hist->SetMarkerColor(kCyan-5);
        MC_hist->SetFillColorAlpha(kCyan-5,0.35);
        MC_hist->SetMarkerStyle(20);
        MC_hist->SetFillStyle(1001);
        MC_hist->SetTitle(";" + xAxis_label+";");
        
        
        Data_hist->SetLineColor(kOrange+10);
        Data_hist->SetLineWidth(1);
        Data_hist->SetLineStyle(1);
        Data_hist->SetFillColor(0);
        Data_cut_hist->SetTitle(";" + xAxis_label+";");

        Data_cut_hist->SetLineColor(kOrange);
        Data_cut_hist->SetLineWidth(1);
        Data_cut_hist->SetLineStyle(1);
        Data_cut_hist->SetFillColorAlpha(kOrange,0.25);


        
        
        if(i==2)
            MC_hist->SetMaximum(std::max(Data_hist->GetMaximum(),MC_hist->GetMaximum()) * 1.5);
        else
            MC_hist->SetMaximum(std::max(Data_hist->GetMaximum(),MC_hist->GetMaximum()) * 1.2);
        
        
        
        
        
       MC_hist->Draw();
        

        //MC_hist->Draw("same");
        Data_hist->Draw("same");
        Data_cut_hist->Draw("same");
        

        gStyle->SetLegendTextSize(0.05);
        
        if(i==1){
            TPaveLabel *t = new TPaveLabel(0.3, 0.92, 0.6, 1,outfile, "brNDC"); // left-up
            t->Draw();
            auto legend = new TLegend(0.50, 0.87, 0.87, 0.67);
            //auto legend = new TLegend();
            legend->AddEntry(MC_hist, "MC Bkg", "f");
            legend->AddEntry(Data_hist,"Data", "l");
            legend->AddEntry(Data_cut_hist,"Data after 0.0 cut", "f");
            legend->SetFillStyle(0);
            legend->SetLineWidth(0);
            legend->Draw("same ");
        }
        
    }
    cout<<"Saving in "<<outfile<<".pdf"<<endl;
    cancG1->SaveAs("./Results/"+outfile + ".pdf)");*/

}

void Variable_Plots_DataMC_Article(TString infile_MC,TString infile_signal,TString infile_background,TString name_model,Float_t score_cut, TString outfile)
{
    gROOT->SetBatch(kTRUE);

    gStyle->SetPaintTextFormat("4.1f");
    gStyle->SetPalette(kLightTemperature);
    gStyle->SetOptStat(0);
    gStyle->SetLabelSize(.04, "xyz");
    gStyle->SetTitleSize(.048, "xyz");
    gStyle->SetTitleSize(.07, "t");
    gStyle->SetFrameLineWidth(1);
    gStyle->SetLineWidth(1);
    gStyle->SetHistLineWidth(1);
    gStyle->SetMarkerStyle(13);
    gStyle->SetTitleW(0.8);
    gStyle->SetTitleH(0.1);

    std::vector<vector<TString>> labels_sig{
        {"P", "p", "custom_range", "0.", "14.",
            "custom_binning", "100", "", "legend", "P"},

        {"Theta", "#theta", "custom_range", "0.", "0.5",
            "custom_binning", "100", "", "legend", "electron_SF"},

        {"Phi", "#phi", "custom_range", "-3.2", "3.2",
            "custom_binning", "100", "", "legend", "electron_SF"},

        {"SFPCAL", "SF_{PCAL}", "custom_range", "0.", "0.3",
            "custom_binning", "100", "", "legend", "positron_SF_4"},

        {"SFECIN", "SF_{ECIN}", "custom_range", "0.", "0.25",
            "custom_binning", "100", "", "legend", "positron_SF_4"},

        {"SFECOUT", "SF_{ECOUT}", "custom_range", "0.", "0.1",
            "custom_binning", "100", "", "legend", "positron_SF_4"},

        {"m2PCAL", "m^{2}_{PCAL}", "custom_range", "0.", "80",
            "custom_binning", "100", "", "legend", "positron_SF_4"},

        {"m2ECIN", "m^{2}_{ECIN}", "custom_range", "0.", "350",
            "custom_binning", "100", "", "legend", "positron_SF_4"},

        {"m2ECOUT", "m^{2}_{ECOUT}", "custom_range", "0.", "200",
            "custom_binning", "100", "", "legend", "positron_SF_4"}
    };

    TFile *Data_file_MC = new TFile(infile_MC+".root");
    TTree *Data_tree_MC_resP = (TTree *)Data_file_MC->Get("resP");

    int entries_tree_MC_resP = Data_tree_MC_resP->GetEntries();

    TCut cut_0 = "";
    TCut cut_1 = "";

    Float_t P, Theta, Phi,SFPCAL,SFECIN,SFECOUT, m2PCAL, m2ECIN, m2ECOUT;
    Float_t P_N, Theta_N, Phi_N,SFPCAL_N,SFECIN_N,SFECOUT_N, m2PCAL_N, m2ECIN_N, m2ECOUT_N;
    Float_t P_P, Theta_P, Phi_P,SFPCAL_P,SFECIN_P,SFECOUT_P, m2PCAL_P, m2ECIN_P, m2ECOUT_P;
    Double_t P_D, Theta_D, Phi_D,SFPCAL_D,SFECIN_D,SFECOUT_D, m2PCAL_D, m2ECIN_D, m2ECOUT_D;
    Float_t scoreBDT_P,scoreBDT_N,score,scoreMLP_P,scoreMLP_N;
    Double_t scoreBDT_D;
    cout<<"Reading MC"<<endl;

    Data_tree_MC_resP->SetBranchAddress("scoreBDT_P",&scoreBDT_P);
    Data_tree_MC_resP->SetBranchAddress("P_P", &P_P);
    Data_tree_MC_resP->SetBranchAddress("Theta_P", &Theta_P);
    Data_tree_MC_resP->SetBranchAddress("Phi_P", &Phi_P);
    Data_tree_MC_resP->SetBranchAddress("SFPCAL_P", &SFPCAL_P);
    Data_tree_MC_resP->SetBranchAddress("SFECIN_P", &SFECIN_P);
    Data_tree_MC_resP->SetBranchAddress("SFECOUT_P", &SFECOUT_P);
    Data_tree_MC_resP->SetBranchAddress("m2PCAL_P", &m2PCAL_P);
    Data_tree_MC_resP->SetBranchAddress("m2ECIN_P", &m2ECIN_P);
    Data_tree_MC_resP->SetBranchAddress("m2ECOUT_P", &m2ECOUT_P);

    cout<<"Reading: "<<infile_signal<<endl;

    TFile *Data_file_signal = new TFile(infile_signal+".root");
    TTree *Data_tree_signal = (TTree *)Data_file_signal->Get("results");

    Data_tree_signal->SetBranchAddress("scoreBDT_D",&scoreBDT_D);
    Data_tree_signal->SetBranchAddress("P_D", &P_D);
    Data_tree_signal->SetBranchAddress("Theta_D", &Theta_D);
    Data_tree_signal->SetBranchAddress("Phi_D", &Phi_D);
    Data_tree_signal->SetBranchAddress("SFPCAL_D", &SFPCAL_D);
    Data_tree_signal->SetBranchAddress("SFECIN_D", &SFECIN_D);
    Data_tree_signal->SetBranchAddress("SFECOUT_D", &SFECOUT_D);
    Data_tree_signal->SetBranchAddress("m2PCAL_D", &m2PCAL_D);
    Data_tree_signal->SetBranchAddress("m2ECIN_D", &m2ECIN_D);
    Data_tree_signal->SetBranchAddress("m2ECOUT_D", &m2ECOUT_D);
    cout<<"Branches set"<<endl;

    TString save_dir = "/Users/mariana/Work/Results/Article/";
    gSystem->mkdir(save_dir, kTRUE);
    TString name_pdf = save_dir + outfile;
    TString n1="_P";
    TString n2="_N";
    TString d1="_D";

    int PS=entries_tree_MC_resP;

    // --- 2D correlation plots: one canvas per histogram ---
    cout << "-------------------------------------------------" << endl;
    cout << "Plotting 2D correlations (Article)" << endl;
    cout << "-------------------------------------------------" << endl;

    TH2F *MC_pSF      = new TH2F("MC_pSF",      "MC_pSF",      100, 0.0,0.2,  100, 4.5,8.0);
    TH2F *MC_pm2      = new TH2F("MC_pm2",       "MC_pm2",      100, 0.0,200., 100, 4.5,8.0);
    TH2F *Data_pSF    = new TH2F("Data_pSF",     "Data_pSF",    100, 0.0,0.2,  100, 4.5,8.0);
    TH2F *Data_pm2    = new TH2F("Data_pm2",     "Data_pm2",    100, 0.0,200., 100, 4.5,8.0);
    TH2F *MC_thetaSF  = new TH2F("MC_thetaSF",   "MC_thetaSF",  100, 0.0,0.2,  100, 0.08,0.30);
    TH2F *MC_thetam2  = new TH2F("MC_thetam2",   "MC_thetam2",  100, 0.0,200., 100, 0.08,0.30);
    TH2F *Data_thetaSF= new TH2F("Data_thetaSF", "Data_thetaSF",100, 0.0,0.2,  100, 0.08,0.30);
    TH2F *Data_thetam2= new TH2F("Data_thetam2", "Data_thetam2",100, 0.0,200., 100, 0.08,0.30);
    TH2F *MC_thetap   = new TH2F("MC_thetap",    "MC_thetap",   100, 0.08,0.30,100, 4.5,8.0);
    TH2F *Data_thetap = new TH2F("Data_thetap",  "Data_thetap", 100, 0.08,0.30,100, 4.5,8.0);

    for (int t_i = 0; t_i < Data_tree_MC_resP->GetEntries(); t_i++){
        Data_tree_MC_resP->GetEntry(t_i);
        MC_pSF->Fill(SFECIN_P,P_P);
        MC_pm2->Fill(m2ECIN_P,P_P);
        MC_thetaSF->Fill(SFECIN_P,Theta_P);
        MC_thetam2->Fill(m2ECIN_P,Theta_P);
        MC_thetap->Fill(Theta_P,P_P);
    }
    for (int t_i = 0; t_i < Data_tree_signal->GetEntries(); t_i++){
        Data_tree_signal->GetEntry(t_i);
        Data_pSF->Fill(SFECIN_D,P_D);
        Data_pm2->Fill(m2ECIN_D,P_D);
        Data_thetaSF->Fill(SFECIN_D,Theta_D);
        Data_thetam2->Fill(m2ECIN_D,Theta_D);
        Data_thetap->Fill(Theta_D,P_D);
    }

    struct { TH2F* h; TString name; } h2list[] = {
        {MC_pSF,       "MC_pSF"},
        {MC_pm2,       "MC_pm2"},
        {Data_pSF,     "Data_pSF"},
        {Data_pm2,     "Data_pm2"},
        {MC_thetaSF,   "MC_thetaSF"},
        {MC_thetam2,   "MC_thetam2"},
        {Data_thetaSF, "Data_thetaSF"},
        {Data_thetam2, "Data_thetam2"},
        {MC_thetap,    "MC_thetap"},
        {Data_thetap,  "Data_thetap"}
    };
    for (auto& e : h2list){
        TCanvas *c2d = new TCanvas("c2d","c2d",800,600);
        e.h->Draw("colz");
        TString outname = name_pdf+"_2D_"+e.name+".pdf";
        cout<<"Saving in "<<outname<<endl;
        c2d->SaveAs(outname);
        delete c2d;
    }

    // --- Signal loop: one canvas per variable ---
    cout << "-------------------------------------------------" << endl;
    cout << "Plotting Signal (Article)" << endl;
    cout << "-------------------------------------------------" << endl;

    for (int i = 0; i < (int)labels_sig.size(); i++){
        TString label1          = labels_sig[i][0];
        TString xAxis_label     = labels_sig[i][1];
        TString range_x_option  = labels_sig[i][2];
        TString min_x_option    = labels_sig[i][3];
        TString max_x_option    = labels_sig[i][4];
        TString binning_x_option= labels_sig[i][5];
        TString nb_x_bins       = labels_sig[i][6];
        TString string_cut      = labels_sig[i][7];

        cout << "Doing 1D " << label1 << " plot" << endl;

        TCut cut = string_cut.Data();

        float min_X_histo_ini = 0., max_X_histo_ini = 1.;
        int nBins_X = 20;
        if (range_x_option == "custom_range"){
            min_X_histo_ini = stof((string)min_x_option.Data());
            max_X_histo_ini = stof((string)max_x_option.Data());
        }
        if (binning_x_option == "custom_binning")
            nBins_X = stoi((string)nb_x_bins.Data());

        TH1F *MC_hist       = new TH1F("MC_hist",       "MC_hist",       nBins_X, min_X_histo_ini, max_X_histo_ini);
        TH1F *Data_hist     = new TH1F("Data_hist",     "Data_hist",     nBins_X, min_X_histo_ini, max_X_histo_ini);
        TH1F *Data_cut_hist = new TH1F("Data_cut_hist", "Data_cut_hist", nBins_X, min_X_histo_ini, max_X_histo_ini);

        Data_tree_MC_resP->Draw(label1+"_P>>MC_hist", Form(cut_1*cut&&"P_P>=%g"&&"P_P<%g"&&"Theta_P>%g"&&"Theta_P<=%g",4.5,8.0,0.05,0.15),"",Data_tree_MC_resP->GetEntries());

        TString corr_SF="+0.00";
        TString corr_m2="*1.0";

        if(label1=="SFECIN")
            Data_tree_signal->Draw(label1+d1+corr_SF+">>Data_hist","","",Data_tree_MC_resP->GetEntries()*1);
        else if(label1=="m2ECIN")
            Data_tree_signal->Draw(label1+d1+corr_m2+">>Data_hist","","",Data_tree_MC_resP->GetEntries()*1);
        else
            Data_tree_signal->Draw(label1+d1+">>Data_hist","","",Data_tree_MC_resP->GetEntries()*1);

        if(label1=="SFECIN")
            Data_tree_signal->Draw(label1+d1+corr_SF+">>Data_cut_hist", Form(cut_1*cut&&"scoreBDT_D>=%g",0.0),"",Data_tree_MC_resP->GetEntries()*0.2);
        else if(label1=="m2ECIN")
            Data_tree_signal->Draw(label1+d1+corr_m2+">>Data_cut_hist", Form(cut_1*cut&&"scoreBDT_D>=%g",0.0),"",Data_tree_MC_resP->GetEntries()*0.2);
        else
            Data_tree_signal->Draw(label1+d1+">>Data_cut_hist", Form(cut_1*cut&&"scoreBDT_D>=%g",0.0),"",Data_tree_MC_resP->GetEntries()*0.2);

        MC_hist->Scale(1./MC_hist->Integral());
        Data_hist->Scale(1./Data_hist->Integral());
        Data_cut_hist->Scale(1./Data_cut_hist->Integral());

        MC_hist->Sumw2(kFALSE);
        Data_hist->Sumw2(kFALSE);
        Data_cut_hist->Sumw2(kFALSE);

        MC_hist->SetLineColorAlpha(kCyan-5,0.35);
        MC_hist->SetFillColorAlpha(kCyan-5,0.35);
        MC_hist->SetMarkerStyle(20);
        MC_hist->SetFillStyle(1001);
        MC_hist->SetTitle(";"+xAxis_label+";Normalized counts");

        Data_hist->SetLineColor(kGreen+3);
        Data_hist->SetLineWidth(1);
        Data_hist->SetLineStyle(1);
        Data_hist->SetFillColor(0);
        Data_hist->SetTitle(";"+xAxis_label+";Normalized counts");

        Data_cut_hist->SetLineColor(kGreen+2);
        Data_cut_hist->SetLineStyle(1);
        Data_cut_hist->SetLineWidth(1);
        Data_cut_hist->SetFillColorAlpha(kGreen-10,0.20);
        Data_cut_hist->SetTitle(";"+xAxis_label+";Normalized counts");

        if(i==2)
            MC_hist->SetMaximum(std::max(Data_hist->GetMaximum(),MC_hist->GetMaximum()) * 1.5);
        else
            MC_hist->SetMaximum(std::max(Data_hist->GetMaximum(),MC_hist->GetMaximum()) * 1.2);

        TCanvas *cancSig = new TCanvas("cancSig","cancSig",800,600);
        cancSig->SetLeftMargin(0.14);
        cancSig->SetRightMargin(0.03);
        MC_hist->GetXaxis()->SetTitleOffset(0.85);
        MC_hist->Draw();
        Data_hist->Draw("same");
        Data_cut_hist->Draw("same");

        gStyle->SetLegendTextSize(0.045);
        TLegend *legend = new TLegend(0.62, 0.72, 0.88, 0.88);
        legend->AddEntry(MC_hist,       "MC Signal",        "f");
        legend->AddEntry(Data_hist,     "Data",             "l");
        legend->AddEntry(Data_cut_hist, "Data after 0.0 cut","f");
        legend->SetFillStyle(0);
        legend->SetLineWidth(0);
        legend->SetTextSize(0.045);
        legend->Draw("same");

        TString outname = name_pdf+"_DataMC_"+label1+".pdf";
        cout<<"Saving in "<<outname<<endl;
        cancSig->SaveAs(outname);

        gSystem->mkdir(save_dir+"Figure_8", kTRUE);
        TString outroot = outname;
        outroot.ReplaceAll(save_dir, save_dir+"Figure_8/");
        outroot.ReplaceAll(".pdf", ".root");
        TFile *fout = new TFile(outroot, "RECREATE");
        MC_hist->Write("MC_Signal");
        Data_hist->Write("Data");
        Data_cut_hist->Write("Data_after_cut");
        cancSig->Write("Canvas");
        fout->Close();
        delete fout;

        delete cancSig;
        delete MC_hist;
        delete Data_hist;
        delete Data_cut_hist;
    }
}


void True_False(TString infile, TString name_model,Float_t score_cut, TString outfile)
{
    gROOT->SetBatch(kTRUE);
    
    gStyle->SetPaintTextFormat("4.1f");
    gStyle->SetPalette(kLightTemperature);
    gStyle->SetOptStat(0);
    gStyle->SetLabelSize(.03, "xyz");
    gStyle->SetTitleSize(.03, "xyz");
    gStyle->SetTitleSize(.07, "t");
    gStyle->SetFrameLineWidth(1);
    gStyle->SetLineWidth(1);
    gStyle->SetHistLineWidth(1);
    gStyle->SetMarkerStyle(13);
    gStyle->SetTitleW(0.8); // per cent of the pad width
    gStyle->SetTitleH(0.1); // per cent of the pad height
    
    
    std::vector<vector<TString>> labels1D{
        {"P", "P", "custom_range", "0.", "14.",
            "custom_binning", "100", "", "legend", "P"},
        
        {"Theta", "#Theta", "custom_range", "0.", "1",
            "custom_binning", "100", "", "legend", "electron_SF"},
        
        {"Phi", "#Phi", "custom_range", "-3.2", "3.2",
            "custom_binning", "100", "", "legend", "electron_SF"},
        
        {"SFPCAL", "SF PCAL", "custom_range", "0.", "0.3",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"SFECIN", "SF ECIN", "custom_range", "0.", "0.25",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"SFECOUT", "SF ECOUT", "custom_range", "0.", "0.25",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"m2PCAL", "m2 PCAL", "custom_range", "0.", "200",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"m2ECIN", "m2 ECIN", "custom_range", "0.", "500",
            "custom_binning", "100", "", "legend", "positron_SF_4"},
        
        {"m2ECOUT", "m2 ECOUT", "custom_range", "0.", "500",
            "custom_binning", "100", "", "legend", "positron_SF_4"}
    };
    
    //TFile *Data_file = new TFile("Documents/Lepton_ID_root/"+name+"_Pion.root");
    
    TFile *Data_file = new TFile(infile+".root");
    TTree *Data_tree = (TTree *)Data_file->Get("resP");
    TTree *Data_tree_1 = (TTree *)Data_file->Get("resN");
    int entries_tree = Data_tree->GetEntries();
    int entries_tree_1 = Data_tree_1->GetEntries();
    TCut cut_0 = "";
    TCut cut_1 = "";
    
    int TP,TN,FP,FN;
    
    Float_t P_P, Theta_P, Phi_P,SFPCAL_P,SFECIN_P,SFECOUT_P, m2PCAL_P, m2ECIN_P, m2ECOUT_P;
    Float_t P_N, Theta_N, Phi_N,SFPCAL_N,SFECIN_N,SFECOUT_N, m2PCAL_N, m2ECIN_N, m2ECOUT_N;
    
    Float_t scoreBDT_P,scoreBDT_N,score,scoreMLP_P,scoreMLP_N;
    
    //---------------Tree 1-------------------------------
    //TMVAPos.root, has true positives and false negatives
    //--------------------------------------------------
    //Data: #positives, #True positives, #False Negatives #score
    Data_tree->SetBranchAddress("scoreBDT_P",&scoreBDT_P);
    Data_tree->SetBranchAddress("scoreMLP_P",&scoreMLP_P);
    //positives sample variables
    //Data_tree->SetBranchAddress("P_P", &P_P);
    Data_tree->SetBranchAddress("P_P", &P_P);
    Data_tree->SetBranchAddress("Theta_P", &Theta_P);
    Data_tree->SetBranchAddress("Phi_P", &Phi_P);
    Data_tree->SetBranchAddress("SFPCAL_P", &SFPCAL_P);
    Data_tree->SetBranchAddress("SFECIN_P", &SFECIN_P);
    Data_tree->SetBranchAddress("SFECOUT_P", &SFECOUT_P);
    Data_tree->SetBranchAddress("m2PCAL_P", &m2PCAL_P);
    Data_tree->SetBranchAddress("m2ECIN_P", &m2ECIN_P);
    Data_tree->SetBranchAddress("m2ECOUT_P", &m2ECOUT_P);
    
    
    //---------------Tree 1-------------------------------
    //TMVAPos.root, has true positives and false negatives
    //--------------------------------------------------
    //Data: #negatives, #True negatives, #False positives #score
    Data_tree_1->SetBranchAddress("scoreBDT_N",&scoreBDT_N);
    Data_tree_1->SetBranchAddress("scoreMLP_N",&scoreMLP_N);
    //negatives sample variables
    //Data_tree->SetBranchAddress("P_N", &P_N);
    Data_tree_1->SetBranchAddress("P_N", &P_N);
    Data_tree_1->SetBranchAddress("Theta_N", &Theta_N);
    Data_tree_1->SetBranchAddress("Phi_N", &Phi_N);
    Data_tree_1->SetBranchAddress("SFPCAL_N", &SFPCAL_N);
    Data_tree_1->SetBranchAddress("SFECIN_N", &SFECIN_N);
    Data_tree_1->SetBranchAddress("SFECOUT_N", &SFECOUT_N);
    Data_tree_1->SetBranchAddress("m2PCAL_N", &m2PCAL_N);
    Data_tree_1->SetBranchAddress("m2ECIN_N", &m2ECIN_N);
    Data_tree_1->SetBranchAddress("m2ECOUT_N", &m2ECOUT_N);
    
    
    TString name1="TPvsFP";
    TString name2="FNvsTN";
    TString name_pdf=outfile+"_TrueFalse";
    //Float_t score_cut=0.86;
    
    //From file
    TString n1="_P";
    TString n2="_N";
    //TString n3="_PP";
    //TString n4="_PN";
    
    
    TString label = "True Positive";
    TString label_1 = "False Positive";
    TString label_2 = "True Negative";
    TString label_3 = "False Negative";
    
    int PS=entries_tree;
    int NS=entries_tree_1;
    int T=0;
    int F=0;
    
    TCanvas *cancG0 = new TCanvas("cancG0", "cancG0", 1500, 1200);
    cancG0->Divide(3,3,0.01,0.01,0);
    
    cout << "-------------------------------------------------" << endl;
    cout << "Plotting True Positive vs False Positive" << endl;
    cout << "-------------------------------------------------" << endl;
    for (int i = 0; i < labels1D.size(); i++){
        int PP=0;
        int PN=0;
        cancG0->cd(i+1);
        
        //////////////////////////////////////////////////
        // Set options for each label
        TString label1 = labels1D[i][0];
        TString xAxis_label = labels1D[i][1];
        TString range_x_option = labels1D[i][2];
        TString min_x_option = labels1D[i][3];
        TString max_x_option = labels1D[i][4];
        TString binning_x_option = labels1D[i][5];
        TString nb_x_bins = labels1D[i][6];
        TString string_cut = labels1D[i][7];
        TString legend_option = labels1D[i][8];
        TString output_string = labels1D[i][9];
        //////////////////////////////////////////////////
        cout << "Doing 1D " << label1 << " plot" << endl;
        
        TCut cut = string_cut.Data();
        
        TString cut_string = cut.GetTitle();
        
        float min_X_histo_ini;
        float max_X_histo_ini;
        
        int nBins_X = 20;
        //Extract custom range options
        if (range_x_option == "custom_range"){
            //cout << "Reading range " << endl;
            min_X_histo_ini = stof((string)min_x_option.Data());
            max_X_histo_ini = stof((string)max_x_option.Data());
        }
        
        //Extract custom bins options
        if (binning_x_option == "custom_binning"){
            //cout << "Reading # of data bins: " << nb_x_bins.Data() << endl;
            nBins_X = stoi((string)nb_x_bins.Data());
        }
        
        //TCanvas *c1 = new TCanvas("c1","c1");
        //setupCanvas(c1);
        
        
        //True Positives: Positives (score>=0.86) from positive sample (n1=_P)
        TH1F *Data_hist = new TH1F("Data_hist", "Data_hist", nBins_X, min_X_histo_ini, max_X_histo_ini);
        //Float_t test_cut=0.01;
        if(name_model=="MLP")
            Data_tree->Draw(label1 +n1+">>Data_hist", Form(cut_0 * cut&&"scoreMLP_P>=%g",score_cut));
        if(name_model=="BDT")
            Data_tree->Draw(label1 +n1+">>Data_hist", Form(cut_0 * cut&&"scoreBDT_P>=%g",score_cut));
        
        //TCut weight = Form("%i/%i", entries_tree, entries_tree_1);
        TCut weight = "";
        
        //False Positives: Positives (score>=0.86) from negative sample (n2=_N)
        TH1F *Data_hist_1 = new TH1F("Data_hist_1", "Data_hist_1", nBins_X, min_X_histo_ini, max_X_histo_ini);
        if(name_model=="MLP")
            Data_tree_1->Draw(label1 +n2+ ">>Data_hist_1", Form(weight*cut_1 * cut&&"scoreMLP_N>=%g",score_cut));
        if(name_model=="BDT")
            Data_tree_1->Draw(label1 +n2+ ">>Data_hist_1",Form(weight* cut_1 * cut&&"scoreBDT_N>=%g",score_cut));
        
        Data_hist->Scale(1./Data_hist->Integral(),"nosw2");
        Data_hist_1->Scale(1./Data_hist_1->Integral(),"nosw2");
        
        
        Data_hist->SetMaximum(std::max(Data_hist->GetMaximum(),Data_hist_1->GetMaximum())*1.2);
        
        //Data_hist->SetLineWidth(2);
        Data_hist->SetLineColor(kBlue);
        Data_hist->SetFillStyle(0);
        Data_hist->SetFillColor(0);
        Data_hist->SetTitle(";" + xAxis_label+";counts");
        
        //Data_hist_1->SetLineWidth(2);
        Data_hist_1->SetLineColor(0);
        Data_hist_1->SetFillStyle(0);
        Data_hist_1->SetFillColor(0);
        Data_hist_1->SetMarkerColor(kMagenta);
        Data_hist_1->SetTitle(";" + xAxis_label+";counts");
        
        Data_hist->Draw();
        Data_hist_1->Draw("same");
        
        TRatioPlot *rp = new TRatioPlot(Data_hist, Data_hist_1);
        cancG0->SetTicks(0, 1);
        rp->Draw();
        rp->GetLowerRefGraph()->SetMinimum(0);
        rp->GetLowerRefGraph()->SetMaximum(2);
        rp->GetLowYaxis()->SetNdivisions(505);
        
        
        
        auto legend = new TLegend(0.65, 0.87, 0.87, 0.67);
        legend->SetFillStyle(0);
        legend->SetLineWidth(0);
        TP=Data_hist->GetEntries();
        FP=Data_hist_1->GetEntries();
        legend->AddEntry(Data_hist, Form("%s(%d)", label.Data(),TP), "l");
        legend->AddEntry(Data_hist_1, Form("%s(%d)", label_1.Data(),FP), "lp");
        legend->Draw("same ");
        
        
        
    }
    //cancG0->cd(0);
    cout<<"Saving in "<<name_pdf<<".pdf"<<endl;
    cancG0->SaveAs(name_pdf + ".pdf(");
    
    
    TCanvas *cancG1 = new TCanvas("cancG1", "cancG1", 1500, 1200);
    cancG1->Divide(3,3,0.01,0.01,0);
    
    cout << "-------------------------------------------------" << endl;
    cout << "Plotting True Negative vs False Negative" << endl;
    cout << "-------------------------------------------------" << endl;
    for (int i = 0; i < labels1D.size(); i++){
        int PP=0;
        int PN=0;
        cancG1->cd(i+1);
        
        //////////////////////////////////////////////////
        // Set options for each label
        TString label1 = labels1D[i][0];
        TString xAxis_label = labels1D[i][1];
        TString range_x_option = labels1D[i][2];
        TString min_x_option = labels1D[i][3];
        TString max_x_option = labels1D[i][4];
        TString binning_x_option = labels1D[i][5];
        TString nb_x_bins = labels1D[i][6];
        TString string_cut = labels1D[i][7];
        TString legend_option = labels1D[i][8];
        TString output_string = labels1D[i][9];
        //////////////////////////////////////////////////
        cout << "Doing 1D " << label1 << " plot" << endl;
        
        TCut cut = string_cut.Data();
        
        TString cut_string = cut.GetTitle();
        
        float min_X_histo_ini;
        float max_X_histo_ini;
        
        int nBins_X = 20;
        //Extract custom range options
        if (range_x_option == "custom_range"){
            //cout << "Reading range " << endl;
            min_X_histo_ini = stof((string)min_x_option.Data());
            max_X_histo_ini = stof((string)max_x_option.Data());
        }
        
        //Extract custom bins options
        if (binning_x_option == "custom_binning"){
            //cout << "Reading # of data bins: " << nb_x_bins.Data() << endl;
            nBins_X = stoi((string)nb_x_bins.Data());
        }
        
        TCut weight = "";
        
        //True Negatives: Negatives (score<0.86) from a negative sample (n2=_N)
        TH1F *Data_hist_2 = new TH1F("Data_hist_2", "Data_hist_2", nBins_X, min_X_histo_ini, max_X_histo_ini);
        if(name_model=="MLP")
            Data_tree_1->Draw(label1 +n2+">>Data_hist_2", Form(weight *cut_1 * cut&&"scoreMLP_N<%g",score_cut));
        if(name_model=="BDT")
            Data_tree_1->Draw(label1 +n2+">>Data_hist_2",Form(weight *cut_1 * cut&&"scoreBDT_N<%g",score_cut));
        
        //False Negatives: Negatives (score<0.86) from a positive sample (n1=_P)
        TH1F *Data_hist_3 = new TH1F("Data_hist_3", "Data_hist_3", nBins_X, min_X_histo_ini, max_X_histo_ini);
        if(name_model=="MLP")
            Data_tree->Draw(label1 +n1+">>Data_hist_3", Form(cut_0 * cut&&"scoreMLP_P<%g",score_cut));
        if(name_model=="BDT")
            Data_tree->Draw(label1 +n1+">>Data_hist_3", Form(cut_0 * cut&&"scoreBDT_P<%g",score_cut));
        
        
        
        
        //Double_t scale = Data_hist_3->GetXaxis()->GetBinWidth(1)/(Data_hist_3->Integral());
        
        Data_hist_2->Scale(1./Data_hist_2->Integral(),"nosw2");
        Data_hist_3->Scale(1./Data_hist_3->Integral(),"nosw2");
        
        //Data_hist_1->SetLineWidth(2);
        Data_hist_2->SetLineColor(kRed);
        Data_hist_2->SetFillStyle(3004);
        Data_hist_2->SetFillColor(kRed);
        Data_hist_2->SetTitle(";" + xAxis_label+";counts");
        
        
        //Data_hist_1->SetLineWidth(2);
        Data_hist_3->SetLineColor(kYellow+1);
        Data_hist_3->SetFillStyle(3004);
        Data_hist_3->SetFillColor(kYellow+1);
        Data_hist_3->SetTitle(";" + xAxis_label+";counts");
        
        
        Data_hist_3->SetMaximum(std::max(Data_hist_3->GetMaximum(),Data_hist_2->GetMaximum()) * 1.2);
        
        
        
        auto legend = new TLegend(0.65, 0.87, 0.87, 0.67);
        legend->SetFillStyle(0);
        legend->SetLineWidth(0);
        
        FN=Data_hist_2->GetEntries();
        TN=Data_hist_3->GetEntries();
        
        
        legend->AddEntry(Data_hist_2, Form("%s(%d)", label_2.Data(),FN), "l");
        legend->AddEntry(Data_hist_3, Form("%s(%d)", label_3.Data(),TN), "l");
        Data_hist_2->Draw("same");
        Data_hist_3->Draw("same");
        legend->Draw("same ");
        
    }
    //cancG0->cd(0);
    cout<<"Saving in "<<name_pdf<<".pdf"<<endl;
    
    cancG1->SaveAs(name_pdf+ ".pdf)");
    
    //gApplication->Terminate();
    
    
}

void ROC(TString infile, TString name_model="All"){
    
    gROOT->SetBatch(kTRUE);
    
    gStyle->SetPaintTextFormat("4.1f");
    gStyle->SetPalette(kLightTemperature);
    gStyle->SetOptStat(0);
    gStyle->SetLabelSize(.03, "xyz");
    gStyle->SetTitleSize(.03, "xyz");
    gStyle->SetTitleSize(.07, "t");
    gStyle->SetFrameLineWidth(1);
    gStyle->SetLineWidth(1);
    gStyle->SetHistLineWidth(1);
    gStyle->SetMarkerStyle(13);
    gStyle->SetTitleW(0.8); // per cent of the pad width
    gStyle->SetTitleH(0.1); // per cent of the pad height
    
    TFile *Data_file = new TFile(infile+".root");
    TTree *Data_tree = (TTree *)Data_file->Get("resP");
    TTree *Data_tree_1 = (TTree *)Data_file->Get("resN");
    int entries_tree = Data_tree->GetEntries();
    int entries_tree_1 = Data_tree_1->GetEntries();
    
    
    int TP,TN,FP,FN;
    
    Float_t scoreBDT_P,scoreBDT_N,score,scoreMLP_P,scoreMLP_N;
    
    //---------------Tree 1-------------------------------
    //TMVAPos.root, has true positives and false negatives
    //--------------------------------------------------
    //Data: #positives, #True positives, #False Negatives #score
    Data_tree->SetBranchAddress("scoreBDT_P",&scoreBDT_P);
    Data_tree->SetBranchAddress("scoreMLP_P",&scoreMLP_P);
    
    
    //---------------Tree 1-------------------------------
    //TMVAPos.root, has true positives and false negatives
    //--------------------------------------------------
    //Data: #negatives, #True negatives, #False positives #score
    Data_tree_1->SetBranchAddress("scoreBDT_N",&scoreBDT_N);
    Data_tree_1->SetBranchAddress("scoreMLP_N",&scoreMLP_N);
    
    Float_t segments, start, end;
    Float_t  start_MLP, end_MLP;
    Float_t  start_BDT, end_BDT;
    Float_t TPR,FPR;
    Float_t score_cut;
    Int_t max=500;
    int i=1;
    Double_t x[max], y[max];
    Double_t x2[max], y2[max];
    
    
    start_MLP=-1.25;
    end_MLP=1.5;
    start_BDT=-0.8;
    end_BDT=0.8;
    
    
    
    
    
    if(name_model=="MLP"||name_model=="All"){
        cout<<"ROC--MLP---Start"<<endl;
        start=start_MLP;
        end=end_MLP;
        segments=(end-start)/max;
        
        score_cut=start;
        
        
        segments=(end-start)/max;
        while (score_cut<=end){
            TP=Data_tree->GetEntries(Form("scoreMLP_P>=%g",score_cut));
            TN=Data_tree_1->GetEntries(Form("scoreMLP_N<%g",score_cut));
            FP=Data_tree_1->GetEntries(Form("scoreMLP_N>=%g",score_cut));
            FN=Data_tree->GetEntries(Form("scoreMLP_P<%g",score_cut));
            
            TPR=(1.0*TP)/(TP+FN);
            FPR=(1.0*FP)/(FP+TN);
            
            
            x[i-1]=TPR;
            y[i-1]=1-FPR;
            
            score_cut=start+i*segments;
            i++;
        }
        cout<<"ROC--MLP---End"<<endl;
    }
    i=1;
    
    if(name_model=="BDT"||name_model=="All"){
        cout<<"ROC--BDT---Start"<<endl;
        
        start=start_BDT;
        end=end_BDT;
        segments=(end-start)/max;
        
        score_cut=start;
        
        while (score_cut<=end){
            TP=Data_tree->GetEntries(Form("scoreBDT_P>=%g",score_cut));
            TN=Data_tree_1->GetEntries(Form("scoreBDT_N<%g",score_cut));
            FP=Data_tree_1->GetEntries(Form("scoreBDT_N>=%g",score_cut));
            FN=Data_tree->GetEntries(Form("scoreBDT_P<%g",score_cut));
            
            TPR=(1.0*TP)/(TP+FN);
            FPR=(1.0*FP)/(FP+TN);
            
            
            x2[i-1]=TPR;
            y2[i-1]=1-FPR;
            
            score_cut=start+i*segments;
            i++;
        }
        cout<<"ROC--BDT---End"<<endl;
        
    }
    
    TCanvas *c1 = new TCanvas("c1","c1",1000, 800);
    
    TGraph *gr_MLP  = new TGraph(max,x,y);
    TGraph *gr_BDT  = new TGraph(max,x2,y2);
    
    
    
    if(name_model=="MLP"){
        gr_MLP->SetLineColor(1);
        gr_MLP->SetLineWidth(3);
        gr_MLP->SetMarkerStyle(20);
        gr_MLP->Draw("ALP");
        c1->SaveAs(name_model+"_ROC.png");
    }
    else if(name_model=="BDT"){
        gr_BDT->SetLineWidth(3);
        gr_BDT->SetMarkerStyle(21);
        gr_BDT->SetLineColor(2);
        gr_BDT->Draw("ALP");
        c1->SaveAs(name_model+"_ROC.png");
    }
    else{
        cout<<"PLOT ROC--MLP AND BDT"<<endl;
        Float_t areaMLP, areaBDT;
        TMultiGraph *mg = new TMultiGraph();
        mg->SetTitle("ROC curve for both methods; True Positives Rate; 1- False Positives Rate");
        
        auto legend = new TLegend();
        legend->SetFillStyle(0);
        legend->SetLineWidth(0);
        mg->GetXaxis()->SetRangeUser(0.75, 1.);
        mg->GetYaxis()->SetRangeUser(0.75, 1.01);
        
        
        gr_MLP->SetName("gr1");
        gr_MLP->SetLineColorAlpha(kBlue,0.95);
        gr_MLP->SetLineWidth(4);
        
        
        //gr_BDT->SetLineStyle(10);
        gr_BDT->SetName("gr2");
        gr_BDT->SetLineWidth(4);
        gr_BDT->SetLineColorAlpha(kRed, 0.45);
        
        
        areaMLP=gr_MLP->Integral();
        areaBDT=gr_BDT->Integral();
        
        
        mg->Add(gr_MLP);
        mg->Add(gr_BDT);
        mg->Draw("AL");
        
        legend->AddEntry("gr1",Form("MLP"),"l");
        legend->AddEntry("gr2",Form("BDT"),"l");
        legend->Draw();
        
        
        c1->SaveAs(infile+"_ROC.png");
    }
    
    
    
    
    
    
    
    //TCanvas *cancG1 = new TCanvas("cancG1", "cancG1", 1500, 1200);
    //cancG1->Divide(3,3,0.01,0.01,0);
    
    //TH1F *Data_hist_3 = new TH1F("Data_hist_3", "Data_hist_3", nBins_X, min_X_histo_ini, max_X_histo_ini);
    
    
    
}

void Print_Table(int TP, int FP, int TN, int FN){
    cout << "              "    << " | "
         << "Actual e+  "    << " | "
         << "Actual pi+"<< "\n";
    
    cout << std::string(10*3 + 2*3, '-') << "\n";
    
    
    cout << "Predicted e^+ "   << " | "<< TP << " | "<< FP << "\n";
    
    cout << "Predicted pi^+"   << " | "<< FN << " | "<< TN << "\n";
}

void ROC_small(){
    gROOT->SetBatch(kTRUE);
    
    gStyle->SetPaintTextFormat("4.1f");
    gStyle->SetPalette(kLightTemperature);
    gStyle->SetOptStat(0);
    gStyle->SetLabelSize(.03, "xyz");
    gStyle->SetTitleSize(.03, "xyz");
    gStyle->SetTitleSize(.07, "t");
    gStyle->SetFrameLineWidth(1);
    gStyle->SetLineWidth(1);
    gStyle->SetHistLineWidth(1);
    gStyle->SetMarkerStyle(13);
    gStyle->SetTitleW(0.8); // per cent of the pad width
    gStyle->SetTitleH(0.1); // per cent of the pad height
    
    
    
    Double_t y_6bdt[10]={1-1.000000,1-0.900379,1-0.461364,1-0.235985, 1-0.142424, 1-0.080303, 1-0.000300,1- 0.000000,1- 0.000000,1- 0.000000};
    Double_t x_6bdt[10]={1.000000,1.000000,0.893591,0.977025, 0.996372, 0.950423, 0.961305, 0.841596, 0.320435, 0.000000};

    
    TGraph *gr_6BDT_data  = new TGraph(10,x_6bdt,y_6bdt);
    gr_6BDT_data->GetXaxis()->SetRangeUser(0.0, 1.);
    gr_6BDT_data->GetYaxis()->SetRangeUser(0.0, 1.);
    gr_6BDT_data->SetTitle("ROC; True Positives Rate; 1- False Positives Rate");
    

}

void ROC_All(TString infile, TString infile2, TString name=""){
    
    gROOT->SetBatch(kTRUE);
    
    gStyle->SetPaintTextFormat("4.1f");
    gStyle->SetPalette(kLightTemperature);
    gStyle->SetOptStat(0);
    gStyle->SetLabelSize(.03, "xyz");
    gStyle->SetTitleSize(.03, "xyz");
    gStyle->SetTitleSize(.07, "t");
    gStyle->SetFrameLineWidth(1);
    gStyle->SetLineWidth(1);
    gStyle->SetHistLineWidth(1);
    gStyle->SetMarkerStyle(13);
    gStyle->SetTitleW(0.8); // per cent of the pad width
    gStyle->SetTitleH(0.1); // per cent of the pad height
    
    TFile *Data_file = new TFile(infile+".root");
    TTree *Data_tree_P = (TTree *)Data_file->Get("resP");
    TTree *Data_tree_N = (TTree *)Data_file->Get("resN");
    
    TFile *Data_file2 = new TFile(infile2+".root");
    TTree *Data_tree2_P = (TTree *)Data_file2->Get("resP");
    TTree *Data_tree2_N = (TTree *)Data_file2->Get("resN");
    
    
    Float_t scoreBDT_P,scoreBDT_N,score,scoreMLP_P,scoreMLP_N;
    
    //---------------File 1-------------------------------
    
    Data_tree_P->SetBranchAddress("scoreBDT_P",&scoreBDT_P);
    Data_tree_P->SetBranchAddress("scoreMLP_P",&scoreMLP_P);
    Data_tree_N->SetBranchAddress("scoreBDT_N",&scoreBDT_N);
    Data_tree_N->SetBranchAddress("scoreMLP_N",&scoreMLP_N);
    
    
    //---------------File 2-------------------------------
    
    Data_tree2_P->SetBranchAddress("scoreBDT_P",&scoreBDT_P);
    Data_tree2_P->SetBranchAddress("scoreMLP_P",&scoreMLP_P);
    Data_tree2_N->SetBranchAddress("scoreBDT_N",&scoreBDT_N);
    Data_tree2_N->SetBranchAddress("scoreMLP_N",&scoreMLP_N);
    
    Float_t segments, start, end;
    Float_t  start_MLP, end_MLP;
    Float_t  start_BDT, end_BDT;
    Float_t TPR,FPR;
    Float_t score_cut;
    Int_t max=500;
    int TP=0;
    int FP=0;
    int TN=0;
    int FN=0;
    int i=1;
    Double_t x_mlp[max], y_mlp[max];
    Double_t x_bdt[max], y_bdt[max];
    
    Double_t x2_mlp[max], y2_mlp[max];
    Double_t x2_bdt[max], y2_bdt[max];
    
    
    start_MLP=-1.25;
    end_MLP=1.5;
    start_BDT=-0.8;
    end_BDT=0.8;
    
    cout<<"ROC "<<infile<<"--MLP"<<endl;
    
    start=start_MLP;
    end=end_MLP;
    segments=(end-start)/max;
    score_cut=start;
    
    while (score_cut<=end){
        TP=Data_tree_P->GetEntries(Form("scoreMLP_P>=%g",score_cut));
        TN=Data_tree_N->GetEntries(Form("scoreMLP_N<%g",score_cut));
        FP=Data_tree_N->GetEntries(Form("scoreMLP_N>=%g",score_cut));
        FN=Data_tree_P->GetEntries(Form("scoreMLP_P<%g",score_cut));
        
        TPR=(1.0*TP)/(TP+FN);
        FPR=(1.0*FP)/(FP+TN);
        
        
        x_mlp[i-1]=TPR;
        y_mlp[i-1]=1-FPR;
        
        score_cut=start+i*segments;
        i++;
    }
    
    
    i=1;
    
    cout<<"ROC "<<infile<<"--BDT"<<endl;
    
    start=start_BDT;
    end=end_BDT;
    segments=(end-start)/max;
    score_cut=start;
    
    while (score_cut<=end){
        TP=Data_tree_P->GetEntries(Form("scoreBDT_P>=%g",score_cut));
        TN=Data_tree_N->GetEntries(Form("scoreBDT_N<%g",score_cut));
        FP=Data_tree_N->GetEntries(Form("scoreBDT_N>=%g",score_cut));
        FN=Data_tree_P->GetEntries(Form("scoreBDT_P<%g",score_cut));
        
        TPR=(1.0*TP)/(TP+FN);
        FPR=(1.0*FP)/(FP+TN);
        
        
        x_bdt[i-1]=TPR;
        y_bdt[i-1]=1-FPR;
        
        score_cut=start+i*segments;
        i++;
    }
    
    //Print_Table(TP,FP,TN,FN);
    i=1;
    
    cout<<"ROC "<<infile2<<"--MLP"<<endl;
    
    start=start_MLP;
    end=end_MLP;
    segments=(end-start)/max;
    score_cut=start;
    
    while (score_cut<=end){
        TP=Data_tree2_P->GetEntries(Form("scoreMLP_P>=%g",score_cut));
        TN=Data_tree2_N->GetEntries(Form("scoreMLP_N<%g",score_cut));
        FP=Data_tree2_N->GetEntries(Form("scoreMLP_N>=%g",score_cut));
        FN=Data_tree2_P->GetEntries(Form("scoreMLP_P<%g",score_cut));
        
        TPR=(1.0*TP)/(TP+FN);
        FPR=(1.0*FP)/(FP+TN);
        
        
        x2_mlp[i-1]=TPR;
        y2_mlp[i-1]=1-FPR;
        
        score_cut=start+i*segments;
        i++;
    }
    
    //Print_Table(TP,FP,TN,FN);
    
    i=1;
    
    cout<<"ROC "<<infile2<<"--BDT"<<endl;
    
    start=start_BDT;
    end=end_BDT;
    segments=(end-start)/max;
    score_cut=start;
    
    while (score_cut<=end){
        TP=Data_tree2_P->GetEntries(Form("scoreBDT_P>=%g",score_cut));
        TN=Data_tree2_N->GetEntries(Form("scoreBDT_N<%g",score_cut));
        FP=Data_tree2_N->GetEntries(Form("scoreBDT_N>=%g",score_cut));
        FN=Data_tree2_P->GetEntries(Form("scoreBDT_P<%g",score_cut));
        
        TPR=(1.0*TP)/(TP+FN);
        FPR=(1.0*FP)/(FP+TN);
        
        
        x2_bdt[i-1]=TPR;
        y2_bdt[i-1]=1-FPR;
        
        score_cut=start+i*segments;
        i++;
    }
    //Print_Table(TP,FP,TN,FN);
    
    
   TCanvas *c1 = new TCanvas("c1","c1",1000, 800);
    
    TGraph *gr_MLP  = new TGraph(max,x_mlp,y_mlp);
    TGraph *gr_BDT  = new TGraph(max,x_bdt,y_bdt);
    TGraph *gr2_MLP  = new TGraph(max,x2_mlp,y2_mlp);
    TGraph *gr2_BDT  = new TGraph(max,x2_bdt,y2_bdt);
    
    //x is true positives rate: signal efficiency
    //y is 1-False positive rate: 1-background suppresion.
    

     Double_t y_6bdt[15]={1-0.999999,1-0.993554, 1-0.928284, 1-0.785657, 1-0.605157,1- 0.476229, 1-0.370669,1-0.303787, 1-0.267526, 1-0.185334, 1-0.145044, 1-0.069299, 1-0.031426, 1-0.000000, 1-0.000000};
    Double_t x_6bdt[15]={1.000000, 1.000655, 1.001802, 1.005242, 1.000983, 0.998198, 0.993775, 0.987221, 0.988041, 0.981979, 0.985419, 0.953146, 0.875983, 0.449869, 0.007208};

    Double_t yerr_6bdt[15]={0.053108, 0.052831, 0.050149, 0.044896, 0.037366, 0.032240, 0.027388, 0.024289, 0.022152, 0.018070, 0.016485, 0.012398, 0.009577, 0.000000, 0.000000};
    Double_t xerr_6bdt[15]={0.024726, 0.024737, 0.024709, 0.024661, 0.024393, 0.024251, 0.023977, 0.023687, 0.023568, 0.023269, 0.023024, 0.021958, 0.020518, 0.012710, 0.004275};

    
    Double_t y_9bdt[15]={1-0.999999, 1-0.999194, 1-0.947623, 1-0.754230, 1-0.503626, 1-0.389202, 1-0.269138, 1-0.161160, 1-0.135375, 1-0.112006, 1-0.091056, 1-0.035455, 1-0.000000, 1-0.000000, 1-0.000000};
    Double_t x_9bdt[15]={1.000000, 1.000328, 1.000819, 1.006225, 1.008519, 1.001147, 0.995904, 0.997543, 1.000491, 0.997870, 0.978539, 0.917104, 0.788336, 0.277031, 0.001147};
    
    Double_t yerr_9bdt[15]={0.053108, 0.053051, 0.051076, 0.043493, 0.033171, 0.028131, 0.022039, 0.017344, 0.015487, 0.014788, 0.014190, 0.009149, 0.000000, 0.000000, 0.000000};
    Double_t xerr_9bdt[15]={0.024726, 0.024732, 0.024696, 0.024596, 0.024279, 0.023982, 0.023653, 0.023254, 0.023075, 0.022863, 0.022447, 0.021209, 0.018896, 0.009785, 0.004145};
    
    
    TGraphErrors *gr_6BDT_data  = new TGraphErrors(15,x_6bdt,y_6bdt,xerr_6bdt,yerr_6bdt);
    TGraphErrors *gr_9BDT_data  = new TGraphErrors(15,x_9bdt,y_9bdt,xerr_9bdt,yerr_9bdt);
    //TGraph *gr_9BDT_data  = new TGraph(11,x2_bdt,y2_bdt);
    
    
    cout<<"PLOT ROC--MLP AND BDT"<<endl;
    Float_t areaMLP, areaBDT;
    Float_t areaMLP2, areaBDT2;
    TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle(" ; True Positives Rate; 1- False Positives Rate");
    
    auto legend = new TLegend(0.15, 0.15, 0.35, 0.35);
    legend->SetFillStyle(0);
    legend->SetTextSize(0.035);
    legend->SetLineWidth(0);
    mg->GetXaxis()->SetRangeUser(0.75, 1.025);
    mg->GetYaxis()->SetRangeUser(0.75, 1.01);
    
    
    gr_MLP->SetName("MLP6");
    gr_MLP->SetLineColorAlpha(kViolet,0.95);
    gr_MLP->SetLineWidth(4);
    
    gr2_MLP->SetName("MLP9");
    gr2_MLP->SetLineColorAlpha(kBlue,0.95);
    gr2_MLP->SetLineWidth(4);
    
    gr_BDT->SetName("BDT6");
    gr_BDT->SetLineWidth(4);
    gr_BDT->SetLineColor(kOrange);
    
    gr2_BDT->SetName("BDT9");
    gr2_BDT->SetLineWidth(4);
    gr2_BDT->SetLineColor(kRed);
    
    gr_6BDT_data->SetName("BDT6_data");
    gr_6BDT_data->SetMarkerStyle(21);
    gr_6BDT_data->SetMarkerColor(kOrange);
    gr_6BDT_data->SetLineColor(kOrange);
    
    gr_9BDT_data->SetName("BDT9_data");
    gr_9BDT_data->SetMarkerStyle(21);
    gr_9BDT_data->SetMarkerColor(kRed);
    gr_9BDT_data->SetLineColor(kRed);
    //gr_9BDT_data->SetLineWidth(4);
    
    double x[] = { 0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1};
    double x1[] = { 0.0,0.2,0.4,0.6,0.8,1};
    TF1 f1("f1",[&](double *x, double *){ return gr_MLP->Eval(x[0]); },0,1,0);
    TF1 f2("f2",[&](double *x1, double *){ return gr_MLP->Eval(x1[0]); },0,1,0);
    TF1 f3("f3",[&](double *x, double *){ return gr_MLP->Eval(x[0]); },0,1,0);
    TF1 f4("f4",[&](double *x, double *){ return gr_MLP->Eval(x[0]); },0,1,0);
    
    //areaMLP = f1.Integral(0.0,1.0);
    
    //TF1 f1("f",[&](double *x, double *){ return g.Eval(x[0]); },0,1);
    
    
    areaMLP=gr_MLP->Integral()+0.5;
    areaBDT=gr_BDT->Integral()+0.5;
    areaMLP2=gr2_MLP->Integral()+0.5;
    areaBDT2=gr2_BDT->Integral()+0.5;

    mg->Add(gr2_MLP);
    mg->Add(gr_MLP);
    
   
    mg->Add(gr2_BDT);
     mg->Add(gr_BDT);


    //mg->Add(gr_6BDT_data, "pl");
    //mg->Add(gr_9BDT_data, "pl");
    mg->Draw("AL");
    
    //legend->AddEntry("gr1",Form("MLP 9 variables (%f)",areaMLP),"l");
    legend->AddEntry("MLP9",Form("MLP 9 variables (AUC=%.3f)",areaMLP),"l");
    legend->AddEntry("BDT9",Form("BDT 9 variables (AUC=%.3f)",areaBDT),"l");
    legend->AddEntry("MLP6",Form("MLP 6 variables (AUC=%.3f)",areaMLP2),"l");
    legend->AddEntry("BDT6",Form("BDT 6 variables (AUC=%.3f)",areaBDT2),"l");
    //legend->AddEntry("BDT6_data","BDT 6 variables Data","p");
    
    //legend->AddEntry("BDT6_data","BDT 6 variables Data","p");
    
    //legend->AddEntry("BDT9_data","BDT 9 variables Data","p");
    //legend->AddEntry("gr2","BDT 9 variables Simulations","l");
    //legend->AddEntry("gr6","BDT 9 variables Data","p");
    //legend->AddEntry("gr3",Form("MLP 6 variables (%f)",areaMLP2),"l");
    //legend->AddEntry("gr4",Form("BDT 6 variables (%f)",areaBDT2),"l");
    //legend->AddEntry("gr3",Form("MLP 6 variables (%f)",areaMLP2),"l");
    legend->Draw();
    
    
    c1->SaveAs("ROC_"+name+".pdf");
}

void ROC_All_Article(TString infile, TString infile2, TString name=""){

    gROOT->SetBatch(kTRUE);

    gStyle->SetPaintTextFormat("4.1f");
    gStyle->SetPalette(kLightTemperature);
    gStyle->SetOptStat(0);
    gStyle->SetLabelSize(.035, "xyz");
    gStyle->SetTitleSize(.035, "xyz");
    gStyle->SetTitleSize(.07, "t");
    gStyle->SetFrameLineWidth(1);
    gStyle->SetLineWidth(1);
    gStyle->SetHistLineWidth(1);
    gStyle->SetMarkerStyle(13);
    gStyle->SetTitleW(0.8);
    gStyle->SetTitleH(0.1);

    TFile *Data_file  = new TFile(infile+".root");
    TTree *Data_tree_P  = (TTree *)Data_file->Get("resP");
    TTree *Data_tree_N  = (TTree *)Data_file->Get("resN");

    TFile *Data_file2 = new TFile(infile2+".root");
    TTree *Data_tree2_P = (TTree *)Data_file2->Get("resP");
    TTree *Data_tree2_N = (TTree *)Data_file2->Get("resN");

    Float_t scoreBDT_P,scoreBDT_N,score,scoreMLP_P,scoreMLP_N;

    Data_tree_P->SetBranchAddress("scoreBDT_P",&scoreBDT_P);
    Data_tree_P->SetBranchAddress("scoreMLP_P",&scoreMLP_P);
    Data_tree_N->SetBranchAddress("scoreBDT_N",&scoreBDT_N);
    Data_tree_N->SetBranchAddress("scoreMLP_N",&scoreMLP_N);

    Data_tree2_P->SetBranchAddress("scoreBDT_P",&scoreBDT_P);
    Data_tree2_P->SetBranchAddress("scoreMLP_P",&scoreMLP_P);
    Data_tree2_N->SetBranchAddress("scoreBDT_N",&scoreBDT_N);
    Data_tree2_N->SetBranchAddress("scoreMLP_N",&scoreMLP_N);

    Float_t segments, start, end;
    Float_t start_MLP, end_MLP;
    Float_t start_BDT, end_BDT;
    Float_t TPR,FPR;
    Float_t score_cut;
    Int_t max=500;
    int TP=0, FP=0, TN=0, FN=0, i=1;
    Double_t x_mlp[max], y_mlp[max];
    Double_t x_bdt[max], y_bdt[max];
    Double_t x2_mlp[max], y2_mlp[max];
    Double_t x2_bdt[max], y2_bdt[max];

    start_MLP=-1.25; end_MLP=1.5;
    start_BDT=-0.8;  end_BDT=0.8;

    cout<<"ROC "<<infile<<"--MLP"<<endl;
    start=start_MLP; end=end_MLP;
    segments=(end-start)/max; score_cut=start;
    while (score_cut<=end){
        TP=Data_tree_P->GetEntries(Form("scoreMLP_P>=%g",score_cut));
        TN=Data_tree_N->GetEntries(Form("scoreMLP_N<%g",score_cut));
        FP=Data_tree_N->GetEntries(Form("scoreMLP_N>=%g",score_cut));
        FN=Data_tree_P->GetEntries(Form("scoreMLP_P<%g",score_cut));
        TPR=(1.0*TP)/(TP+FN); FPR=(1.0*FP)/(FP+TN);
        x_mlp[i-1]=TPR; y_mlp[i-1]=1-FPR;
        score_cut=start+i*segments; i++;
    }

    i=1;
    cout<<"ROC "<<infile<<"--BDT"<<endl;
    start=start_BDT; end=end_BDT;
    segments=(end-start)/max; score_cut=start;
    while (score_cut<=end){
        TP=Data_tree_P->GetEntries(Form("scoreBDT_P>=%g",score_cut));
        TN=Data_tree_N->GetEntries(Form("scoreBDT_N<%g",score_cut));
        FP=Data_tree_N->GetEntries(Form("scoreBDT_N>=%g",score_cut));
        FN=Data_tree_P->GetEntries(Form("scoreBDT_P<%g",score_cut));
        TPR=(1.0*TP)/(TP+FN); FPR=(1.0*FP)/(FP+TN);
        x_bdt[i-1]=TPR; y_bdt[i-1]=1-FPR;
        score_cut=start+i*segments; i++;
    }

    i=1;
    cout<<"ROC "<<infile2<<"--MLP"<<endl;
    start=start_MLP; end=end_MLP;
    segments=(end-start)/max; score_cut=start;
    while (score_cut<=end){
        TP=Data_tree2_P->GetEntries(Form("scoreMLP_P>=%g",score_cut));
        TN=Data_tree2_N->GetEntries(Form("scoreMLP_N<%g",score_cut));
        FP=Data_tree2_N->GetEntries(Form("scoreMLP_N>=%g",score_cut));
        FN=Data_tree2_P->GetEntries(Form("scoreMLP_P<%g",score_cut));
        TPR=(1.0*TP)/(TP+FN); FPR=(1.0*FP)/(FP+TN);
        x2_mlp[i-1]=TPR; y2_mlp[i-1]=1-FPR;
        score_cut=start+i*segments; i++;
    }

    i=1;
    cout<<"ROC "<<infile2<<"--BDT"<<endl;
    start=start_BDT; end=end_BDT;
    segments=(end-start)/max; score_cut=start;
    while (score_cut<=end){
        TP=Data_tree2_P->GetEntries(Form("scoreBDT_P>=%g",score_cut));
        TN=Data_tree2_N->GetEntries(Form("scoreBDT_N<%g",score_cut));
        FP=Data_tree2_N->GetEntries(Form("scoreBDT_N>=%g",score_cut));
        FN=Data_tree2_P->GetEntries(Form("scoreBDT_P<%g",score_cut));
        TPR=(1.0*TP)/(TP+FN); FPR=(1.0*FP)/(FP+TN);
        x2_bdt[i-1]=TPR; y2_bdt[i-1]=1-FPR;
        score_cut=start+i*segments; i++;
    }

    TString save_dir = "/Users/mariana/Work/Results/Article/";
    gSystem->mkdir(save_dir, kTRUE);

    TCanvas *c1 = new TCanvas("c1","c1",1000,800);

    TGraph *gr_MLP  = new TGraph(max,x_mlp, y_mlp);
    TGraph *gr_BDT  = new TGraph(max,x_bdt, y_bdt);
    TGraph *gr2_MLP = new TGraph(max,x2_mlp,y2_mlp);
    TGraph *gr2_BDT = new TGraph(max,x2_bdt,y2_bdt);

    Double_t y_6bdt[15]={1-0.999999,1-0.993554, 1-0.928284, 1-0.785657, 1-0.605157,1- 0.476229, 1-0.370669,1-0.303787, 1-0.267526, 1-0.185334, 1-0.145044, 1-0.069299, 1-0.031426, 1-0.000000, 1-0.000000};
    Double_t x_6bdt[15]={1.000000, 1.000655, 1.001802, 1.005242, 1.000983, 0.998198, 0.993775, 0.987221, 0.988041, 0.981979, 0.985419, 0.953146, 0.875983, 0.449869, 0.007208};
    Double_t yerr_6bdt[15]={0.053108, 0.052831, 0.050149, 0.044896, 0.037366, 0.032240, 0.027388, 0.024289, 0.022152, 0.018070, 0.016485, 0.012398, 0.009577, 0.000000, 0.000000};
    Double_t xerr_6bdt[15]={0.024726, 0.024737, 0.024709, 0.024661, 0.024393, 0.024251, 0.023977, 0.023687, 0.023568, 0.023269, 0.023024, 0.021958, 0.020518, 0.012710, 0.004275};

    Double_t y_9bdt[15]={1-0.999999, 1-0.999194, 1-0.947623, 1-0.754230, 1-0.503626, 1-0.389202, 1-0.269138, 1-0.161160, 1-0.135375, 1-0.112006, 1-0.091056, 1-0.035455, 1-0.000000, 1-0.000000, 1-0.000000};
    Double_t x_9bdt[15]={1.000000, 1.000328, 1.000819, 1.006225, 1.008519, 1.001147, 0.995904, 0.997543, 1.000491, 0.997870, 0.978539, 0.917104, 0.788336, 0.277031, 0.001147};
    Double_t yerr_9bdt[15]={0.053108, 0.053051, 0.051076, 0.043493, 0.033171, 0.028131, 0.022039, 0.017344, 0.015487, 0.014788, 0.014190, 0.009149, 0.000000, 0.000000, 0.000000};
    Double_t xerr_9bdt[15]={0.024726, 0.024732, 0.024696, 0.024596, 0.024279, 0.023982, 0.023653, 0.023254, 0.023075, 0.022863, 0.022447, 0.021209, 0.018896, 0.009785, 0.004145};

    TGraphErrors *gr_6BDT_data = new TGraphErrors(15,x_6bdt,y_6bdt,xerr_6bdt,yerr_6bdt);
    TGraphErrors *gr_9BDT_data = new TGraphErrors(15,x_9bdt,y_9bdt,xerr_9bdt,yerr_9bdt);

    cout<<"PLOT ROC--MLP AND BDT"<<endl;
    Float_t areaMLP, areaBDT, areaMLP2, areaBDT2;
    TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle(" ; True Positives Rate; 1- False Positives Rate");

    auto legend = new TLegend(0.15, 0.15, 0.45, 0.38);
    legend->SetFillStyle(0);
    legend->SetTextSize(0.035);
    legend->SetLineWidth(0);
    mg->GetXaxis()->SetRangeUser(0.75, 1.025);
    mg->GetYaxis()->SetRangeUser(0.75, 1.01);

    gr_MLP->SetName("MLP6");
    gr_MLP->SetLineColorAlpha(kViolet,0.95);
    gr_MLP->SetLineWidth(4);

    gr2_MLP->SetName("MLP9");
    gr2_MLP->SetLineColorAlpha(kBlue,0.95);
    gr2_MLP->SetLineWidth(4);

    gr_BDT->SetName("BDT6");
    gr_BDT->SetLineWidth(4);
    gr_BDT->SetLineColor(kOrange);

    gr2_BDT->SetName("BDT9");
    gr2_BDT->SetLineWidth(4);
    gr2_BDT->SetLineColor(kRed);

    gr_6BDT_data->SetName("BDT6_data");
    gr_6BDT_data->SetMarkerStyle(21);
    gr_6BDT_data->SetMarkerColor(kOrange);
    gr_6BDT_data->SetLineColor(kOrange);

    gr_9BDT_data->SetName("BDT9_data");
    gr_9BDT_data->SetMarkerStyle(21);
    gr_9BDT_data->SetMarkerColor(kRed);
    gr_9BDT_data->SetLineColor(kRed);

    areaMLP =gr_MLP->Integral() +0.5;
    areaBDT =gr_BDT->Integral() +0.5;
    areaMLP2=gr2_MLP->Integral()+0.5;
    areaBDT2=gr2_BDT->Integral()+0.5;

    mg->Add(gr2_MLP);
    mg->Add(gr_MLP);
    mg->Add(gr2_BDT);
    mg->Add(gr_BDT);
    mg->Draw("AL");

    legend->AddEntry("MLP9",Form("MLP 9 variables (AUC=%.3f)",areaMLP), "l");
    legend->AddEntry("BDT9",Form("BDT 9 variables (AUC=%.3f)",areaBDT), "l");
    legend->AddEntry("MLP6",Form("MLP 6 variables (AUC=%.3f)",areaMLP2),"l");
    legend->AddEntry("BDT6",Form("BDT 6 variables (AUC=%.3f)",areaBDT2),"l");
    legend->Draw();

    TString outname = save_dir + "ROC_" + name + ".pdf";
    cout<<"Saving in "<<outname<<endl;
    c1->SaveAs(outname);

    TString outroot = outname;
    outroot.ReplaceAll(".pdf", ".root");
    TFile *fout = new TFile(outroot, "RECREATE");
    gr_MLP->Write("MLP6");
    gr2_MLP->Write("MLP9");
    gr_BDT->Write("BDT6");
    gr2_BDT->Write("BDT9");
    gr_6BDT_data->Write("BDT6_data");
    gr_9BDT_data->Write("BDT9_data");
    mg->Write("MultiGraph");
    c1->Write("Canvas");
    fout->Close();
    delete fout;
}

int plot(){
    TString model="BDT";
    Float_t cut=(0.0);//5336
    TString version="F18in";
    TString lepton="positives";
    TString lep="pos";
    TString mod="_modFC";
    TString outfile=version+"pos"+mod;
    TString outfile_fig5=version+"pos";
    TString file9="Files/9-BDT/"+version+"_"+lepton+"_Final";
    TString file6="Files/6-BDT/"+version+"_"+lepton+"_Final";
    TString file_data="Files/Result_v2/"+lep+"_"+version+"_BDT"+mod;
    TString file_bkg="Files/Result/B_posit_"+version+"_9_BDT";

    cout<<"File: "<<file9<<endl;
    //Variable_Plots_TMVA(file9,model,cut, outfile);
    Variable_Plots_TMVA_Article(file9,model,cut, outfile_fig5);
    Variable_Plots_DataMC_Article(file9,file_data,file_bkg,model,cut, outfile);
    //True_False(file9,model,cut, outfile);
    //ROC(file);
    //ROC_All(file6,file9,version);
    ROC_All_Article(file6,file9,version);
    
    gApplication->Terminate();
    return 0;
}
