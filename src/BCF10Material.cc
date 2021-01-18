#include "BCF10Material.hh"
#include "G4SystemOfUnits.hh"

BCF10::Materials
BCF10::createMaterials()
{
  G4NistManager* sNistMan = G4NistManager::Instance();

  // Core
  // Use the existing polysterene but alter density
  // according to the catalogue (1.06 -> 1.05)
  G4Material* PolystereneCore = sNistMan->BuildMaterialWithNewDensity(
    "BCF10_Polysterene", "G4_POLYSTYRENE", 1.05 * g / cm3);

  G4cout << "HERE " << PolystereneCore << G4endl;
  // Set Birk's constant
  PolystereneCore->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

  // Material Properties Table for the Optical properties of the core
  G4MaterialPropertiesTable* BCF10_Polysterene_MPT =
    new G4MaterialPropertiesTable();

  constexpr G4int NUMENTRIES = 11;
  G4double photonEnergy[NUMENTRIES] = { 2.32 * eV, 2.45 * eV, 2.59 * eV,
                                        2.65 * eV, 2.70 * eV, 2.79 * eV,
                                        2.86 * eV, 2.90 * eV, 2.95 * eV,
                                        3.01 * eV, 3.07 * eV };

  G4double core_scintilationSpectra[11] = { 0.00, 0.06, 0.19, 0.36, 0.52, 0.71,
                                            1.00, 0.88, 0.69, 0.17, 0.00 };

  G4double core_pathLenght[NUMENTRIES] = { 2.2 * m, 2.2 * m, 2.2 * m, 2.2 * m,
                                           2.2 * m, 2.2 * m, 2.2 * m, 2.2 * m,
                                           2.2 * m, 2.2 * m, 2.2 * m };

  G4double core_RefractiveIndex[NUMENTRIES] = { 1.6, 1.6, 1.6, 1.6, 1.6, 1.6,
                                                1.6, 1.6, 1.6, 1.6, 1.6 };

  const G4double core_scintilationYield = 8000 / MeV;

  const G4double core_YieldRatio = 1.0;

  const G4double core_decayTimeConstant = 2.7 * ns;

  const G4double core_resolutionScale = 1.0;

  BCF10_Polysterene_MPT->AddProperty(
    "RINDEX", photonEnergy, core_RefractiveIndex, NUMENTRIES);

  BCF10_Polysterene_MPT->AddProperty(
    "ABSLENGTH", photonEnergy, core_pathLenght, NUMENTRIES);

  BCF10_Polysterene_MPT->AddProperty(
    "FASTCOMPONENT", photonEnergy, core_scintilationSpectra, NUMENTRIES);

  BCF10_Polysterene_MPT->AddConstProperty("SCINTILLATIONYIELD",
                                          core_scintilationYield);

  BCF10_Polysterene_MPT->AddConstProperty("RESOLUTIONSCALE",
                                          core_resolutionScale);

  BCF10_Polysterene_MPT->AddConstProperty("FASTTIMECONSTANT",
                                          core_decayTimeConstant);

  BCF10_Polysterene_MPT->AddConstProperty("YIELDRATIO", core_YieldRatio);

  PolystereneCore->SetMaterialPropertiesTable(BCF10_Polysterene_MPT);

  // Cladding
  // PMMA C5 O2 H8
  std::vector<G4int> PMMAnatoms = { 5, 8, 2 };
  std::vector<G4String> PMMAelements = { "C", "H", "O" };
  G4double PMMAdensity = 1.190 * g / cm3;

  G4Material* PMMAClad1 = sNistMan->ConstructNewMaterial(
    "BCF10_PMMAClad1", PMMAelements, PMMAnatoms, PMMAdensity);

  G4Material* PMMAClad2 = sNistMan->ConstructNewMaterial(
    "BCF10_PMMAClad2", PMMAelements, PMMAnatoms, PMMAdensity);

  return { PolystereneCore, PMMAClad1, PMMAClad2 };
}
