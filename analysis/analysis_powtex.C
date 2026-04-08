#include "TF1.h"
#include "TH1.h"
#include "TH3.h"
#include "TMath.h"
#include "TSystem.h"
#include "TRandom.h"
#include "stdio.h"
#include "stdlib.h"
#include "../include/S_for_Mantel.h"
#include "../include/W_for_Mantel.h"
#include "../include/const_for_POWTEX.h"
#include <iostream>

//Usage: .L analysis/analysis_powtex.C / analysis()
// to be used with the powtex.root file created by using CreatePowtexTree.C
// Run this from the project root directory


void analysis()
{


TFile* file = TFile::Open("powtex.root","update");

TTree* tree = (TTree*)file->Get("powtex_ev"); 

const int nevents = tree->GetEntries();

double* vposx=new double[nevents]();
double* vposy=new double[nevents]();
double* vposz=new double[nevents]();
unsigned long int* vtof=new unsigned long int[nevents]();
double* venergy=new double[nevents]();
double* vweight=new double[nevents]();
int* vsn=new int[nevents]();
int* vseg=new int[nevents]();
int* vsumo=new int[nevents]();
int* vsec=new int[nevents]();
int* vvoxel=new int[nevents]();
int* vscopy=new int[nevents]();
int* vwire=new int[nevents]();
int* vstrip=new int[nevents]();
int* vcounter=new int[nevents]();
int* vmodule=new int[nevents]();
int* vmult=new int[nevents]();
int* vcoat=new int[nevents]();

Double_t energy,posx,posy,posz,weight;
Int_t coat,step_no,ss_no,module,seg,sec_no,voxel,scopy,strip;
ULong64_t tof;

//Branches from the powtex.root tree created at the previous step by running CreatePowtexTree.C

tree->SetBranchAddress("tof",&tof);   // time of flight
tree->SetBranchAddress("coat",&coat);  // identifier for the Boron layer
tree->SetBranchAddress("step_no",&step_no);    // some GEANT4 variable
tree->SetBranchAddress("ss_no",&ss_no);     // detector ID. Mantle = 7
tree->SetBranchAddress("sec_no",&sec_no);  // not used here
tree->SetBranchAddress("module",&module);   // module number
tree->SetBranchAddress("seg",&seg);       // segment number
tree->SetBranchAddress("voxel",&voxel);
tree->SetBranchAddress("scopy",&scopy);

tree->SetBranchAddress("energy",&energy);  // energy deposited in gas by the reaction product
tree->SetBranchAddress("posx",&posx);   // x-pos of the detected neutron
tree->SetBranchAddress("posy",&posy);   // y-pos of the detected neutron
tree->SetBranchAddress("posz",&posz);   // z-pos of the detected neutron
tree->SetBranchAddress("weight",&weight);   // weight neutron

if ((TTree*) file->Get("powtex_new"))
	{cout<<"Found an old tree, I am going to delete it!"<<endl; 
	 gDirectory->Delete("powtex_new");
	 cout<<"....Done!"<<endl; cout<<" "<<endl; }
	
cout<<"now I am going to create the new tree!"<<endl; 
TTree* newt=new TTree("powtex_new","powtex_new");
cout<<"....Done!"<<endl; 
cout<<" "<<endl; 
     
double nposx,nposy,nposz,nenergy;
int nsegment,nvoxel,factor,segment,wire,nwire,nstrip,ncounter,nmodule,mult,ncopy;
//unsigned long nindex;
UInt_t nindexC,nindexS;
ULong64_t ntof;
int ncoat,nsumo,nstep,nsec;
double rad,twotheta,phi,lambda,nweight;
float d_spacing;

//Branches in the new data tree
     
newt->Branch("ntof",&ntof,"ntof/l");  // tof is in ms
newt->Branch("ncoat",&ncoat,"ncoat/I");  // event origin housing coating or cathode coating
newt->Branch("nsumo",&nsumo,"nsumo/I");  // the detector system identifier, Mantle = 7
newt->Branch("nstep",&nstep,"nstep/I");  // the step number of the neutron detection in B4C
newt->Branch("nsegment",&nsegment,"nsegment/I");  // segment number
newt->Branch("nvoxel",&nvoxel,"nvoxel/I");    // gas voxel number
newt->Branch("nmodule",&nmodule,"nmodule/I");    // module number
newt->Branch("nsec",&nsec,"nsec/I");    //sector number, applicable only for DREAM HR&SANS
newt->Branch("nwire",&nwire,"nwire/I");      // wire number
newt->Branch("nstrip",&nstrip,"nstrip/I");      // strip number
newt->Branch("ncounter",&ncounter,"ncounter/I");  // counter no in segment 1=top, 2=bottom
newt->Branch("nindexS",&nindexS,"nindexS/i");    // voxel identifier
newt->Branch("nindexC",&nindexC,"nindexC/i");    // voxel identifier
newt->Branch("ncopy",&ncopy,"ncopy/I");       // which side of the cathode 
newt->Branch("nenergy",&nenergy,"nenergy/D");  //energy deposited in gas by the reaction product 
newt->Branch("nposx",&nposx,"nposx/D");      // x pos of the detected neutron
newt->Branch("nposy",&nposy,"nposy/D");     // y pos of the detected neutron
newt->Branch("nposz",&nposz,"nposz/D");       // z pos of the detected neutron
newt->Branch("nweight",&nweight,"nweight/D");    //weight neutron (probability)
newt->Branch("lambda",&lambda,"lambda/D");     //calculated wavelength 
newt->Branch("twotheta",&twotheta,"twotheta/D");  //calculated scattering angle theta
newt->Branch("phi",&phi,"phi/D");            // calculated scattering angle phi
  
newt->Branch("rad",&rad,"rad/D");      // distance interaction voxel from the sample
newt->Branch("d_spacing",&d_spacing,"d_spacing/F");    // calculated d_spacing

// mapping root file, containing the mapping tree 

TFile* ffile = TFile::Open("powtex_new_cal.root","recreate");

if ((TTree*) ffile->Get("powtex_new_cal"))
	{cout<<"Found an old cal tree, I am going to delete it!"<<endl; 
	 gDirectory->Delete("powtex_new_cal");
	 cout<<"....Done!"<<endl; cout<<" "<<endl; }
	
cout<<"now I am going to create the new calibration tree!"<<endl; 
TTree* fnewt=new TTree("powtex_new_cal","powtex_new_cal");
cout<<"....Done!"<<endl; 
cout<<" "<<endl; 

double rad_c,lambda_c,twotheta_c,phi_c,cweight;
ULong64_t ntof_c,ntof_t;
double nvoxel_x,nvoxel_y,nvoxel_z,nangle;
int mm,nc;
UInt_t nikS,nikC;
float d_spacing_c;

fnewt->Branch("nikS",&nikS,"nikS/i");    //index1 for the mapping
fnewt->Branch("nikC",&nikC,"nikC/i");    //index2 for the mapping
fnewt->Branch("nc",&nc,"nc/I");       // counter number (1 for left of common cathode, 2 for right)
fnewt->Branch("cweight",&cweight,"cweight/D");    // weight neutron
fnewt->Branch("nvoxel_x",&nvoxel_x,"nvoxel_x/D");  // x position of voxel centre (=wire position in the real detector)
fnewt->Branch("nvoxel_y",&nvoxel_y,"nvoxel_y/D");   // y position of voxel centre (=wire position in the real detector)
fnewt->Branch("nvoxel_z",&nvoxel_z,"nvoxel_z/D");   // z position of voxel centre (=wire position in the real detector)
fnewt->Branch("twotheta_c",&twotheta_c,"twotheta_c/D");  // theta angle of the voxel center
fnewt->Branch("nangle",&nangle,"nangle/D");   // inclination angle of the Boron layer at the interaction point
fnewt->Branch("phi_c",&phi_c,"phi_c/D");    // phi angle of the voxel center
fnewt->Branch("rad_c",&rad_c,"rad_c/D");
fnewt->Branch("ntof_c",&ntof_c,"ntof_c/l");   //tof calculated wrt the voxel centre
fnewt->Branch("lambda_c",&lambda_c,"lambda_c/D");  //lambda calculated wrt the voxel centre
fnewt->Branch("d_spacing_c",&d_spacing_c,"d_spacing_c/F");  //d_spacing calculated wrt the voxel centre


// loop through the original events in the powtex.root tree

for (Long64_t i=0; i<nevents; i++){

  	tree->GetEntry(i);
		
	    vtof[i] = tof;
        vcoat[i] = coat;     // event origin, housing or cathode coatings
        vsumo[i] = ss_no;     // sumo number
        vsn[i] = step_no;
        vseg[i] = seg;     // segment number
        vsec[i] = sec_no;     // sector  number HR&SANS
        vvoxel[i] = voxel;       // gas voxel number. It will be used later to extract the wire and strip IDs
        vscopy[i] = scopy;   // index for top and bottom counters. Used to extract the counter ID
        vmodule[i] = module;   // index for the 12-deg sector number
	
        venergy[i] = energy;
        vweight[i] = weight;
        vposx[i] = posx;
        vposy[i] = posy;
        vposz[i] = posz;
        
        if (ss_no == 7){      // Mantle modules
         	wire = calculateW_for_Mantel(voxel,scopy);
        	strip = calculateS_for_Mantel(voxel,scopy);
        	
 		    if (scopy==1 || scopy==2) {vcounter[i] = 1;}
        	
   	        if (scopy==3 || scopy==4) {vcounter[i] = 2;}
   	        
			}
			
			
    	vwire[i] = wire;
		vstrip[i] = strip;
}

// this loop removes the double counting of detected neutrons

  for (Long64_t i=0; i<nevents-1; i++){
  
 	if ((vmodule[i] == vmodule[i+1]) && (vseg[i] == vseg[i+1]) && (vcounter[i] == vcounter[i+1])) {

  		if (vtof[i] >= vtof[i+1] + 10 || vtof[i] <= vtof[i+1] - 10 ) venergy[i+1] = 0;	
							
		}
		
		}
	  

file->cd();

// save the data in the new tree called powtex_new
	
  for (Long64_t i=0; i<nevents; i++){
  
  	if (venergy[i] != 0){
	
        ntof = vtof[i];   // tof neutron in ms
		ncoat = vcoat[i];  // event origin, =10 is cathode, =20 if housing
		nsumo = vsumo[i];  // detector ID
		nsegment = vseg[i];       // detector segment
		nmodule = vmodule[i];    // module number 
		nsec = vsec[i];    // sector number for HR&SANS
		nstep = vsn[i];            // step number of the detection in B4C
		nvoxel = vvoxel[i];     // original voxel number
		nweight = vweight[i];
		nposx = vposx[i];
		nposy = vposy[i];
		nposz = vposz[i];
		nwire = vwire[i];     // wire number
		nstrip = vstrip[i];   // strip number
		ncounter = vcounter[i];  // counter no in segment: 1 is left, 2 is right
		ncopy = vscopy[i];  // same as ncounter
	       
		nindexS = nsumo*100000+nsec*10000+nmodule*100+ncounter;
		nindexC = nsegment*100000+nstrip*100+nwire;
		
        		
		Double_t std_fwhm = venergy[i]*0.10/2.35;
        Double_t rnd = gRandom->Gaus(venergy[i],std_fwhm);
		nenergy = rnd;  // energy deposited in gas, randomized
//		nenergy = venergy[i];  // energy deposited in gas, not randomized
		
	    rad = sqrt(nposx*nposx+nposy*nposy+nposz*nposz);  // distance voxel from the sample in mm
		
        twotheta = 180 - acos(-nposz/rad)*180./pi; // 2theta angle
    	phi = 2*atan(nposx/rad)*180/pi; //  phi angle
	    lambda = hp*ntof/1e+6/mn/(41410+rad)*m2a;  // Vitess file
	    
	    d_spacing = lambda/2/sin(twotheta/2*pi/180.);
		
		newt->Fill();
		
	}
	
		
//	cout<<"ncopy, factor, wire, segment "<<ncopy<<", "<<factor<<", "<<wire<<", "<<segment<<endl;  
	

}

	cout<<"   "<<endl; 
	newt->Write(0,TObject::kOverwrite); //because I don't want to create a new key for the new tree every time I execute the code
	cout<<"Writing the new tree...done"<<endl; 
	
	cout<<"number of entries in the old tree is "<<nevents<<endl; 
	cout<<" "<<endl; 
	
	delete[] vposx;delete[] vposy;delete[] vposz;delete[] vmodule;delete[] vtof;delete[] venergy;delete[] vscopy;
	delete[] vwire; delete[] vstrip; delete[] vcounter; delete[] vseg; delete[] vweight;delete[] vcoat;delete[] vsumo;
	
		 

TFile* lfile = TFile::Open("powtex_lookup.root","read");

TTree* ltree = (TTree*)lfile->Get("lookup_tree"); 

TBranch *nx = ltree->GetBranch("posx");
TLeaf *mx = nx->GetLeaf("posx");
TBranch *ny = ltree->GetBranch("posy");
TLeaf *my = ny->GetLeaf("posy");
TBranch *nz = ltree->GetBranch("posz");
TLeaf *mz = nz->GetLeaf("posz");
TBranch *zz = ltree->GetBranch("counter");
TLeaf *zza = zz->GetLeaf("counter");

//TRandom myrnd(12345); myrnd.Rndm()*0.1 + variable to be randomized

ltree->BuildIndex("indexj","indexi");

int noev = newt->GetEntries();

for (Long64_t k=0; k<noev; k++){
	
        newt->GetEntry(k);
	    
	    nikS = nindexS;  	//	nindexS = nsumo*100000+nmodule*100+ncopy;
	    nikC = nindexC;    //	nindexC = nsegment*100000+nstrip*100+nwire;

	    cweight = nweight;
	    
	    int ev = ltree->GetEntryNumberWithIndex(nikC,nikS);
	    	    
 	    ltree->GetEntry(ev);
		
//	        cout<<"event number in lookup rootfile = "<<ev<<endl;
//	        cout<<"event number in powtex rootfile = "<<k<<endl;
	     
	     ffile->cd();
	                
	     nvoxel_x = mx->GetValue(0);
	     nvoxel_y = my->GetValue(0);
	     nvoxel_z = mz->GetValue(0);	
	     nc = zza->GetValue(0);	
	     	     
	     rad_c = sqrt(nvoxel_x*nvoxel_x+nvoxel_y*nvoxel_y+nvoxel_z*nvoxel_z);  // in mm
	     
         twotheta_c = 180 - acos(-nvoxel_z/rad_c)*180./pi; // this one works for EndCap
    	 phi_c = 2*atan(nvoxel_x/rad_c)*180/pi; // EndCap
	     lambda_c = hp*ntof/1e+6/mn/(41410+rad_c)*m2a; // use this for the Vitess data file

	     d_spacing_c = lambda_c/2/sin(twotheta_c/2*pi/180.);
	     
	     fnewt->Fill();
	     
}
	
	ffile->cd(); 
	fnewt->Write(0,TObject::kOverwrite); //because I don't want to create a new key for the new tree every time I execute the code
	cout<<"Writing the new cal tree...done"<<endl; 
	
	cout<<"number of entries in the new tree is "<<noev<<endl; 
	cout<<" "<<endl; 
	
//  ROOT commands for plots and drawings

// If you want to fit a peak with FitPeaks.C do not use the "hist" option for the spectrum!!!
//otherwise it won't draw the fit function 
// 
//TGeoManager *geom = TGeoManager::Import("hmbGeo.gdml")
//geom->GetTopVolume()->Draw("ogl")
//

//		powtex_new->AddFriend("powtex_new_cal","powtex_new_cal.root")
//      reminder in the ROOT prompt fmod(voxel-4,32) = (voxel - 4) % 32
//      powtex_new->Draw("d_spacing_c>>dspc(3500,0,7)","cweight*(twotheta_c>85 && twotheta_c<95)","ehist")
//      powtex_new->Draw("d_spacing_c>>dspc(3500,0,7)","cweight","HIST")
//      powtex_new->Draw("d_spacing_c>>dspc(3500,0,7)","cweight*(nsumo==3)","ehistsame")
//      powtex_new->Draw("twotheta_c:lambda_c>>test1(1000,0,10,200,0,200)","cweight","colz")
//      powtex_new->Draw("twotheta:lambda>>test(1000,0,10,200,0,200)","nweight","colz")
//      powtex_new->Draw("phi_c:twotheta_c>>tt(180,0,180,180,-90,90)","cweight","colz");
//		biosample:
//      powtex_new->Draw("twotheta:phi>>tt(720,-180,180,360,0,180)","nweight","colz"); 
//		powtex_new->Draw("sqrt(lambda*lambda-2*log(cos(twotheta/2*TMath::Pi()/180))):d_spacing>>tt(5000,0,5,5000,0,5)","nsumo==7 && nmodule==3 ","colz")
//      powtex_new->Draw("ntof>>test(1000,0,100)","cweight","ehist")
//      powtex_new->Draw("lambda>>test(1000,0,10)","cweight","ehist")
//		powtex_new->Draw("nposz:nposx:nposy")
//		powtex_new->Draw("nposx:nposy")
//		powtex_new->Draw("nposx:nposz")
//		powtex_new->Draw("nstrip:nwire")
//		powtex_new->Draw("nposy:nposx")
//		powtex_new->Draw("nvoxel_x:nvoxel_z","","same")
//		powtex_new->Draw("nvoxel_y:nvoxel_z","","same")
//      powtex_new->Draw("phi_c:twotheta_c>>tt(1800,0,180,100,-50,50)","cweight","colz")
//		powtex_new->Draw("phi>>tt(2000,-10,20)","nsumo==5")
//		powtex_new->Draw("lambda_c-lambda>>tt(1000,-0.01,0.01)","cweight")
//      powtex_new->Draw("twotheta_c-twotheta>>tt(1000,-1,1)","cweight")
//      powtex_new->Draw("rad_c-rad>>tt(500,-25,25)","cweight")
//      powtex_new->Draw("sin(twotheta_c/2*3.1415/180)-sin(twotheta/2*3.1415/180)>>tt(2000,-0.01,0.01)","cweight")
//      powtex_new->Draw("d_spacing_c-d_spacing>>tt(2000,-0.05,0.05)","cweight")



}

// Entry point matching this macro filename for ROOT auto-execution.
void analysis_powtex()
{
	analysis();
}
