//
// ============================================================================
// PrimaryGeneratorAction - Orchestrates the Primary Particle Generation
// ============================================================================
// This class orchestrates the overall primary particle generation pipeline.
// It receives optional command-line arguments and creates/configures the
// PrimaryGenerator and supporting infrastructure.
//
// INPUT PIPELINE FLOW:
// 1. User provides --input-file <path> on command line (if applicable)
// 2. Main program creates PrimaryGeneratorAction with this path
// 3. PrimaryGeneratorAction creates PrimaryGenerator
// 4. If path provided, locks it in PrimaryGenerator (prevents macro override)
// 5. PrimaryGenerator creates PrimaryGeneratorMessenger for UI commands
// 6. Each Geant4 event calls GeneratePrimaries(event)
// 7. Which delegates to PrimaryGenerator::GeneratePrimaryVertex()
//
#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Types.hh"
#include "globals.hh"


class PrimaryGenerator;
class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  // Constructor: Can receive input file path from command line
  // If inputFileFromCommandLine is non-empty, it locks the file path
  explicit PrimaryGeneratorAction(const G4String& inputFileFromCommandLine = "");
  ~PrimaryGeneratorAction();
  
  // Called by Geant4 event loop for each event
  // Delegates to PrimaryGenerator::GeneratePrimaryVertex()
  void GeneratePrimaries(G4Event*);
  
private:
  // The PrimaryGenerator reads from VITESS file and creates vertices
  PrimaryGenerator* fPrimaryGenerator;
  
  // Not used in current implementation (particle gun alternative)
  G4ParticleGun* particleGun;

};

#endif


