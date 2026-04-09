//
// ********************************************************************
// 1.00 IS, FRM2  
// ********************************************************************
//
#include "PrimaryGeneratorAction.hh"
#include "PrimaryGenerator.hh"

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
PrimaryGeneratorAction::PrimaryGeneratorAction(const G4String& inputFileFromCommandLine)
: G4VUserPrimaryGeneratorAction(),fPrimaryGenerator(0)
  
{

  fPrimaryGenerator = new PrimaryGenerator();

  if (!inputFileFromCommandLine.empty()) {
    fPrimaryGenerator->SetInputFilePathFromCommandLine(inputFileFromCommandLine);
  }

}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fPrimaryGenerator;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){


  fPrimaryGenerator->GeneratePrimaryVertex(anEvent);	



}


