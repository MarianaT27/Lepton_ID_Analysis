#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/MethodCategory.h"
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"

int TRAINFUNC()
{
   gROOT->SetBatch(kTRUE);
   TMVA::Tools::Instance();

   std::cout << std::endl;
   std::cout << "==> Start TMVAClassification (timing benchmark: __NAME__, __NVARS__ vars)" << std::endl;

   TString name="__NAME__";

   TFile *input1 = new TFile("__VARDIR__/"+name+"_Lepton.root");
   TFile *input2 = new TFile("__VARDIR__/"+name+"_Pion.root");

   TTree *signalTree     = (TTree*)input1->Get("tree");
   TTree *background     = (TTree*)input2->Get("tree");

   TString outfileName( "__NAME___%NVARS%var_TMVA.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );

   TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset_timing___NAME_____NVARS__var");

__VARBLOCK__

   Double_t signalWeight     = 1.0;
   Double_t backgroundWeight = 1.0;

   dataloader->AddSignalTree    ( signalTree,     signalWeight );
   dataloader->AddBackgroundTree( background, backgroundWeight );

   dataloader->PrepareTrainingAndTestTree( "", "",
   					"SplitMode=Random:NormMode=NumEvents:!V" );

   factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLP", "!H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator" );

   factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

   factory->TrainAllMethods();

   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAClassification (timing benchmark) is done!" << std::endl;

   delete factory;
   delete dataloader;

   return 0;
}
