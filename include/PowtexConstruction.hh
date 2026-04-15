//******************************************************************************
// PowtexConstruction.hh
//
// Geometry entry point for the Powtex detector.
//
// This class owns the Geant4 detector construction workflow:
// - define detector materials
// - build the world volume
// - delegate detailed Mantel geometry assembly to include-based helpers
//
// The heavy geometry logic lives in companion .ihh/.icc files to keep the
// class interface compact and to preserve compatibility with existing macros.
//
// 1.00 IS, ESS, 2022:  First version.
//******************************************************************************
//

class G4VPhysicalVolume;
class G4Material;

#include "G4VUserDetectorConstruction.hh"

/// Main detector construction class for Powtex
/// Builds the complete detector hierarchy, including Mantel modules,
/// material assignment, and world placement.
class PowtexConstruction : public G4VUserDetectorConstruction
{
  public:
  
    /// Constructor
    PowtexConstruction();
    /// Destructor
    ~PowtexConstruction();
    
    /// Build and return the master world physical volume.
    /// This method is called by Geant4 at initialization time.
    G4VPhysicalVolume* Construct();

  private:
     
    /// Define all detector materials (gas mixture, aluminum, boron layers, world).
    /// Material instances are assigned to members declared in PowtexMaterials.ihh.
    void DefineMaterials();
     
    /// Persistent logical-volume members used by the Mantel construction code.
    #include "PowtexMantelConstruct.ihh"

    /// Material pointers shared between DefineMaterials() and Construct().
    #include "PowtexMaterials.ihh"
     
    /// Reserved hook for optional world material override.
    /// Kept for backward compatibility with earlier geometry variants.
    G4Material*  fMaterial;   
  

};

