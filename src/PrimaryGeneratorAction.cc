//
// ============================================================================
// PrimaryGeneratorAction Implementation
// Main entry point for primary particle generation pipeline
// ============================================================================
//
#include "PrimaryGeneratorAction.hh"
#include "PrimaryGenerator.hh"

//============================================================================
// Constructor: Initialize generator and apply command-line configuration
//============================================================================
// Parameters:
//   inputFileFromCommandLine - Path from --input-file command line argument
//                              Empty string if not provided on command line
// 
// If a file path is provided from command line:
//   - It is immediately locked in PrimaryGenerator
//   - This prevents macro commands from override (ensures reproducibility)
PrimaryGeneratorAction::PrimaryGeneratorAction(const G4String& inputFileFromCommandLine)
: G4VUserPrimaryGeneratorAction(),fPrimaryGenerator(0)
  
{
  // Create primary generator instance
  fPrimaryGenerator = new PrimaryGenerator();

  // If user provided --input-file on command line, configure it now
  // This also sets the lock to prevent macro override
  if (!inputFileFromCommandLine.empty()) {
    fPrimaryGenerator->SetInputFilePathFromCommandLine(inputFileFromCommandLine);
  }

}

//============================================================================
// Destructor: Clean up generator
//============================================================================
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fPrimaryGenerator;
}

//============================================================================
// GeneratePrimaries - Called by Geant4 for each event
//============================================================================
// Delegates to the actual vertex generation in PrimaryGenerator
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){

  // Call the file reader and vertex generator
  fPrimaryGenerator->GeneratePrimaryVertex(anEvent);	



}


