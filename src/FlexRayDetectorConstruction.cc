#include "FlexRayDetectorConstruction.hh"

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

using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayDetectorConstruction::FlexRayDetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayDetectorConstruction::~FlexRayDetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume*
FlexRayDetectorConstruction::Construct()
{
  //--------- Material definition ---------
  // manager for NIST materials
  //G4NistManager* nist = G4NistManager::Instance();

  // Definition of elements

  G4Element* N = new G4Element("Nitrogen", "N", 7., 14.01 * g / mole);
  G4Element* O = new G4Element("Oxygen", "O", 8., 16.00 * g / mole);

  // Definition of materials
  G4double density;
  G4int nel;

  // Air
  G4Material* Air = new G4Material("Air", density = 1.29 * mg / cm3, nel = 2);
  Air->AddElement(N, 0.7);
  Air->AddElement(O, 0.3);

  // Print all the materials defined.
  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  // Definitions of Solids, Logical Volumes, Physical Volumes ---------
  G4double fWorldLength = 1.0 * m;
  G4double HalfWorldLength = 0.5 * fWorldLength;

  G4Box* solidWorld =
    new G4Box("world", HalfWorldLength, HalfWorldLength, HalfWorldLength);
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

  // Return world
  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
