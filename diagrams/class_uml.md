# POWTEX Class UML (Project Classes Vs Geant4 Base Classes)

```mermaid
classDiagram
direction LR

class PowtexConstruction {
  +Construct() G4VPhysicalVolume*
  -DefineMaterials() void
  -fMaterial : G4Material*
}
class PrimaryGeneratorAction {
  +PrimaryGeneratorAction(const G4String& inputFileFromCommandLine = "")
  +GeneratePrimaries(G4Event*)
  -fPrimaryGenerator : PrimaryGenerator*
  -particleGun : G4ParticleGun*
}
class PrimaryGenerator {
  +GeneratePrimaryVertex(G4Event*)
  +SetInputFilePath(const G4String& filePath)
  +SetInputFilePathFromCommandLine(const G4String& filePath)
  +GetInputFilePath() G4String
  +GetNoPrimaries() G4int
  -fprimaries : G4int
  -fInputFilePath : G4String
  -fInputFileLockedByCommandLine : G4bool
  -fMessenger : PrimaryGeneratorMessenger*
}
class PrimaryGeneratorMessenger {
  +SetNewValue(G4UIcommand* command, G4String newValue)
  -fInputFileCmd : G4UIcmdWithAString*
  -fPowtexDirectory : G4UIdirectory*
  -fSourceDirectory : G4UIdirectory*
}
class SteppingAction {
  +UserSteppingAction(const G4Step*)
}
class DefineUserGas {
  +GetUserGasDensity(...)
  +GetUserGasAeff(...)
  +GetUserGasZeff(...)
  +ConstructUserGas(...)
}

class G4VUserDetectorConstruction {
  <<Geant4>>
}
class G4VUserPrimaryGeneratorAction {
  <<Geant4>>
}
class G4VPrimaryGenerator {
  <<Geant4>>
}
class G4UImessenger {
  <<Geant4>>
}
class G4UserSteppingAction {
  <<Geant4>>
}
class G4RunManager {
  <<Geant4>>
}
class QGSP_BERT_HP {
  <<Geant4>>
}
class G4Event {
  <<Geant4>>
}
class G4Step {
  <<Geant4>>
}
class G4Material {
  <<Geant4>>
}
class G4UIcmdWithAString {
  <<Geant4>>
}
class G4UIdirectory {
  <<Geant4>>
}

PowtexConstruction --|> G4VUserDetectorConstruction
PrimaryGeneratorAction --|> G4VUserPrimaryGeneratorAction
PrimaryGenerator --|> G4VPrimaryGenerator
PrimaryGeneratorMessenger --|> G4UImessenger
SteppingAction --|> G4UserSteppingAction

PrimaryGeneratorAction *-- PrimaryGenerator : owns
PrimaryGenerator *-- PrimaryGeneratorMessenger : owns
PrimaryGeneratorAction ..> PrimaryGenerator : forwards GeneratePrimaries()
PrimaryGeneratorAction ..> PrimaryGenerator : SetInputFilePathFromCommandLine()
PrimaryGeneratorMessenger ..> PrimaryGenerator : SetInputFilePath()
PrimaryGeneratorMessenger ..> G4UIcmdWithAString : defines /powtex/source/inputFile
PrimaryGeneratorMessenger ..> G4UIdirectory : creates /powtex and /powtex/source
PowtexConstruction ..> DefineUserGas : uses for gas setup
PowtexConstruction ..> G4Material : configures materials
PrimaryGenerator ..> G4Event : adds primary vertices
SteppingAction ..> G4Step : inspects step state

G4RunManager --> PowtexConstruction : SetUserInitialization()
G4RunManager --> QGSP_BERT_HP : SetUserInitialization()
G4RunManager --> PrimaryGeneratorAction : SetUserAction()
G4RunManager --> SteppingAction : SetUserAction()
```

Legend:
- Project classes: PowtexConstruction, PrimaryGeneratorAction, PrimaryGenerator, PrimaryGeneratorMessenger, SteppingAction, DefineUserGas.
- Geant4 classes: G4VUserDetectorConstruction, G4VUserPrimaryGeneratorAction, G4VPrimaryGenerator, G4UImessenger, G4UserSteppingAction, G4RunManager, QGSP_BERT_HP, G4Event, G4Step, G4Material, G4UIcmdWithAString, G4UIdirectory.
- Primary input file can be set from macro command /powtex/source/inputFile or from command line --input-file/-f.
- Command-line input path has precedence and locks out later macro overrides.
