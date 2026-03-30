#include "TF1.h"
#include "TH1.h"
#include "TH3.h"
#include "TMath.h"
#include "TSystem.h"
#include "TRandom.h"
#include "TGraph.h"
#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <limits>

using namespace std;

//Usage: .L analysis/CreatePowtexTree.C / CreatePowtexRootFile()
// Run this from the project root directory 

void CreatePowtexRootFile()
{
     TTree* t=new TTree("powtex_ev","powtex_ev");
     
     Double_t posx,posy,posz,energy,weight;
     Int_t ss_no,module,voxel,scopy,seg,coat,step_no,sec_no;
     ULong64_t tof;

     // see below the description of the variables 
     
     t->Branch("tof",&tof,"tof/l");
     t->Branch("step_no",&step_no,"step_no/I");
     t->Branch("coat",&coat,"coat/I");
     t->Branch("ss_no",&ss_no,"ss_no/I");
     t->Branch("sec_no",&sec_no,"sec_no/I");
     t->Branch("module",&module,"module/I");
     t->Branch("seg",&seg,"seg/I");
     t->Branch("voxel",&voxel,"voxel/I");
     t->Branch("scopy",&scopy,"scopy/I");
     t->Branch("energy",&energy,"energy/D");
     t->Branch("weight",&weight,"weight/D");
     t->Branch("posx",&posx,"posx/D");
     t->Branch("posy",&posy,"posy/D");
     t->Branch("posz",&posz,"posz/D");
     
 gSystem->Exec("rm powtex.root");
 TFile *rf=new TFile("powtex.root","new");

	vector<string> row;
	string line,word;

	fstream fin;

     fin.open("myfile_info.txt");  
     
     cout<<"    "<<endl;
     
     cout<<"Reading the data file..."<<endl;
     
      while (!fin.eof())
       {
        
        row.clear();
        
        // read an entire row and store it in a string variable 'line'
        
        getline(fin, line);

       // used for breaking words
        
        stringstream s(line);
        
        // read every column data of a row and store it in a string variable, 'word'
        
        while (getline(s, word, '\t')){
        
        // add all the column data of a row to a vector
       
        	row.push_back(word);
        
		//      cout<<"word ="<<word<<endl;    
        
        	}

	    tof=stol(row[0]);
        step_no=stod(row[1]);           // step number of the track
        coat=stod(row[2]);           // event origin =10 if cathode, =20 if housing 
        ss_no=stod(row[3]);           //  sumo number 
        sec_no=stod(row[4]);         // sector number, needed for HR and SANS
        module=stod(row[5]);           //  module number, anti-clockwise for EndCap and Mantel
        seg=stod(row[6]);     // position of the segment in the module: =4 ==> upper half, =3 ==>lower half
        voxel=stod(row[7]);            // voxel number
        scopy=stod(row[8]);          //   TL=1, TR=2,
        weight=stod(row[9]);       // weight detected neutron, from McStas
	    energy=stod(row[10]);       // energy reaction product
        posx=stod(row[11]);
        posy=stod(row[12]);
        posz=stod(row[13]);

	  //	  cout<<"tof ="<<tof<<"posx ="<<posx<<endl;
		t->Fill();

	
	}
	
        fin.close();
	t->Write();
	
	cout<<"Fill data tree...done"<<endl; 
	cout<<"The data tree has "<<t->GetEntries()<<" entries."<<endl; 
	cout<<"  "<<endl;
	
		 

}
	
	
