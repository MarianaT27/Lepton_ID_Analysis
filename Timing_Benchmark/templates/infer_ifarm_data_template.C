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

// Reads the real-Data production trees made by toroot_v2.C (tree "analysis",
// Double_t branches "<species>_p/theta/phi/sfpcal/sfecin/sfecout/m2pcal/m2ecin/m2ecout",
// species = "positron" for the *_positives models, "electron" for the *_negatives models).
// NOTE: toroot_v2.C computes theta/phi in DEGREES (Theta()*57.2958) — if the training
// samples used radians, scores from this benchmark won't be physically meaningful, but
// that doesn't affect the timing measurement (same fixed amount of work either way).
void INFERFUNC()
{
    gROOT->SetBatch(kTRUE);
    TMVA::Tools::Instance();

    std::cout << "\n==> Start TMVAClassificationApplication on ifarm real Data (timing benchmark: __NAME__, __NVARS__ vars, species=__SPECIES__)" << std::endl;

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
    std::cout << "--- Using ifarm Data input file: " << input->GetName() << std::endl;
    TTree* theTree = (TTree*)input->Get("analysis");

    Double_t d_P, d_Theta, d_Phi, d_SFPCAL, d_SFECIN, d_SFECOUT, d_m2PCAL, d_m2ECIN, d_m2ECOUT;
    theTree->SetBranchAddress( "__SPECIES___p", &d_P );
    theTree->SetBranchAddress( "__SPECIES___theta", &d_Theta );
    theTree->SetBranchAddress( "__SPECIES___phi", &d_Phi );
    theTree->SetBranchAddress( "__SPECIES___sfpcal", &d_SFPCAL );
    theTree->SetBranchAddress( "__SPECIES___sfecin", &d_SFECIN );
    theTree->SetBranchAddress( "__SPECIES___sfecout", &d_SFECOUT );
    theTree->SetBranchAddress( "__SPECIES___m2pcal", &d_m2PCAL );
    theTree->SetBranchAddress( "__SPECIES___m2ecin", &d_m2ECIN );
    theTree->SetBranchAddress( "__SPECIES___m2ecout", &d_m2ECOUT );

    Long64_t nEntries = theTree->GetEntries();
    std::cout << "--- Processing: " << nEntries << " Data events" << std::endl;

    TStopwatch timerBDT, timerMLP;
    timerBDT.Reset();
    timerMLP.Reset();

    Float_t scoreBDT, scoreMLP;
    for (Long64_t ievt=0; ievt < nEntries; ievt++) {
        theTree->GetEntry(ievt);
        P=d_P; Theta=d_Theta; Phi=d_Phi;
        PCAL=d_SFPCAL; ECIN=d_SFECIN; ECOUT=d_SFECOUT;
        m2PCAL=d_m2PCAL; m2ECIN=d_m2ECIN; m2ECOUT=d_m2ECOUT;

        timerBDT.Start(kFALSE);
        scoreBDT = reader->EvaluateMVA("BDT method");
        timerBDT.Stop();

        timerMLP.Start(kFALSE);
        scoreMLP = reader->EvaluateMVA("MLP method");
        timerMLP.Stop();
    }
    input->Close();

    std::cout << "\n==================================================" << std::endl;
    std::cout << "      INFERENCE SPEED REPORT (ifarm Data)        " << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "Total Events Evaluated: " << nEntries << std::endl;
    std::cout << "BDT Total Time: " << timerBDT.RealTime() << " s"
              << " | Per Event: " << (timerBDT.RealTime() / nEntries) * 1e6 << " µs" << std::endl;
    std::cout << "MLP Total Time: " << timerMLP.RealTime() << " s"
              << " | Per Event: " << (timerMLP.RealTime() / nEntries) * 1e6 << " µs" << std::endl;
    std::cout << "==================================================\n" << std::endl;

    delete reader;
    std::cout << "==> ifarm Data inference timing benchmark is done!" << std::endl;
}
