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
  +GeneratePrimaries(G4Event*)
  -fPrimaryGenerator : PrimaryGenerator*
  -particleGun : G4ParticleGun*
}
class PrimaryGenerator {
  +GeneratePrimaryVertex(G4Event*)
  +GetNoPrimaries() G4int
  -fprimaries : G4int
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

PowtexConstruction --|> G4VUserDetectorConstruction
PrimaryGeneratorAction --|> G4VUserPrimaryGeneratorAction
PrimaryGenerator --|> G4VPrimaryGenerator
SteppingAction --|> G4UserSteppingAction

PrimaryGeneratorAction *-- PrimaryGenerator : owns
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
- Project classes: PowtexConstruction, PrimaryGeneratorAction, PrimaryGenerator, SteppingAction, DefineUserGas.
- Geant4 classes: G4VUserDetectorConstruction, G4VUserPrimaryGeneratorAction, G4VPrimaryGenerator, G4UserSteppingAction, G4RunManager, QGSP_BERT_HP, G4Event, G4Step, G4Material.
