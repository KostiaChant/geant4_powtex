//******************************************************************************
// PowtexConstruction.hh
//
// Defines the detector geometry construction for the Powtex neutron detector.
// Inherits from G4VUserDetectorConstruction to build the detector structure.
//
// 1.00 IS, ESS, 2022:  First version.
//******************************************************************************
//

class G4VPhysicalVolume;
class G4Material;

#include "G4VUserDetectorConstruction.hh"

/// Main detector construction class for Powtex
/// Responsible for building the complete detector geometry including
/// the mantel detector, materials, and logical volumes.
class PowtexConstruction : public G4VUserDetectorConstruction
{
  public:
  
    /// Constructor
    PowtexConstruction();
    /// Destructor
    ~PowtexConstruction();
    
    /// Build and return the physical world volume with complete detector geometry
    G4VPhysicalVolume* Construct();

  private:
     
    /// Define all materials used in the detector (gas, aluminum, boron, etc.)
    void DefineMaterials();
     
    /// Include mantel detector construction member variables and logic
    #include "PowtexMantelConstruct.ihh"
    /// Include material declarations
    #include "PowtexMaterials.ihh"
     
    /// Default material for world volume
    G4Material*  fMaterial;   
  

};

