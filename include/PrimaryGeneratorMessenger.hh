//
// ********************************************************************
// POWTEX Primary Generator Messenger
// ********************************************************************
//

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

  void SetNewValue(G4UIcommand* command, G4String newValue) override;

private:
  PrimaryGenerator* fPrimaryGenerator;
  G4UIdirectory* fPowtexDirectory;
  G4UIdirectory* fSourceDirectory;
  G4UIcmdWithAString* fInputFileCmd;
};

#endif