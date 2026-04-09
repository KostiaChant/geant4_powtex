//
// ============================================================================
// PrimaryGeneratorMessenger Implementation
// Provides macro command interface for primary generator configuration
// ============================================================================
// USAGE IN MACRO FILES:
//   /powtex/source/inputFile /path/to/vitess/data.txt
//
// This creates a hierarchical command structure that can be used in:
//   - Macro files (.mac)
//   - Interactive G4 session (g4_prompt >)
//

#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGenerator.hh"
#include "G4ApplicationState.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"

//============================================================================
// Constructor: Create UI directory structure and commands
//============================================================================
PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGenerator* primaryGenerator)
: G4UImessenger(),
  fPrimaryGenerator(primaryGenerator),  // Store pointer to generator
  fPowtexDirectory(0),                   // Will create /powtex/ directory
  fSourceDirectory(0),                   // Will create /powtex/source/ directory
  fInputFileCmd(0)                       // Will create inputFile command
{
  // Create top-level POWTEX directory for all simulation controls
  fPowtexDirectory = new G4UIdirectory("/powtex/");
  fPowtexDirectory->SetGuidance("POWTEX simulation control commands.");

  // Create subdirectory for primary source configuration
  fSourceDirectory = new G4UIdirectory("/powtex/source/");
  fSourceDirectory->SetGuidance("Primary source controls and configuration.");

  // Create command to set VITESS input file path
  // Accessible in PreInit and Idle states (before and during initialization)
  fInputFileCmd = new G4UIcmdWithAString("/powtex/source/inputFile", this);
  fInputFileCmd->SetGuidance(
    "Set path to VITESS primary input file containing neutron events.\n"
    "File format: 9 columns (ToF, lambda, weight, pos_x, pos_y, pos_z, dir_x, dir_y, dir_z)"
  );
  fInputFileCmd->SetParameterName("filePath", false);  // Parameter is mandatory
  fInputFileCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//============================================================================
// Destructor: Clean up UI commands and directories
//============================================================================
PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete fInputFileCmd;
  delete fSourceDirectory;
  delete fPowtexDirectory;
}

//============================================================================
// SetNewValue - Called when user enters a command in macro or UI
//============================================================================
// Processes commands and delegates to PrimaryGenerator
// This is the callback function when user types /powtex/source/inputFile <path>
void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  // Check which command was entered and call appropriate generator method
  if (command == fInputFileCmd) {
    // User entered: /powtex/source/inputFile <path>
    // Note: This respects the command-line lock in PrimaryGenerator
    fPrimaryGenerator->SetInputFilePath(newValue);
  }
}