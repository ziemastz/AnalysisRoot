#include <iostream>
#include <vector>
#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TRandom.h"
#include "TTreeIndex.h"

using namespace std;
using namespace ROOT;

int main(int argc, char **argv)
{
    TString filename = "TDCR_OUT.root";
    if(argc == 2) {
        filename = argv[1];
    }
    
    FILE* reportFile = fopen("Report.txt","w");
    FILE* energyFile = fopen("ENERGY.TAB","w");
    TFile file(filename);
    Int_t eventID;
    Int_t trackID;
    Int_t parrentID;
    Double_t edep_cocktail = 0.;
    Double_t p_edep_cocktail = 0.;
    Double_t s_edep_cocktail = 0.;
    Double_t edep_vial = 0.;
    Double_t edep_cup = 0.;
    Double_t tot_edep_cocktail = 0.;
    Double_t tot_edep_vial = 0.;
    Double_t tot_edep_cup = 0.;
    
    TTree *HITS = (TTree*)file.Get("Cocktail");
    TTree *vialHITS = (TTree*)file.Get("Vial");
    TTree *cupHITS = (TTree*)file.Get("Cup");
    // Get data from from HITS tree
    HITS->SetBranchAddress("EventID",&eventID);		                    // event ID ==> entry
    HITS->SetBranchAddress("TrackID",&trackID);
    HITS->SetBranchAddress("ParrentID",&parrentID);
    HITS->SetBranchAddress("Edep",&edep_cocktail);
    HITS->BuildIndex("EventID");
    TTreeIndex*index = (TTreeIndex*)HITS->GetTreeIndex();

    Int_t nevent = static_cast<Int_t>(HITS->GetEntries());
    
    fprintf(reportFile,"NEVENT = %i\n",nevent);
    
    int current_eventID = -1;
    int id = 0;
    int p_c = 0;
    int s_c = 0;
    for (  int i = 0; i <index->GetN(); i++ )
    {
        Long64_t local = HITS->LoadTree(index->GetIndex()[i]);
        HITS->GetEvent(local);
        if(current_eventID != eventID) {
            id++;
            fprintf(energyFile,"\n%i",id);
            current_eventID = eventID;
        }
        //if(edep_cocktail*1000 > 0.06)
            fprintf(energyFile,"\t%f",edep_cocktail*1000);
        //total deposited energy in cocktail
        tot_edep_cocktail+=edep_cocktail;
        if(parrentID == 0) {
            p_edep_cocktail += edep_cocktail;
            p_c++;
        }else {
            s_edep_cocktail += edep_cocktail;
            s_c++;
        }
    }
    fprintf(reportFile,"Decay event in cocktail = %i\n",id);
    fprintf(reportFile,"Total deposited energy in cocktail = %f keV\n",tot_edep_cocktail*1000);
    fprintf(reportFile,"Number of primary particles in cocktail = %i\n",p_c);
    fprintf(reportFile,"Deposited energy in cocktail 1st = %f keV\n",p_edep_cocktail*1000);
    fprintf(reportFile,"Number of secendary particles in cocktail = %i\n",s_c);
    fprintf(reportFile,"Deposited energy in cocktail 2nd = %f keV\n",s_edep_cocktail*1000);
     
     if(true) {
        vialHITS->SetBranchAddress("EventID",&eventID);		                    // event ID ==> entry
    vialHITS->SetBranchAddress("Edep",&edep_vial);
    vialHITS->BuildIndex("EventID");
    
    nevent = static_cast<Int_t>(vialHITS->GetEntries());
    fprintf(reportFile,"\nNEVENT = %i\n",nevent);
    if(nevent > 0) {
        index = (TTreeIndex*)vialHITS->GetTreeIndex();
    current_eventID = -1;
    id = 0;
    for (  int i = 0; i <index->GetN(); i++ )
    {
        Long64_t local = vialHITS->LoadTree(index->GetIndex()[i]);
        vialHITS->GetEvent(local);
        if(current_eventID != eventID) {
            id++;
            current_eventID = eventID;
        }
        
        //total deposited energy in vial
        tot_edep_vial += edep_vial;
    }
    fprintf(reportFile,"Decay event in vial = %i\n",id);
    fprintf(reportFile,"Total deposited energy in vial = %f keV\n",tot_edep_vial*1000);
    }
    

    cupHITS->SetBranchAddress("EventID",&eventID);		                    // event ID ==> entry
    cupHITS->SetBranchAddress("Edep",&edep_cup);
    cupHITS->BuildIndex("EventID");

    
    nevent = static_cast<Int_t>(cupHITS->GetEntries());
    fprintf(reportFile,"\nNEVENT = %i\n",nevent);
    if(nevent > 0) {
         index = (TTreeIndex*)cupHITS->GetTreeIndex();
    current_eventID = -1;
    id = 0;
    for (  int i = 0; i <index->GetN(); i++ )
    {
        Long64_t local = cupHITS->LoadTree(index->GetIndex()[i]);
        cupHITS->GetEvent(local);
        if(current_eventID != eventID) {
            id++;
            current_eventID = eventID;
        }
        
        //total deposited energy in vial
        tot_edep_cup += edep_cup;
    }
    fprintf(reportFile,"Decay event in cup = %i\n",id);
    fprintf(reportFile,"Total deposited energy in cup = %f keV\n",tot_edep_cup*1000);
    }
   
    }
    

    fclose(reportFile);
    delete reportFile;
    fclose(energyFile);
    delete energyFile;

    return 0;
}