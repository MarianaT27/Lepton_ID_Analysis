#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"

using namespace TMVA;

void INFERFUNC()
{
    gROOT->SetBatch(kTRUE);
    TMVA::Tools::Instance();

    std::cout << "\n==> Start TMVAClassificationApplication (timing benchmark: __NAME__, __NVARS__ vars)" << std::endl;

    TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );

    Float_t P, Theta, Phi, PCAL, ECIN, ECOUT, m2PCAL, m2ECIN, m2ECOUT;

__READERVARBLOCK__
    reader->AddVariable( "SFPCAL", &PCAL );
    reader->AddVariable( "SFECIN", &ECIN );
    reader->AddVariable( "SFECOUT", &ECOUT );
    reader->AddVariable( "m2PCAL", &m2PCAL );
    reader->AddVariable( "m2ECIN", &m2ECIN );
    reader->AddVariable( "m2ECOUT", &m2ECOUT );

    TString name = "__NAME__";
    TString out_name = "__NAME___%NVARS%var_infer.root";

    TFile *target = new TFile(out_name, "RECREATE" );
    TTree *resP = new TTree("resP", out_name);
    TTree *resN = new TTree("resN", out_name);

    Float_t P_P, Theta_P, Phi_P, SFPCAL_P, SFECIN_P, SFECOUT_P, m2PCAL_P, m2ECIN_P, m2ECOUT_P;
    Float_t P_N, Theta_N, Phi_N, SFPCAL_N, SFECIN_N, SFECOUT_N, m2PCAL_N, m2ECIN_N, m2ECOUT_N;
    Float_t scoreBDT_P, scoreMLP_P, scoreBDT_N, scoreMLP_N;

    resP->Branch("scoreBDT_P", &scoreBDT_P, "scoreBDT_P/F");
    resP->Branch("scoreMLP_P", &scoreMLP_P, "scoreMLP_P/F");
    resN->Branch("scoreBDT_N", &scoreBDT_N, "scoreBDT_N/F");
    resN->Branch("scoreMLP_N", &scoreMLP_N, "scoreMLP_N/F");

    resP->Branch("P_P", &P_P, "P_P/F");
    resP->Branch("Theta_P", &Theta_P, "Theta_P/F");
    resP->Branch("Phi_P", &Phi_P, "Phi_P/F");
    resP->Branch("SFPCAL_P", &SFPCAL_P, "SFPCAL_P/F");
    resP->Branch("SFECIN_P", &SFECIN_P, "SFECIN_P/F");
    resP->Branch("SFECOUT_P", &SFECOUT_P, "SFECOUT_P/F");
    resP->Branch("m2PCAL_P", &m2PCAL_P, "m2PCAL_P/F");
    resP->Branch("m2ECIN_P", &m2ECIN_P, "m2ECIN_P/F");
    resP->Branch("m2ECOUT_P", &m2ECOUT_P, "m2ECOUT_P/F");

    resN->Branch("P_N", &P_N, "P_N/F");
    resN->Branch("Theta_N", &Theta_N, "Theta_N/F");
    resN->Branch("Phi_N", &Phi_N, "Phi_N/F");
    resN->Branch("SFPCAL_N", &SFPCAL_N, "SFPCAL_N/F");
    resN->Branch("SFECIN_N", &SFECIN_N, "SFECIN_N/F");
    resN->Branch("SFECOUT_N", &SFECOUT_N, "SFECOUT_N/F");
    resN->Branch("m2PCAL_N", &m2PCAL_N, "m2PCAL_N/F");
    resN->Branch("m2ECIN_N", &m2ECIN_N, "m2ECIN_N/F");
    resN->Branch("m2ECOUT_N", &m2ECOUT_N, "m2ECOUT_N/F");

    TString dir    = "__WEIGHTDIR__";
    TString prefix = "TMVAClassification";

    reader->BookMVA( "MLP method", dir + prefix + "_MLP.weights.xml" );
    reader->BookMVA( "BDT method", dir + prefix + "_BDT.weights.xml" );

    TString file1 = "__TRAINDIR__/" + name + "_Lepton.root";
    TFile *input1 = new TFile(file1);

    std::cout << "--- TMVAClassificationApp    : Using input file: " << input1->GetName() << std::endl;
    TTree* theTree = (TTree*)input1->Get("tree");
    theTree->SetBranchAddress( "P", &P );
    theTree->SetBranchAddress( "Theta", &Theta);
    theTree->SetBranchAddress( "Phi", &Phi);
    theTree->SetBranchAddress( "SFPCAL", &PCAL);
    theTree->SetBranchAddress( "SFECIN", &ECIN);
    theTree->SetBranchAddress( "SFECOUT", &ECOUT );
    theTree->SetBranchAddress( "m2PCAL", &m2PCAL);
    theTree->SetBranchAddress( "m2ECIN", &m2ECIN);
    theTree->SetBranchAddress( "m2ECOUT", &m2ECOUT);

    Long64_t nEntries1 = theTree->GetEntries();
    std::cout << "--- Processing: " << nEntries1 << " events" << std::endl;

    TStopwatch timerBDT, timerMLP;
    timerBDT.Reset();
    timerMLP.Reset();

    for (Long64_t ievt=0; ievt < nEntries1; ievt++) {
        theTree->GetEntry(ievt);
        P_P=P; Theta_P=Theta; Phi_P=Phi;
        SFPCAL_P=PCAL; SFECIN_P=ECIN; SFECOUT_P=ECOUT;
        m2PCAL_P=m2PCAL; m2ECIN_P=m2ECIN; m2ECOUT_P=m2ECOUT;

        timerBDT.Start(kFALSE);
        scoreBDT_P = reader->EvaluateMVA("BDT method");
        timerBDT.Stop();

        timerMLP.Start(kFALSE);
        scoreMLP_P = reader->EvaluateMVA("MLP method");
        timerMLP.Stop();

        resP->Fill();
    }
    input1->Close();

    std::cout << "\n==================================================" << std::endl;
    std::cout << "             INFERENCE SPEED REPORT               " << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "Total Events Evaluated: " << nEntries1 << std::endl;
    std::cout << "BDT Total Time: " << timerBDT.RealTime() << " s"
              << " | Per Event: " << (timerBDT.RealTime() / nEntries1) * 1e6 << " µs" << std::endl;
    std::cout << "MLP Total Time: " << timerMLP.RealTime() << " s"
              << " | Per Event: " << (timerMLP.RealTime() / nEntries1) * 1e6 << " µs" << std::endl;
    std::cout << "==================================================\n" << std::endl;

    TString file2 = "__TRAINDIR__/" + name + "_Pion.root";
    TFile *input2 = new TFile(file2);
    TTree* theTree2 = (TTree*)input2->Get("tree");
    theTree2->SetBranchAddress( "P", &P );
    theTree2->SetBranchAddress( "Theta", &Theta);
    theTree2->SetBranchAddress( "Phi", &Phi);
    theTree2->SetBranchAddress( "SFPCAL", &PCAL);
    theTree2->SetBranchAddress( "SFECIN", &ECIN);
    theTree2->SetBranchAddress( "SFECOUT", &ECOUT );
    theTree2->SetBranchAddress( "m2PCAL", &m2PCAL);
    theTree2->SetBranchAddress( "m2ECIN", &m2ECIN);
    theTree2->SetBranchAddress( "m2ECOUT", &m2ECOUT);

    for (Long64_t ievt=0; ievt < theTree2->GetEntries(); ievt++) {
        theTree2->GetEntry(ievt);
        P_N=P; Theta_N=Theta; Phi_N=Phi;
        SFPCAL_N=PCAL; SFECIN_N=ECIN; SFECOUT_N=ECOUT;
        m2PCAL_N=m2PCAL; m2ECIN_N=m2ECIN; m2ECOUT_N=m2ECOUT;

        scoreBDT_N = reader->EvaluateMVA("BDT method");
        scoreMLP_N = reader->EvaluateMVA("MLP method");

        resN->Fill();
    }

    target->Write();
    target->Close();
    delete reader;

    std::cout << "==> TMVAClassificationApplication (timing benchmark) is done!" << std::endl;
}
