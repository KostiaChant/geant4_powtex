//
// ********************************************************************
// 1.00 IS, ESS  
// *                                                                  *
// ********************************************************************
//
//

//
// ============================================================================
// PrimaryGenerator - Reads neutron primaries from VITESS-generated data file
// ============================================================================
// This class implements the primary particle generation by reading from an
// input file containing neutron event data (ToF, wavelength, position, direction).
// 
// INPUT FILE PIPELINE:
// - Input file path can be set in two ways:
//   1. Command line: --input-file <path> (locked, cannot be overridden)
//   2. Macro command: /powtex/source/inputFile <path> (only if not command line)
// - The PrimaryGeneratorMessenger handles macro commands via UI
// - File is parsed during GeneratePrimaryVertex() for each event
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
  // Main generation method called once per event
  void GeneratePrimaryVertex(G4Event*) override;
  
  // Accessors for primary particle count
  G4int GetNoPrimaries() const;
  void IncrementNoPrimaries();
  
  // Input file configuration methods
  // SetInputFilePath: called by messenger from macro commands (can be overridden)
  void SetInputFilePath(const G4String& filePath);
  // SetInputFilePathFromCommandLine: command line override (locks file path)
  void SetInputFilePathFromCommandLine(const G4String& filePath);
  const G4String& GetInputFilePath() const;
 
private:
  // Particle counter for tracking generated neutron events
  G4int fprimaries;
  
  // Input file path from VITESS neutron simulation
  G4String fInputFilePath;
  
  // Flag to lock input file when set from command line (prevents macro override)
  G4bool fInputFileLockedByCommandLine;
  
  // UI messenger for handling /powtex/source/inputFile commands from macros
  PrimaryGeneratorMessenger* fMessenger;
  
};

 inline G4int PrimaryGenerator::GetNoPrimaries() const { return fprimaries; }
 inline void PrimaryGenerator::IncrementNoPrimaries() { fprimaries++; }
 inline const G4String& PrimaryGenerator::GetInputFilePath() const { return fInputFilePath; }
#endif


