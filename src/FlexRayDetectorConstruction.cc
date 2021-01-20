#include "FlexRayDetectorConstruction.hh"
#include "BCF10Material.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4GeometryManager.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4ios.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4VPhysicalVolume.hh"

using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayDetectorConstruction::FlexRayDetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayDetectorConstruction::~FlexRayDetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume*
FlexRayDetectorConstruction::Construct()
{

  /*
   * Definition of materials
   */
  G4NistManager* sNistMan = G4NistManager::Instance();
  sNistMan->SetVerbose(2);

  G4Material* Air = sNistMan->FindOrBuildMaterial("G4_AIR");
  BCF10::Materials materials = BCF10::createMaterials();

  // print fr materials (for debug purposes mainly)
  G4cout << G4endl << "The materials defined are : " << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  G4cout << "Material properties table for BCF10 core : " << G4endl;
  materials.core->GetMaterialPropertiesTable()->DumpTable();
  G4cout << "Material properties table for BCF10 clad1 : " << G4endl;
  materials.clad1->GetMaterialPropertiesTable()->DumpTable();
  G4cout << "Material properties table for BCF10 clad2 : " << G4endl;
  materials.clad2->GetMaterialPropertiesTable()->DumpTable();

  /*
   * Construct the enclosing world
   */
  G4double worldLength = 1.0 * m;
  G4double halfWorldLength = 0.5 * worldLength;
  G4Box* solidWorld =
    new G4Box("world", halfWorldLength, halfWorldLength, halfWorldLength);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, Air, "World");

  //  Must place the World Physical volume unrotated at (0,0,0).
  G4VPhysicalVolume* physiWorld =
    new G4PVPlacement(0,               // no rotation
                      G4ThreeVector(), // at (0,0,0)
                      logicWorld,      // its logical volume
                      "World",         // its name
                      0,               // its mother  volume
                      false,           // no boolean operations
                      0);              // copy number

  // Visualization attributes
  G4VisAttributes* WorldVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
  // Give visualization attributes to the logical volumes
  logicWorld->SetVisAttributes(WorldVisAtt);

  /*
   * Construct the scintillating fiber geometry
   */
  
  G4double fiberLength = 0.2 * m;
  G4double fiberRadius = 1 * mm;
  G4double fiberInnerRadius2 = 0.99 * fiberRadius; // inner radius of second cladding
  G4double fiberInnerRadius1 = 0.96 * fiberRadius; // inner radius of first cladding

  G4Tubs* fiberClad2 = new G4Tubs("OuterCladding", 0, fiberRadius, fiberLength/2, 0 * deg, 360 * deg);
  G4Tubs* fiberClad1 = new G4Tubs("InnerCladding", 0, fiberInnerRadius2, fiberLength/2, 0 * deg, 360 * deg);
  G4Tubs* fiberCore = new G4Tubs("Core", 0, fiberInnerRadius1, fiberLength/2, 0 * deg, 360 * deg);

  G4OpticalSurface *opSurface = new G4OpticalSurface("OpSurface", glisur, ground, dielectric_dielectric, 0.999); //roughness=0.999?

  G4LogicalVolume *logicFiberClad2 = new G4LogicalVolume(fiberClad2, materials.clad2, "OuterCladding");
  G4LogicalVolume *logicFiberClad1 = new G4LogicalVolume(fiberClad1, materials.clad1, "InnerCladding");
  G4LogicalVolume *logicFiberCore = new G4LogicalVolume(fiberCore, materials.core, "Core");

  // construct full fiber (place IC and core inside OC)
  G4VPhysicalVolume *PhysClad1 = new G4PVPlacement(0, G4ThreeVector(), logicFiberClad1, "InnerCladding", logicFiberClad2, false, 0,true);
  G4VPhysicalVolume *PhysCore = new G4PVPlacement(0, G4ThreeVector(), logicFiberCore, "Core", logicFiberClad1, false, 0,true);

  new G4LogicalBorderSurface("SurfClad1Out", PhysCore, PhysClad1, opSurface);
  new G4LogicalBorderSurface("SurfClad1In", PhysClad1, PhysCore, opSurface);


  G4int numFibers = 4;
  G4double fiberSpacing = 2.2 * mm;
  G4double layerSpacing = 3 * mm;

  G4RotationMatrix *xrot = new G4RotationMatrix();
  xrot->rotateX(90*deg);
  G4RotationMatrix *yrot = new G4RotationMatrix();
  yrot->rotateY(90*deg);

  for(G4int i=0; i<numFibers; i++){
    G4double offset = (-numFibers * 0.5 + i + 0.5) * fiberSpacing;

    G4ThreeVector xpos(offset, 0, layerSpacing*0.5);
    new G4PVPlacement(xrot, xpos, logicFiberClad2, "OuterCladdingX", logicWorld, false, i,true);

    G4ThreeVector ypos(0, offset, -layerSpacing*0.5);
    new G4PVPlacement(yrot, ypos, logicFiberClad2, "OuterCladdingY", logicWorld, false, i,true);
  }

  // Return world
  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
