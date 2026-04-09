//
// ********************************************************************
// 1.00 IS, FRM2  
// ********************************************************************
//
#include "PrimaryGenerator.hh"
#include "PrimaryGeneratorMessenger.hh"
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
  fprimaries(0),
  fInputFilePath(""),
  fInputFileLockedByCommandLine(false),
  fMessenger(0)
{
  fMessenger = new PrimaryGeneratorMessenger(this);
}

PrimaryGenerator::~PrimaryGenerator()
{
  delete fMessenger;
}

void PrimaryGenerator::SetInputFilePath(const G4String& filePath)
{
  if (fInputFileLockedByCommandLine) {
    G4cout << "Primary input file is locked by command line override: "
           << fInputFilePath << G4endl;
    return;
  }

  fInputFilePath = filePath;
  G4cout << "Primary input file set from macro/UI command: "
         << fInputFilePath << G4endl;
}

void PrimaryGenerator::SetInputFilePathFromCommandLine(const G4String& filePath)
{
  fInputFilePath = filePath;
  fInputFileLockedByCommandLine = true;

  G4cout << "Primary input file set from command line: "
         << fInputFilePath << G4endl;
}


void PrimaryGenerator::GeneratePrimaryVertex(G4Event* event){

  G4double sample_shift = 0*cm;   // this is for the shift of the sample on the beam axis [...] 
  
  unsigned int il=0;  // number of lines
  
  char aa[10],bb[10],cc[13],dd[10],ee[9],ff[9],gg[10],hh[10],kk[10];
  
  G4double data[9];

  FILE* input_file = 0;

  if (!fInputFilePath.empty()) {
    input_file = fopen(fInputFilePath.c_str(),"r");
  }
  
  char line[128];

  if (input_file != NULL)  // if open successfully

  {
	  
	  G4cout<<"Using primary input file: "<<fInputFilePath<<G4endl;
 	  G4cout<<"input file successfully opened, start reading data...."<<G4endl;
	  G4cout<<" "<<G4endl;
   
	       
  	   while (fgets(line,128,input_file) != NULL)
  	   
  	   {
  	      // Skip header lines and empty lines
  	      if (line[0] == '#' || line[0] == '\n' || line[0] == '\0') {
  	        continue;
  	      }
  	      
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
    if (fInputFilePath.empty()) {
      G4cout<<"No input file was configured."<<G4endl;
      G4cout<<"Set it via macro command /powtex/source/inputFile <path> "
            <<"or pass --input-file <path> on the command line."<<G4endl;
    } else {
      G4cout<<"Configured path: "<<fInputFilePath<<G4endl;
    }
  }
 
  
}

