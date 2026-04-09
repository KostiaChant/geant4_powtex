//
// ********************************************************************
// 1.00 IS, ESS  
// *                                                                  *
// ********************************************************************
//
//

#ifndef PrimaryGenerator_h
#define PrimaryGenerator_h 1

#include "G4VPrimaryGenerator.hh"
#include "globals.hh"

class G4Event;
class PrimaryGeneratorMessenger;

class PrimaryGenerator : public G4VPrimaryGenerator
{
public:
  PrimaryGenerator();
  ~PrimaryGenerator();
  
public:
  void GeneratePrimaryVertex(G4Event*) override;
  G4int GetNoPrimaries() const;
  void IncrementNoPrimaries();
  void SetInputFilePath(const G4String& filePath);
  void SetInputFilePathFromCommandLine(const G4String& filePath);
  const G4String& GetInputFilePath() const;
 
private:
 
  G4int fprimaries;
  G4String fInputFilePath;
  G4bool fInputFileLockedByCommandLine;
  PrimaryGeneratorMessenger* fMessenger;
  
};

 inline G4int PrimaryGenerator::GetNoPrimaries() const { return fprimaries; }
 inline void PrimaryGenerator::IncrementNoPrimaries() { fprimaries++; }
 inline const G4String& PrimaryGenerator::GetInputFilePath() const { return fInputFilePath; }
#endif


