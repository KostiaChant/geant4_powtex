// 1.00 IS, ESS


#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "SteppingAction.hh"
#include "G4Track.hh"
#include "globals.hh"
#include "G4SteppingManager.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4UnitsTable.hh"

#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

SteppingAction::SteppingAction()
{ }


SteppingAction::~SteppingAction()
{ }

typedef unsigned long long uint64;
uint64 tof;
G4double Ene=0.0,tenergy=0.0,s_time=0.;
G4double pos_x = 0.0,pos_y=0.0,pos_z=0.0,slength=0.;
G4int copyNo=0,copyS=0,dd=0;
G4int kk=0;

// Mantel geometry is organized in 9 imprints per module (0..8 segment ids).
G4int no_segments = 8; 

// Event output stream. One line is appended per accepted gas-boundary hit.
ofstream myfile;
 

void SteppingAction::UserSteppingAction(const G4Step* fStep) 
{ 


  G4int prec = G4cout.precision(5); // precision of cout

  G4Track* fTrack = fStep->GetTrack();

	G4StepPoint* prepoint = fStep->GetPreStepPoint();   // Boundary point.
	G4StepPoint* postpoint = fStep->GetPostStepPoint(); // First point in the next volume.

  G4String proc = postpoint->GetProcessDefinedStep()->GetProcessName();  
  
  G4String name = fTrack->GetDefinition()->GetParticleName(); 
  G4double charge = fTrack->GetDefinition()->GetPDGCharge(); 
  G4String pname = fTrack->GetDefinition()->GetParticleName(); 
  
  G4String vname; 

  G4int StepNo = fTrack->GetCurrentStepNumber();
  
  G4String str_vol = prepoint->GetTouchableHandle()->GetVolume()->GetName();
  G4String str_vol_n = fTrack->GetVolume()->GetName();

  G4String str_vol_m;
  

 G4int ss_no=0,imp_no=0,vox_no=0,mod_no=0,seg_no=0,sec_no=0;
 
// only interested in the charged particles that reach the gas layer (gas voxel) inside the detector 
 
 if ((name =="alpha" || name == "Li7[0.0]") && int(str_vol.find("GasLayer")) > 0){ 
// if ((charge>0) && int(str_vol.find("GasLayer")) > 0){ 
 	 if (prepoint->GetStepStatus() == fGeomBoundary){

		vname = prepoint->GetPhysicalVolume()->GetLogicalVolume()->GetName();
		
	    str_vol_m = fTrack->GetOriginTouchable()->GetVolume()->GetName();
		
		// Detector-side tag: 10 for cathode converter, 20 for housing converter.
		if (int(str_vol_m.find("Converter Cathode")) > 0) {dd = 10;}
		    else {dd = 20;}
		
		// Parse imprint and voxel ids from volume name tokens.
		G4int pos3 = str_vol.find("_",9);
		G4String imp_no_str2 = str_vol.substr(pos3+1,3);   // imprint is the segment number
		      imp_no = atoi(imp_no_str2);                // counting starts at 2
 				
		G4int pos5 = str_vol.find_last_of("_");
		G4String imp_no_str3 = str_vol.substr(pos5+1,6);   // voxel number
		      vox_no = atoi(imp_no_str3);
								
		if (int(str_vol.find("_M_")) > 0)  // Mantel
		      {  ss_no = 7;
		      	 // Convert linear imprint index into module and segment ids.
		      	 mod_no = abs((imp_no - 1)/(no_segments+1)) + 1;  // module number
				 seg_no = (imp_no - 1) % (no_segments+1);  // segment number
				 sec_no = 1;   // this is actually the module number
				 
				}	
		      		         
		      
		G4cout<<"    "<<G4endl;
		G4cout<<"sector no ="<<sec_no<<G4endl;
		G4cout<<"module no ="<<mod_no<<G4endl;
		G4cout<<"seg no  = "<<seg_no<<G4endl;
		G4cout<<"detector type = "<<ss_no<<G4endl;
		G4cout<<"StepNo = "<<StepNo<<G4endl;
		G4cout<<"voxel no = "<<vox_no<<G4endl;
//		G4cout<<"charge = "<<charge<<G4endl;
//		G4cout<<"particle name = "<<pname<<G4endl;
//		G4cout<<"volume  = "<<int(str_vol.find("GasLayer"))<<G4endl;
		
		tof = fTrack->GetGlobalTime()/ns; 
		s_time = fTrack->GetLocalTime()/ns; 
		//	  v_n = fTrack->GetKineticEnergy()/meV; 

		copyNo = prepoint->GetTouchableHandle()->GetCopyNumber();
 		Ene = prepoint->GetKineticEnergy();
		
		pos_x = fTrack->GetPosition().x();
		pos_y = fTrack->GetPosition().y();
		pos_z = fTrack->GetPosition().z();
         	G4double nw = fTrack->GetWeight(); 
			  
		G4cout<<"tof neutron [ns] = "<<tof<<G4endl;
//		G4cout<<"step time [ns] = "<<s_time<<G4endl;
//		G4cout<<"copy No event ="<<copyNo<<G4endl;
//		G4cout<<"Energy event ="<<Ene<<G4endl;
//		G4cout<<"pos_x event ="<<pos_x<<G4endl;
//		G4cout<<"pos_y event ="<<pos_y<<G4endl;
//		G4cout<<"pos_z event ="<<pos_z<<G4endl;

		        // Map quadrant label to local copy index expected by strip/wire mapping.
		        if (int(vname.find("TL")) > 0) { copyS =1;}
		   else if (int(vname.find("TR")) > 0) { copyS =2;}
		   	else if (int(vname.find("BL")) > 0) { copyS =3;}  // copyS =1;
			   	else if (int(vname.find("BR")) > 0) { copyS =4;}  //copyS =2;
		
//		G4cout<<"copyS ="<<copyS<<G4endl;
		
		myfile.open("myfile_info.txt", ios::out | ios::app);

//		if (nw>0.0000001)
		// Output columns:
		// tof, stepNo, dd, detType, sector, module, segment, voxel, copyS,
		// weight, kineticEnergy, posX, posY, posZ
		myfile << tof<<"\t"<<StepNo<<"\t"<<dd<<"\t"<<ss_no<<"\t"<<sec_no<<"\t"<<mod_no<<"\t"<<seg_no<<"\t"<<vox_no<<"\t"<<copyS<<"\t"<<nw<<"\t"<<Ene<<"\t"<<pos_x<<"\t"<<pos_y<<"\t"<<pos_z<<endl;
			   
			   
	}
			  
 }
  	  

 	   
//  trick to avoid the infinite looping 	 

    if (fTrack->GetCurrentStepNumber() > 500){
    
      fTrack->SetTrackStatus(fKillTrackAndSecondaries);
      // inform end of tracking to the physics processes 
      fTrack->GetDefinition()->GetProcessManager()->EndTracking();
      }
      
      
 G4cout.precision(prec);  

 myfile.close();
    
}

