#include <cstdlib>
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TROOT.h"
#include "TStopwatch.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

using namespace TMVA;

void INFERFUNC()
{
    gROOT->SetBatch(kTRUE);
    TMVA::Tools::Instance();

    std::cout << "\n==> Start TMVAClassificationApplication on real Data (timing benchmark: __NAME__, __NVARS__ vars)" << std::endl;

    TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );

    Float_t P, Theta, Phi, PCAL, ECIN, ECOUT, m2PCAL, m2ECIN, m2ECOUT;

__READERVARBLOCK__
    reader->AddVariable( "SFPCAL", &PCAL );
    reader->AddVariable( "SFECIN", &ECIN );
    reader->AddVariable( "SFECOUT", &ECOUT );
    reader->AddVariable( "m2PCAL", &m2PCAL );
    reader->AddVariable( "m2ECIN", &m2ECIN );
    reader->AddVariable( "m2ECOUT", &m2ECOUT );

    reader->BookMVA( "MLP method", "__MLP_WEIGHTS__" );
    reader->BookMVA( "BDT method", "__BDT_WEIGHTS__" );

    TString file = "__DATAFILE__";
    TFile *input = new TFile(file);
    std::cout << "--- Using Data input file: " << input->GetName() << std::endl;
    TTree* theTree = (TTree*)input->Get("results");

    Double_t P_D, Theta_D, Phi_D, SFPCAL_D, SFECIN_D, SFECOUT_D, m2PCAL_D, m2ECIN_D, m2ECOUT_D;
    theTree->SetBranchAddress( "P_D", &P_D );
    theTree->SetBranchAddress( "Theta_D", &Theta_D );
    theTree->SetBranchAddress( "Phi_D", &Phi_D );
    theTree->SetBranchAddress( "SFPCAL_D", &SFPCAL_D );
    theTree->SetBranchAddress( "SFECIN_D", &SFECIN_D );
    theTree->SetBranchAddress( "SFECOUT_D", &SFECOUT_D );
    theTree->SetBranchAddress( "m2PCAL_D", &m2PCAL_D );
    theTree->SetBranchAddress( "m2ECIN_D", &m2ECIN_D );
    theTree->SetBranchAddress( "m2ECOUT_D", &m2ECOUT_D );

    Long64_t nEntries = theTree->GetEntries();
    std::cout << "--- Processing: " << nEntries << " Data events" << std::endl;

    TStopwatch timerBDT, timerMLP;
    timerBDT.Reset();
    timerMLP.Reset();

    Float_t scoreBDT, scoreMLP;
    for (Long64_t ievt=0; ievt < nEntries; ievt++) {
        theTree->GetEntry(ievt);
        P=P_D; Theta=Theta_D; Phi=Phi_D;
        PCAL=SFPCAL_D; ECIN=SFECIN_D; ECOUT=SFECOUT_D;
        m2PCAL=m2PCAL_D; m2ECIN=m2ECIN_D; m2ECOUT=m2ECOUT_D;

        timerBDT.Start(kFALSE);
        scoreBDT = reader->EvaluateMVA("BDT method");
        timerBDT.Stop();

        timerMLP.Start(kFALSE);
        scoreMLP = reader->EvaluateMVA("MLP method");
        timerMLP.Stop();
    }
    input->Close();

    std::cout << "\n==================================================" << std::endl;
    std::cout << "         INFERENCE SPEED REPORT (Data)           " << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "Total Events Evaluated: " << nEntries << std::endl;
    std::cout << "BDT Total Time: " << timerBDT.RealTime() << " s"
              << " | Per Event: " << (timerBDT.RealTime() / nEntries) * 1e6 << " µs" << std::endl;
    std::cout << "MLP Total Time: " << timerMLP.RealTime() << " s"
              << " | Per Event: " << (timerMLP.RealTime() / nEntries) * 1e6 << " µs" << std::endl;
    std::cout << "==================================================\n" << std::endl;

    delete reader;
    std::cout << "==> Data inference timing benchmark is done!" << std::endl;
}
