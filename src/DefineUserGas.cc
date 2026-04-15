//******************************************************************************
// DefineUserGas.cc
//
// 1.00 IS, FRM2, 2013:  First version.
//******************************************************************************
//
#include "DefineUserGas.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4Element.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4NistMaterialBuilder.hh"
#include "G4NistElementBuilder.hh"
#include "G4MaterialTable.hh"

#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"

#include <vector>
#include "globals.hh"
#include "G4ios.hh"

DefineUserGas::DefineUserGas()
{
  // Initialize utility object for gas calculations
  // No internal state; this class provides only static-like utility methods
}

DefineUserGas::~DefineUserGas()
{
  // No dynamic memory managed by this utility class
}

/// Calculate the total density of a gas mixture by weighted sum of component densities
/// 
/// Algorithm:
///   rhoGas = sum( frac[i] * density[i] )
/// where frac[i] is the mass fraction and density[i] is from G4Material.
/// 
/// This assumes ideal gas behavior and linear mixing of molar volumes.
G4double DefineUserGas::GetUserGasDensity(const std::vector<G4Material*>& mats,
                                              const std::vector<G4double>& frac)
{
 
 G4int mat_size = mats.size();
 G4double rhoGas = 0.;
 
 if (mat_size ==0) {
     G4cout<< " Please input the constituents of the new gas! "
	   << G4endl;
	   return 0;
	   }

 // Sum weighted densities from each component
 for (G4int i=0; i<mat_size; i++){
 	G4double rho = mats[i]->GetDensity()/(g/cm3);
 	rhoGas += frac[i]*rho;         // density compound material
      } 
 
// G4cout<<"**********************************************"<<G4endl;
// G4cout<<"Output DefineUserGas: density gas mixture ="<<rhoGas<<" g/cm3"<<G4endl;
// G4cout<<"density gas mixture ="<<G4BestUnit(rho*mg/cm3,"Volumic Mass")<<G4endl;
// G4cout<<"**********************************************"<<G4endl;

 return rhoGas;
 
 
}

/// Calculate effective atomic number (Zeff) for a gas mixture
/// 
/// Algorithm:
///   1. Check that total mass fraction <= 1.0 (some may be other materials)
///   2. For each component: Zeff += frac[i]/rho_total * rho[i] * Zeff_single[i]
/// 
/// This weights the single-material Zeff values by the density ratio of each component,
/// accounting for the fact that density varies with composition.
/// 
/// Zeff is important for:
///   - Electron ionization and excitation (especially for detectors)
///   - Bremsstrahlung radiation production
///
G4double DefineUserGas::GetUserGasZeff(const std::vector<G4Material*>& mats,
                                           const std::vector<G4double>& frac)
{

 G4int mat_size = mats.size();
 G4double rhoGas = 0.;
 G4double fracT = 0.;
 G4double Zeff = 0.;
 
 if (mat_size ==0) {
//     G4cout<< " Please input the constituents of the new gas! "
//	   << G4endl;
	   return 0;
	   }

 // First pass: validate that fractions sum to reasonable value
 for (G4int i=0; i<mat_size; i++){
  	fracT += frac[i];         
      } 
 
 if (fracT > 1) {     
//     G4cout<< " The sum of the gas mass fractions is greater than 1!"
//	   << G4endl;
	   return -1;  // Error flag
	   }
 else {
 	// Second pass: calculate mixture properties
 	for (G4int i=0; i<mat_size; i++){
 		G4double rho = mats[i]->GetDensity()/(g/cm3);
 		rhoGas += frac[i]*rho;         // Total density of mixture
        	}
 	// Third pass: weight Zeff by density fractions
 	for (G4int i=0; i<mat_size; i++){
 		G4double rho = mats[i]->GetDensity()/(g/cm3);
        	Zeff += frac[i]*rho/rhoGas*GetUserZeff(mats[i]);  // Weighted Zeff
      		}
      
// 	G4cout<<"**********************************************"<<G4endl;
// 	G4cout<<"Output DefineUserGas: Zeff gas mixture ="<<Zeff<<G4endl;
// 	G4cout<<"**********************************************"<<G4endl;
 
 	return Zeff;
	}
}

/// Calculate effective atomic mass (Aeff) for a gas mixture
/// 
/// Algorithm:
///   1. Validate that total mass fraction <= 1.0
///   2. Calculate total density: rhoGas = sum(frac[i] * rho[i])
///   3. Calculate weighted Aeff: Aeff += frac[i]/rho_total * rho[i] * Aeff_single[i]
/// 
/// Similar to Zeff calculation, weights by density ratios to account for
/// varying composition effects on stopping power and energy loss.
///
G4double DefineUserGas::GetUserGasAeff(const std::vector<G4Material*>& mats,
                                           const std::vector<G4double>& frac)
{
 
 G4int mat_size = mats.size();
 G4double rhoGas = 0.;
 G4double fracT = 0.;
 G4double Aeff = 0.;
 
 if (mat_size ==0) {
//     G4cout<< " Please input the constituents of the new gas! "
//	   << G4endl;
	   return 0;
	   }

 // First pass: validate fractions
 for (G4int i=0; i<mat_size; i++){
  	fracT += frac[i];         
      } 
 
 if (fracT > 1) {     
//     G4cout<< " The sum of the gas mass fractions is greater than 1!"
//	   << G4endl;
	   return -1;  // Error flag
	   }
 else {
 	// Calculate total mixture density
 	for (G4int i=0; i<mat_size; i++){
 		G4double rho = mats[i]->GetDensity()/(g/cm3);
 		rhoGas += frac[i]*rho;         // Total density of mixture
      		}
 	// Weight Aeff by density fractions
	 for (G4int i=0; i<mat_size; i++){
 		G4double rho = mats[i]->GetDensity()/(g/cm3);
 		Aeff += frac[i]*rho/rhoGas*GetUserAeff(mats[i]);  // Weighted Aeff
      		}

// 	G4cout<<"**********************************************"<<G4endl;
// 	G4cout<<"Output DefineUserGas: Aeff gas mixture ="<<Aeff<<G4endl;
// 	G4cout<<"**********************************************"<<G4endl;

 	return Aeff;
	}
}

/// Calculate effective atomic mass for a single material
/// 
/// Algorithm:
///   For each element in the material:
///     Aeff += A[element] * massFraction[element]
/// 
/// This is a mass-weighted average of atomic masses in the compound,
/// used for stopping power calculations.
///
G4double DefineUserGas::GetUserAeff(G4Material *mat)
{
 
	G4double aeff=0.;
 
   // Get number of elements in this material
   const G4int nom = mat->GetNumberOfElements();
 
 	for (G4int i=0; i<nom; i++){
		// Get atomic mass (in g/mole) and mass fraction for this element
		G4double a = mat->GetElement(i)->GetA()/(g/mole);
		G4double massFraction = mat->GetFractionVector()[i];
		// Accumulate weighted atomic mass
		aeff += a*massFraction;
		 
//        G4cout<<"**********************************************"<<G4endl;
//	G4cout<<"Output DefineUserGas: mat ="<<mat->GetElement(i)->GetName()<<" A="<<a<<" mass fract="<<massFraction<<G4endl;
	}
 
	G4cout<<" Output DefineUserGas: Aeff mat ="<<aeff<<G4endl;
 return aeff; 
}

/// Calculate effective atomic number for a single material
/// 
/// Algorithm:
///   For each element in the material:
///     Zeff += Z[element] * massFraction[element]
/// 
/// This is a mass-weighted average of atomic numbers in the compound.
/// Zeff is important for modeling:
///   - Ionization and excitation cross-sections
///   - Bremsstrahlung production
///   - Multiple scattering
///
G4double DefineUserGas::GetUserZeff(G4Material *mat)
{
 
	G4double zeff=0.;
 
	// Get number of elements in this material
	const G4int nom = mat->GetNumberOfElements();
 
 	for (G4int i=0; i<nom; i++){
		// Get atomic number and mass fraction for this element
		G4double z = mat->GetElement(i)->GetZ();
		G4double massFraction = mat->GetFractionVector()[i];
		// Accumulate weighted atomic number
		zeff += z*massFraction;
		 
//        G4cout<<"**********************************************"<<G4endl;
//	G4cout<<"Output DefineUserGas: mat ="<<mat->GetElement(i)->GetName()<<" Z ="<<z<<" mass fract="<<massFraction<<G4endl;
	}
 
//	G4cout<<" Output DefineUserGas: Zeff mat ="<<zeff<<G4endl;
	return zeff; 
}


/// Construct a new G4Material representing a custom gas at specified T, P
/// 
/// Algorithm:
///   1. Calculate total molecular mass: M = sum(nbAtoms[i] * AtomicMass[i])
///   2. Use ideal gas law to compute density: rho = M / (N_A * k_B * T / P)
///      where:
///        N_A = Avogadro's number
///        k_B = Boltzmann constant
///        T = temperature (K)
///        P = pressure (Pa)
///   3. Create and populate G4Material object with elements
/// 
/// Example usage for He3-CF4 mixture at 300K, 1 atm:
///   vector<G4Element*> elems = {He, C, F};
///   vector<G4int> counts = {1, 1, 4};
///   G4Material* gas = definer.ConstructUserGas("He3CF4", elems, counts, 300*K, 1e5*Pa);
///
G4Material* DefineUserGas::ConstructUserGas(const G4String& mname, 
											const std::vector<G4Element*>& name,
											const std::vector<G4int>& nbAtoms,
											G4double temp,G4double pres)
{
 
 G4NistManager* man = G4NistManager::Instance();
  
 G4int els = name.size();
 
 G4State state = kStateGas;  // Must be gas state
 
 G4double massPerMole = 0.;
 
 if (els ==0) {
//     G4cout<< " Please input the constituents of the new gas! "
//	   << G4endl;
	   return 0;  // Error: no elements
	   }
	   
 // Calculate total molar mass from atomic masses and stoichiometry
 for (G4int i=0; i<els; i++){
                G4double zelem = name[i]->GetZ();
                // Accumulate: (number of atoms) * (atomic mass of element)
                massPerMole += nbAtoms[i] * man->GetAtomicMassAmu(int(zelem))*amu_c2;
		}
 
 // Apply ideal gas law: PV = NkT => rho = M/(N_A*kB*T/P)
 G4double rho = massPerMole/(Avogadro*k_Boltzmann*temp/pres);  		
		
// G4cout<<"**********************************************"<<G4endl;
// G4cout<<"Output ConstructUserGas: density new gas ="<<rho<<G4endl;
// G4cout<<"**********************************************"<<G4endl;

 // Create G4Material object
 G4Material* mat = new G4Material(mname,rho*g/cm3,els,state,temp,pres);

 // Add all elements with their stoichiometry
 for (G4int i=0; i<els; i++){
 		mat->AddElement(name[i],nbAtoms[i]);
		}
	
 return mat;  // Caller assumes ownership of returned pointer

}
 
