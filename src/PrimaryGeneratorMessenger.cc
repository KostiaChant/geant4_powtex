//
// ********************************************************************
// POWTEX Primary Generator Messenger
// ********************************************************************
//

#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGenerator.hh"
#include "G4ApplicationState.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGenerator* primaryGenerator)
: G4UImessenger(),
  fPrimaryGenerator(primaryGenerator),
  fPowtexDirectory(0),
  fSourceDirectory(0),
  fInputFileCmd(0)
{
  fPowtexDirectory = new G4UIdirectory("/powtex/");
  fPowtexDirectory->SetGuidance("POWTEX simulation control commands.");

  fSourceDirectory = new G4UIdirectory("/powtex/source/");
  fSourceDirectory->SetGuidance("Primary source controls.");

  fInputFileCmd = new G4UIcmdWithAString("/powtex/source/inputFile", this);
  fInputFileCmd->SetGuidance("Set the VITESS primary input file path.");
  fInputFileCmd->SetParameterName("filePath", false);
  fInputFileCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete fInputFileCmd;
  delete fSourceDirectory;
  delete fPowtexDirectory;
}

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == fInputFileCmd) {
    fPrimaryGenerator->SetInputFilePath(newValue);
  }
}