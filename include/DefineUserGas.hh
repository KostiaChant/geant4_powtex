//******************************************************************************
// DefineUserGas.hh
//
// 1.00 IS, FRM2, 2013:  First version.
//******************************************************************************
// 
// This class provides utilities for creating and characterizing custom gas
// mixtures in Geant4. It calculates effective atomic number (Zeff) and atomic
// mass (Aeff) for gas mixtures, and constructs G4Material objects with custom
// compositions, temperatures, and pressures.
//
// Usage:
//   - For density/Aeff/Zeff calculations: pass vectors of materials and fractions
//   - For constructing a new material: provide elements, atom counts, temp, pressure
//
#ifndef DefineUserGas_H
#define DefineUserGas_H 1

#include "G4Material.hh"
#include <vector>
#include "globals.hh"

class G4Material;
class G4Element;


/// Utility class for creating and analyzing custom gas mixtures
/// 
/// Provides methods to:
/// - Calculate effective density of gas mixtures
/// - Calculate effective atomic number (Zeff) for electron interactions
/// - Calculate effective atomic mass (Aeff) for mass interactions
/// - Construct new G4Material objects with custom gas compositions
class DefineUserGas 
{
  public:
    /// Constructor
    DefineUserGas();
    /// Destructor
    ~DefineUserGas();

  public:

    /// Calculate total density of a gas mixture from constituent densities
    /// @param mats Vector of G4Material pointers for gas constituents
    /// @param frac Vector of mass fractions for each material (should sum to 1.0)
    /// @return Mixed gas density in g/cm³
    /// @note All materials should be gases; behavior undefined for solids/liquids
    G4double GetUserGasDensity(const std::vector<G4Material*>& mats,
                                const std::vector<G4double>& frac);
				
/// Calculate effective atomic mass (A) for a single material
    /// @param mat Pointer to G4Material
    /// @return Effective atomic mass in atomic mass units (amu)
    /// @note Used as basis for mixture calculations via mass fractions
    G4double GetUserAeff(G4Material *mat);
    
    /// Calculate effective atomic number (Z) for a single material
    /// @param mat Pointer to G4Material
    /// @return Effective atomic number (unitless)
    /// @note Important for electron interaction cross-sections
    G4double GetUserZeff(G4Material *mat);
    
    /// Calculate effective atomic mass of a gas mixture
    /// @param mats Vector of G4Material pointers for gas constituents
    /// @param frac Vector of mass fractions for each material
    /// @return Weighted effective atomic mass in amu
    /// @note Used in stopping power and energy loss calculations
    G4double GetUserGasAeff(const std::vector<G4Material*>& mats,
                            const std::vector<G4double>& frac);
		     
    /// Calculate effective atomic number of a gas mixture
    /// @param mats Vector of G4Material pointers for gas constituents
    /// @param frac Vector of mass fractions for each material
    /// @return Weighted effective atomic number
    /// @note Critical for accurate EM interaction modeling in mixed gases
    G4double GetUserGasZeff(const std::vector<G4Material*>& mats,
                            const std::vector<G4double>& frac);
     
/// Create a new G4Material with custom gas composition
    /// @param mname Name for the new material
    /// @param name Vector of G4Element pointers (constituent elements)
    /// @param nbAtoms Vector of atom counts per element (molecule composition)
    /// @param temp Temperature in Kelvin
    /// @param pres Pressure in Pa
    /// @return Pointer to newly constructed G4Material (caller owns memory)
    /// @note Example: He3-CF4 mixture would have 2 elements (He, F) and counts (1, 4)
    G4Material* ConstructUserGas(const G4String& mname, 
			         const std::vector<G4Element*>& name,
                                 const std::vector<G4int>& nbAtoms,
				 G4double temp,
				 G4double pres);
				 
				  
};

#endif
