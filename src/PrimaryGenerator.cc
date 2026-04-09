//
// ============================================================================
// PrimaryGenerator Implementation
// Reads neutron event data from VITESS-generated file and creates G4 vertices
// ============================================================================
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

//============================================================================
// Constructor: Initialize primary generator and create messenger interface
//============================================================================
PrimaryGenerator::PrimaryGenerator()
: G4VPrimaryGenerator(),
  fprimaries(0),                        // Counter for generated neutrons
  fInputFilePath(""),                   // Path to VITESS input file
  fInputFileLockedByCommandLine(false), // Flag for command-line lock
  fMessenger(0)                         // UI messenger not yet created
{
  // Create the UI messenger that handles /powtex/source/ commands
  fMessenger = new PrimaryGeneratorMessenger(this);
}

//============================================================================
// Destructor: Clean up messenger
//============================================================================
PrimaryGenerator::~PrimaryGenerator()
{
  delete fMessenger;
}

//============================================================================
// SetInputFilePath - Set input file from macro commands (respects lock)
//============================================================================
// This is called by the PrimaryGeneratorMessenger when user enters:
//   /powtex/source/inputFile <path>
// If command line already set a file, this call is ignored (file is locked)
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

//============================================================================
// SetInputFilePathFromCommandLine - Set input file from command line (locks)
//============================================================================
// This is called from PrimaryGeneratorAction when user provides --input-file
// Sets fInputFileLockedByCommandLine to prevent macro override
void PrimaryGenerator::SetInputFilePathFromCommandLine(const G4String& filePath)
{
  fInputFilePath = filePath;
  fInputFileLockedByCommandLine = true;

  G4cout << "Primary input file set from command line: "
         << fInputFilePath << G4endl;
}


//============================================================================
// GeneratePrimaryVertex - Main method called once per G4 event
//============================================================================
// Reads one line from the VITESS input file and creates a neutron vertex.
// INPUT FILE FORMAT (9 columns, space-separated):
//   1. ToF (time-of-flight) in milliseconds
//   2. Lambda (wavelength) in Angstroms  
//   3. Weight (statistical weight for this event)
//   4. PosX - position X (sample coordinates, swapped to Z in Geant4)
//   5. PosY - position Y
//   6. PosZ - position Z (beam axis, swapped to X in Geant4)
//   7. DirX - direction X (unit vector)
//   8. DirY - direction Y (unit vector)
//   9. DirZ - direction Z (unit vector)
// Note: VITESS uses (X,Y,Z) with X as beam axis; Geant4 uses Z as beam axis
void PrimaryGenerator::GeneratePrimaryVertex(G4Event* event){

  // Sample position shift along beam axis (for aligning sample in apparatus)
  G4double sample_shift = 0*cm;   // Adjustable if sample repositioning needed
  
  unsigned int il=0;  // Line counter for debugging/logging
  
  // Temporary buffers for parsing file columns (space-separated values)
  char aa[10],bb[10],cc[13],dd[10],ee[9],ff[9],gg[10],hh[10],kk[10];
  
  // Array to store parsed neutron event data
  G4double data[9];

  FILE* input_file = 0;

  // Open VITESS input file if path is configured
  if (!fInputFilePath.empty()) {
    input_file = fopen(fInputFilePath.c_str(),"r");
  }
  
  char line[128];

  //========================================================================
  // FILE READING SECTION
  //========================================================================
  if (input_file != NULL)  // Successfully opened input file
  {
	  
	  G4cout<<"Using primary input file: "<<fInputFilePath<<G4endl;
 	  G4cout<<"input file successfully opened, start reading data...."<<G4endl;
	  G4cout<<" "<<G4endl;
   
	  // Read file line-by-line until end
  	   while (fgets(line,128,input_file) != NULL)
  	   
  	   {
  	      // Skip header lines (start with '#') and empty lines
  	      if (line[0] == '#' || line[0] == '\n' || line[0] == '\0') {
  	        continue;
  	      }
  	      
  	      // Parse 9 space-separated values from current line
		  	  sscanf(line,"%s %s %s %s %s %s %s %s %s",aa,bb,cc,dd,ee,ff,gg,hh,kk);
			
			  // Convert string buffers to doubles and map to data array
			  data[0]=strtod(aa,NULL); // ToF (time-of-flight) in milliseconds	  
			  data[1]=strtod(bb,NULL); // Lambda (wavelength) in Angstroms
			  data[2]=strtod(cc,NULL); // Weight (count rate)
			  data[3]=strtod(dd,NULL); // Position X (VITESS coordinate)
			  data[4]=strtod(ee,NULL); // Position Y (VITESS coordinate)
			  data[5]=strtod(ff,NULL); // Position Z (VITESS coordinate)
			  data[6]=strtod(gg,NULL); // Direction X (unit vector)
			  data[7]=strtod(hh,NULL); // Direction Y (unit vector)
			  data[8]=strtod(kk,NULL); // Direction Z (unit vector)
        il += 1;  // Increment line counter for this event
		  
     		  //====================================================================
     		  // VERTEX CREATION SECTION
     		  //====================================================================
     		  // Get neutron particle definition from Geant4 particle table
 			 G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
			 G4PrimaryParticle* particle = new G4PrimaryParticle(particleDefinition);

			  // COORDINATE TRANSFORMATION:
			  // VITESS uses X as beam axis: (X_vitess, Y_vitess, Z_vitess)
			  // Geant4 uses Z as beam axis: (X_g4, Y_g4, Z_g4)
			  // Mapping: X_g4 = Z_vitess, Y_g4 = Y_vitess, Z_g4 = X_vitess (+ shift)
			  G4ThreeVector positionA(data[5]*cm,data[4]*cm,data[3]*cm + sample_shift);

			  // Create primary vertex at specified position and time-of-flight
			  G4PrimaryVertex* vertex = new G4PrimaryVertex(positionA,data[0]*millisecond);
			  // Alternative: G4PrimaryVertex* vertex = new G4PrimaryVertex(positionA,0);
  
        // KINETIC ENERGY & MOMENTUM CALCULATION
        // Get neutron rest mass
        G4double mc = particle->GetMass();
			  
			  // Calculate kinetic energy from wavelength using de Broglie relation:
			  // E = h²/(2*m*λ²) ≈ 0.081805 eV·Å² / λ²
			  // (constant 0.081805 is h²/(2*m_n) in these units)
			  G4double kine = 0.081805*eV/data[1]/data[1];
			  
			  // Calculate total momentum: p = sqrt(E*(E + 2*m*c²))
			  G4double tmom = std::sqrt(kine*(kine+2*mc));
		  
			  // Set momentum using unit direction vector scaled by total momentum
			  // Note: direction components follow coordinate transformation (swap X,Z)
			  particle->SetMomentum(data[8]*tmom,data[7]*tmom,data[6]*tmom);
			  
			  // Attach particle to vertex
			  vertex->SetPrimary(particle);
			  
			  // Set statistical weight for this event
			  vertex->SetWeight(data[2]);
		  
			  // Add vertex to current event
			  event->AddPrimaryVertex(vertex);
			  
			  // Increment counter of generated neutron events
			  IncrementNoPrimaries();
		    
    }
   
    // Close input file
    fclose(input_file);

		  G4cout<<"Closed the file successfully. The file has "<<il<<" entries. "<<G4endl;
      G4cout<<"Total number of primaries generated: "<<GetNoPrimaries()<<G4endl;
 		  G4cout<<" "<<G4endl;
 		  
  } else {
    // INPUT FILE NOT OPENED - ERROR HANDLING
    G4cout<<"ERROR: Unable to open neutron data file! "<<G4endl;
    if (fInputFilePath.empty()) {
      // No file path configured at all
      G4cout<<"No input file was configured."<<G4endl;
      G4cout<<"CONFIGURATION OPTIONS:"<<G4endl;
      G4cout<<"  1. Macro command: /powtex/source/inputFile <path>"<<G4endl;
      G4cout<<"  2. Command line:  --input-file <path>"<<G4endl;
    } else {
      // File path configured but file not found or inaccessible
      G4cout<<"Configured path: "<<fInputFilePath<<G4endl;
      G4cout<<"Please verify the file exists and is readable."<<G4endl;
    }
  }
 
  
}

