#include "FlexRayDetectorConstruction.hh"
#include "BCF10Material.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4GeometryManager.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"
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

  /*
   * Definition of materials
   */
  G4NistManager* sNistMan = G4NistManager::Instance();
  sNistMan->SetVerbose(2);
  // Air
  G4Material* Air = new G4Material("Air", 1.29 * mg / cm3, 2);
  G4Element* N = new G4Element("Nitrogen", "N", 7., 14.01 * g / mole);
  G4Element* O = new G4Element("Oxygen", "O", 8., 16.00 * g / mole);
  Air->AddElement(N, 70 * perCent);
  Air->AddElement(O, 30 * perCent);
  G4MaterialPropertiesTable* AIR_MPT = new G4MaterialPropertiesTable();
  AIR_MPT->AddConstProperty("RINDEX", 1.0);
  Air->SetMaterialPropertiesTable(AIR_MPT);

  /*
   *
   * MAterials for BCF10
   */
  using namespace BCF10;
  // Core
  // Use the existing polysterene but alter density
  // according to the catalogue (1.06 -> 1.05)

  // Polysterene C8 O8
  std::vector<G4int> Polysterenenatoms = { 8, 8 };
  std::vector<G4String> Polystereneelements = { "C", "H" };
  G4double polysterenedensity = 1.05 * g / cm3;

  G4Material* PolystereneCore =
    sNistMan->ConstructNewMaterial("BCF10_Polysterene",
                                   Polystereneelements,
                                   Polysterenenatoms,
                                   polysterenedensity);
  G4MaterialPropertiesTable* BCF10_Polysterene_MPT =
    new G4MaterialPropertiesTable();

  BCF10_Polysterene_MPT->AddProperty(
    "RINDEX", core_photonEnergy, core_RefractiveIndex, NUMENTRIES);

  BCF10_Polysterene_MPT->AddProperty(
    "ABSLENGTH", core_photonEnergy, core_pathLenght, NUMENTRIES);

  BCF10_Polysterene_MPT->AddProperty(
    "FASTCOMPONENT", core_photonEnergy, core_scintilationSpectra, NUMENTRIES);

  BCF10_Polysterene_MPT->AddConstProperty("SCINTILLATIONYIELD",
                                          core_scintilationYield);

  BCF10_Polysterene_MPT->AddConstProperty("RESOLUTIONSCALE",
                                          core_resolutionScale);

  BCF10_Polysterene_MPT->AddConstProperty("FASTTIMECONSTANT",
                                          core_decayTimeConstant);

  BCF10_Polysterene_MPT->AddConstProperty("YIELDRATIO", core_YieldRatio);

  PolystereneCore->SetMaterialPropertiesTable(BCF10_Polysterene_MPT);
  // Set Birk's constant
  PolystereneCore->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

  // PMMA C5 O2 H8
  std::vector<G4int> PMMAnatoms = { 5, 8, 2 };
  std::vector<G4String> PMMAelements = { "C", "H", "O" };
  G4double PMMAdensity = 1.190 * g / cm3;

  // Clad1
  G4Material* PMMAClad1 = sNistMan->ConstructNewMaterial(
    "BCF10_PMMAClad1", PMMAelements, PMMAnatoms, PMMAdensity);

  G4MaterialPropertiesTable* BCF10_PMMAClad1_MPT =
    new G4MaterialPropertiesTable();

  BCF10_PMMAClad1_MPT->AddProperty(
    "RINDEX", core_photonEnergy, clad1_RefractiveIndex, NUMENTRIES);

  BCF10_PMMAClad1_MPT->AddProperty(
    "ABSLENGTH", clad_photonEnergy, clad_pathLenght, NUMENTRIESCLAD);
  PMMAClad1->SetMaterialPropertiesTable(BCF10_PMMAClad1_MPT);

  // print fr materials (for debug purposes mainly)
  G4cout << G4endl << "The materials defined are : " << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  G4cout << "Material properties for AIR: " << G4endl;
  Air->GetMaterialPropertiesTable()->DumpTable();

  G4cout << "Material properties table for BCF10 core : " << G4endl;
  PolystereneCore->GetMaterialPropertiesTable()->DumpTable();
  G4cout << "Material properties table for BCF10 clad1 : " << G4endl;
  PMMAClad1->GetMaterialPropertiesTable()->DumpTable();
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

  /*
   * Create an optical surface
   */
  G4OpticalSurface* opSurface = new G4OpticalSurface("polish");
  opSurface->SetModel(glisur);
  opSurface->SetType(dielectric_dielectric);
  opSurface->SetFinish(polished);
  /*
   * Construct the scintillating fiber geometry
   */

  G4double fiberLength = 20 * cm;
  // inner radius of first cladding
  G4double fiberCoreRadius = 4.75 * mm;
  G4double cladThickness = 0.25 * mm;

  G4Tubs* fiberClad1 = new G4Tubs("InnerCladding",
                                  fiberCoreRadius,
                                  fiberCoreRadius + cladThickness,
                                  fiberLength / 2,
                                  0 * deg,
                                  360 * deg);
  G4Tubs* fiberCore =
    new G4Tubs("Core", 0, fiberCoreRadius, fiberLength / 2, 0 * deg, 360 * deg);

  G4LogicalVolume* logicFiberClad1 =
    new G4LogicalVolume(fiberClad1, PMMAClad1, "InnerCladding");
  G4LogicalVolume* logicFiberCore =
    new G4LogicalVolume(fiberCore, PolystereneCore, "Core");
  /*
   * place clad1 into world
   */
  G4VPhysicalVolume* physiClad1 = new G4PVPlacement(0,
                                                    G4ThreeVector(0,0,0),
                                                    logicFiberClad1,
                                                    "InnerCladding",
                                                    logicWorld,
                                                    false,
                                                    0,
                                                    true);

  /*
   * place core into clad1
   */
  G4VPhysicalVolume* physiCore = new G4PVPlacement(0,
                                                   G4ThreeVector(0.,0.,0.),
                                                   logicFiberCore,
                                                   "Core",
                                                   logicWorld,
                                                   false,
                                                   0,
                                                   true);

  // Return world
  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
