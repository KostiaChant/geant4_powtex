//
// ============================================================================
// PrimaryGeneratorMessenger - UI Command Interface for Primary Generator
// ============================================================================
// This class provides user interface commands for configuring the primary
// particle source. It implements the G4UImessenger interface to handle
// macro commands and interactive UI.
//
// COMMAND STRUCTURE:
//   /powtex/                    - POWTEX simulation control directory
//   /powtex/source/             - Primary source configuration directory  
//   /powtex/source/inputFile    - Set path to VITESS input data file
//
// The messenger relays UI commands to the PrimaryGenerator via SetNewValue()
// User can set input file via macro: /powtex/source/inputFile /path/to/file
#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class PrimaryGenerator;

class PrimaryGeneratorMessenger : public G4UImessenger
{
public:
  explicit PrimaryGeneratorMessenger(PrimaryGenerator* primaryGenerator);
  ~PrimaryGeneratorMessenger() override;

  // Override method called when user enters a macro command
  void SetNewValue(G4UIcommand* command, G4String newValue) override;

private:
  // Pointer to the generator this messenger configures
  PrimaryGenerator* fPrimaryGenerator;
  
  // UI directory structure for organizing commands
  G4UIdirectory* fPowtexDirectory;   // /powtex/
  G4UIdirectory* fSourceDirectory;   // /powtex/source/
  
  // Input file command: /powtex/source/inputFile
  G4UIcmdWithAString* fInputFileCmd;
};

#endif