#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include <fstream>

using namespace std;

void DumpTreeToTXT(){

  gSystem->Exec("rm data_powtex_biosample.csv");

  TFile *f=new TFile("powtex.root"); // opens the root file
  TTree *tr=(TTree*)f->Get("powtex_new"); // creates the TTree object
  tr->AddFriend("powtex_new_cal","powtex_new_cal.root");
//  tr->Scan("ntof"); // prints the content on the screen

  int nmodule,nstrip,nwire,nsegment,ncounter; // create variables of the same type as the branches you want to access
  double nposx,nposy,nposz,nvoxel_x,nvoxel_y,nvoxel_z,lambda;
  ULong64_t ntof;

  tr->SetBranchAddress("nmodule",&nmodule); 
  tr->SetBranchAddress("nsegment",&nsegment); 
  tr->SetBranchAddress("nstrip",&nstrip); 
  tr->SetBranchAddress("nwire",&nwire); 
  tr->SetBranchAddress("ntof",&ntof); 
  tr->SetBranchAddress("ncounter",&ncounter); 
  tr->SetBranchAddress("lambda",&lambda); 
  tr->SetBranchAddress("nposx",&nposx); 
  tr->SetBranchAddress("nposy",&nposy); 
  tr->SetBranchAddress("nposz",&nposz); 
  tr->SetBranchAddress("nvoxel_x",&nvoxel_x); 
  tr->SetBranchAddress("nvoxel_y",&nvoxel_y); 
  tr->SetBranchAddress("nvoxel_z",&nvoxel_z); 

  ofstream myfile;
  myfile.open ("data_powtex_biosample.csv");
  myfile<<"tof [ns]"<<"\t"<<"lambda [A]"<<"\t"<<"module"<<"\t"<<"segment"<<"\t"<<"counter"<<"\t"<<"wire"<<"\t"<<"strip"<<"\t"<<"x_pos [mm]"<<"\t"<<"y_pos [mm]"<<"\t"<<"z_pos [mm]"<<"\t"<<"voxel_x [mm]"<<"\t"<<"voxel_y [mm]"<<"\t"<<"voxel_z [mm]\n";
  
  for (int i=0;i<tr->GetEntries();i++){
    // loop over the tree
    tr->GetEntry(i);
    
//    if (sumo==7 && module<=10 && wire==16 && strip==1)
        
  	myfile<<ntof<<"\t"<<lambda<<"\t"<<nmodule<<"\t"<<nsegment<<"\t"<<ncounter<<"\t"<<nwire<<"\t"<<nstrip<<"\t"<<nposx<<"\t"<<nposy<<"\t"<<nposz<<"\t"<<nvoxel_x<<"\t"<<nvoxel_y<<"\t"<<nvoxel_z<<"\n";

  }
  

  myfile.close();
}