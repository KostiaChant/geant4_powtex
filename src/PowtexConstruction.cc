//******************************************************************************
// PowtexConstruction.cc
//
// Top-level detector construction for Powtex.
//
// This file prepares global geometry/material parameters and delegates the
// detailed Mantel segment/module construction to PowtexMantelConstruct.icc.
//
// 1.00 IS, FRM2, 2013:  First version.
//******************************************************************************
//
#include "PowtexConstruction.hh"
#include "DefineUserGas.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4Element.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4NistMaterialBuilder.hh"
#include "G4NistElementBuilder.hh"
#include "G4MaterialTable.hh"

#include "G4Box.hh"
#include "G4Ellipsoid.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Polyhedra.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PVParameterised.hh"

#include "G4MultiUnion.hh"

#include "G4UnitsTable.hh"

#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4AssemblyVolume.hh"
#include "G4GenericTrap.hh"
#include "G4TwoVector.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"


#include <vector>
#include "globals.hh"
#include "G4ios.hh"


using namespace std;

PowtexConstruction::PowtexConstruction()

{;}

PowtexConstruction::~PowtexConstruction()
{;}


void PowtexConstruction::DefineMaterials()
{
  // Material members are declared in PowtexMaterials.ihh and assigned in this
  // include to preserve the legacy material-definition workflow.
#include "PowtexMaterials.icc"
}

G4VPhysicalVolume* PowtexConstruction::Construct()
{

    // Visualization palette for geometry inspection runs.

    G4Colour  white   (1.0, 1.0, 1.0) ;
  	G4Colour  grey    (0.5, 0.5, 0.5) ;
  	G4Colour  lgrey   (.85, .85, .85) ;
  	G4Colour  red     (1.0, 0.0, 0.0) ;
  	G4Colour  blue    (0.0, 0.0, 1.0) ;
  	G4Colour  cyan    (0.0, 1.0, 1.0) ;
  	G4Colour  magenta (1.0, 0.0, 1.0) ; 
  	G4Colour  yellow  (1.0, 1.0, 0.0) ;
  	G4Colour  orange  (.75, .55, 0.0) ;
  	G4Colour  lblue   (0.7,  0.77, 0.87) ; // light steel blue
  	G4Colour  lgreen  (0.0, .75, 0.0) ;
  	G4Colour  green   (0.0, 1.0, 0.0) ;
  	G4Colour  brown   (0.7, 0.4, 0.1) ;
  	G4Colour  black   (0.0, 0.0, 0.0) ;


	// Build all detector materials before creating logical volumes.
	DefineMaterials();

  // World volume that hosts all detector components.
  G4Box* solidWorld = new G4Box("World", 5.*m, 5.0*m, 5.*m);
  					       
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), logicWorld,
			"World", 0, false, 0);

  // Hide world box in visualization so only detector elements are visible.
  (*logicWorld).SetVisAttributes(G4VisAttributes::GetInvisible());

  // Lookup-table output handle (opened and closed in PowtexMantelConstruct.icc).
  FILE* oFile;

  // Mantel geometry configuration (all dimensions in Geant4 units).
  G4double Bthick = 1.1*um; /* boron converter thickness at normal incidence */
  G4double Althick = 300.*um; /* aluminum cathode thickness */

  G4double shieldz = 5./2*cm; /* boron shielding half-length at detector back */

  G4double tilt_theta = -10.; /* Mantel inclination angle in degrees */
  
  G4int nM = 8;   // number of segments (cassettes) per module
 
  G4int no_modulesM = 32;  // set 45 to fill the full cylindrical coverage
  
  G4int n_stripsM = 192; // strips per segment (mirrored around center)
  G4int n_wiresM = 16;   // wire layers per segment depth
  
  G4double dist_det = 800*mm;   /* sample-to-segment reference distance */

  G4double init_angleM = 70; // module ring start angle (legacy scan values kept in comment)
  // 122 for the +3*mm setting, 123.2 for the 0*mm setting
  
  G4double dthetaM = 0.469*M_PI/180;  /* angular pitch between neighboring strips */
  
  G4double margin = 10.*mm; // internal clearance in segment side directions
  G4double marginz = 3.*mm; // internal clearance in segment longitudinal direction
  G4double margintb = 1.5*mm; // reserved top/bottom clearance for geometry studies
  G4double seg_dist = 2*mm; // minimum spacing between adjacent segments to avoid overlap
   
  G4double CathSubstrX1 = Althick/2;
  G4double CathSubstrX2 = Althick/2;
  G4double CathConvX1 = Bthick/2;
  G4double CathConvX2 = Bthick/2;
  
  G4double eta_b = 0.;
  G4double eta_t = 0.;
  G4double eta_w = 0.;  
  
  
  // Reused rotations for segment assemblies and gas voxels.
  G4RotationMatrix* rotT13 = new G4RotationMatrix();  // assembly orientation
  G4RotationMatrix* rotTa = new G4RotationMatrix();   // subtraction-solid orientation
  G4RotationMatrix* rotGL = new G4RotationMatrix();   // gas voxel orientation
    
  G4ThreeVector Tvl, Tvr;

  G4ThreeVector T14t,T14b,T14,T14h;
  G4ThreeVector Th,ThC1,ThC2;
    
  G4ThreeVector TGL,Ts,Tsm,Tsc,Tnull;     
  

 // Voxel-center caches used for placement and lookup-table generation.

 static double  voxelZZM[129][33];
 static double  voxelXXM[33];
 static double  voxelYYM[33];

 static double segZ[15];
 static double segX[15];
 static double segY[15];
  
 char const *outform2,*outform3;
 

// Extended NeXus-friendly output (kept for optional debug exports).
 outform3 = "%4d  %4d  %4d   %4d   %4d  %4d   %6.2f    %6.2f    %6.2f   %6.2f   %6.3f    %6.3f    %6.3f    %6.3f    %6.3f \n";
 
// Compact lookup-table format used by ROOT analysis scripts.
 outform2 = "%4d  %4d  %4d  %4d   %4d   %4d     %8.3f    %8.3f   %8.3f\n";

 // Global coordinate caches for transformed voxel centers.
 static double VYM[5000][32][128];
 static double VXM[5000][32][128];
 static double VZM[5000][32][128];

 static double sxM[5000][32][128];
 static double syM[5000][32][128];
 static double szM[5000][32][128];

 // Visualization attribute handles filled in the Mantel include.
 G4VisAttributes* TrAlu_vat;
 G4VisAttributes* TrBor_vat;
 G4VisAttributes* TrBShield_vat;
 G4VisAttributes* TrGLbr_vat;
 G4VisAttributes* TrGLbl_vat;
 G4VisAttributes* TrGLur_vat;
 G4VisAttributes* TrGLul_vat;


 // Build Mantel geometry, place assemblies, and write lookup table.
#include "PowtexMantelConstruct.icc"


  // Return pointer to the master world physical volume.
  return physWorld;
}

