//
// ********************************************************************
// 1.00 IS, FRM2  
// ********************************************************************
//
#include "PrimaryGenerator.hh"
#include "G4PrimaryParticle.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4ios.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryVertex.hh"
#include "globals.hh"
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
PrimaryGenerator::PrimaryGenerator()
: G4VPrimaryGenerator(),
  fprimaries(0)
{}

PrimaryGenerator::~PrimaryGenerator()
{}


void PrimaryGenerator::GeneratePrimaryVertex(G4Event* event){

  G4double sample_shift = 0*cm;   // this is for the shift of the sample on the beam axis [...] 
  
  unsigned int il=0;  // number of lines
  
  char aa[10],bb[10],cc[13],dd[10],ee[9],ff[9],gg[10],hh[10],kk[10];
  
  G4double data[9];

  FILE* input_file;

  // Try relative path first (new location), then fall back to old paths
  input_file = fopen("data/input/VitessDataPOWTEX/noutascii_reduced.dat","r");

  /*
  if (input_file == NULL) {
    // Fallback to alternative data file
    input_file = fopen("data/input/VitessDataPOWTEX/noutascii_reduced.dat","r");
  }

  if (input_file == NULL) {
    // Last resort: try old absolute paths for backward compatibility
    input_file = fopen("/Users/irinastefanescu/g4new/ess/POWTEX/VitessDataPOWTEX/noutascii_biosample_r.dat","r");
  }*/
  
  char line[128];

  if (input_file != NULL)  // if open successfully

  {
	  
 	  G4cout<<"input file successfully opened, start reading data...."<<G4endl;
	  G4cout<<" "<<G4endl;
   
	       
  	   while (fgets(line,128,input_file) != NULL)
  	   
  	   {
		  	  sscanf(line,"%s %s %s %s %s %s %s %s %s",aa,bb,cc,dd,ee,ff,gg,hh,kk);
				
			  data[0]=strtod(aa,NULL); // tof in ms	  
			  data[1]=strtod(bb,NULL);	// lambda  
			  data[2]=strtod(cc,NULL);	// weight  
			  data[3]=strtod(dd,NULL);	// posx  
			  data[4]=strtod(ee,NULL);	// posy 
			  data[5]=strtod(ff,NULL);	// posz  
			  data[6]=strtod(gg,NULL);	// dirx
			  data[7]=strtod(hh,NULL);	// diry 
			  data[8]=strtod(kk,NULL);	// dirz  
		  
     		  il += 1;  // line number, useful for debugging
		  
 			 G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
			 G4PrimaryParticle* particle = new G4PrimaryParticle(particleDefinition);

//            swap Z and X because in VITESS the X-axis is the beamaxis

			  G4ThreeVector positionA(data[5]*cm,data[4]*cm,data[3]*cm + sample_shift);

			  G4PrimaryVertex* vertex = new G4PrimaryVertex(positionA,data[0]*millisecond);
//			  G4PrimaryVertex* vertex = new G4PrimaryVertex(positionA,0); // initiate all neutrons at time 0
  
        G4double mc = particle->GetMass();
			  G4double kine = 0.081805*eV/data[1]/data[1]; //calculate kinetic energy as in Geant4
			  G4double tmom = std::sqrt(kine*(kine+2*mc));  // total momentum
		  
			  particle->SetMomentum(data[8]*tmom,data[7]*tmom,data[6]*tmom); 
			  
			  vertex->SetPrimary(particle);
			  vertex->SetWeight(data[2]);
//			  vertex->SetWeight(1);
		  
			  event->AddPrimaryVertex(vertex);
			  IncrementNoPrimaries();
		    
    }
   
    fclose(input_file);


		  G4cout<<"Closed the file successfully. The file has "<<il<<" entries. "<<G4endl;
      G4cout<<"Total number of primaries generated: "<<GetNoPrimaries()<<G4endl;
 		  G4cout<<" "<<G4endl;
 		  
  } else {
    G4cout<<"ERROR: Unable to open neutron data file! "<<G4endl;
    G4cout<<"Searched for: "<<G4endl;
    G4cout<<"  - data/input/VitessDataPOWTEX/noutascii_biosample_r.dat"<<G4endl;
    G4cout<<"  - data/input/VitessDataPOWTEX/noutascii_reduced.dat"<<G4endl;
    G4cout<<"Please ensure the data files exist in data/input/VitessDataPOWTEX/"<<G4endl;
  }
 
  
}

