#include "BCF10Material.hh"
#include "G4SystemOfUnits.hh"

namespace {

constexpr G4int NUMENTRIES = 11;
G4double core_photonEnergy[NUMENTRIES] = { 2.32 * eV, 2.45 * eV, 2.59 * eV,
                                           2.65 * eV, 2.70 * eV, 2.80 * eV,
                                           2.87 * eV, 2.91 * eV, 2.96 * eV,
                                           3.01 * eV, 3.07 * eV };

G4double core_scintilationSpectra[NUMENTRIES] = { 0.00, 0.06, 0.19, 0.36,
                                                  0.52, 0.71, 1.00, 0.88,
                                                  0.69, 0.17, 0.00 };

G4double core_pathLenght[NUMENTRIES] = { 2.2 * m, 2.2 * m, 2.2 * m, 2.2 * m,
                                         2.2 * m, 2.2 * m, 2.2 * m, 2.2 * m,
                                         2.2 * m, 2.2 * m, 2.2 * m };

const G4double core_RefractiveIndex = 1.6;

const G4double core_scintilationYield = 8000 / MeV;

const G4double core_YieldRatio = 1.0;

const G4double core_decayTimeConstant = 2.7 * ns;

const G4double core_resolutionScale = 1.0;

constexpr G4int NUMENTRIESCLAD = 50;

G4double clad_photonEnergy[NUMENTRIESCLAD] = {
  2.00 * eV, 2.03 * eV, 2.06 * eV, 2.09 * eV, 2.12 * eV, 2.15 * eV, 2.18 * eV,
  2.21 * eV, 2.24 * eV, 2.27 * eV, 2.30 * eV, 2.33 * eV, 2.36 * eV, 2.39 * eV,
  2.42 * eV, 2.45 * eV, 2.48 * eV, 2.51 * eV, 2.54 * eV, 2.57 * eV, 2.60 * eV,
  2.63 * eV, 2.66 * eV, 2.69 * eV, 2.72 * eV, 2.75 * eV, 2.78 * eV, 2.81 * eV,
  2.84 * eV, 2.87 * eV, 2.90 * eV, 2.93 * eV, 2.96 * eV, 2.99 * eV, 3.02 * eV,
  3.05 * eV, 3.08 * eV, 3.11 * eV, 3.14 * eV, 3.17 * eV, 3.20 * eV, 3.23 * eV,
  3.26 * eV, 3.29 * eV, 3.32 * eV, 3.35 * eV, 3.38 * eV, 3.41 * eV, 3.44 * eV,
  3.47 * eV
};

G4double clad_pathLenght[NUMENTRIESCLAD] = {
  5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m,
  5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m,
  5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m,
  5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m,
  5.40 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m,
  1.10 * m, 1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,
  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,
  1. * mm
};

const G4double clad1_RefractiveIndex = 1.49;
const G4double clad2_RefractiveIndex = 1.42;

}

BCF10::Materials
BCF10::createMaterials()
{
  G4NistManager* sNistMan = G4NistManager::Instance();

  // Core
  // Use the existing polysterene but alter density
  // according to the catalogue (1.06 -> 1.05)
  G4Material* PolystereneCore = sNistMan->BuildMaterialWithNewDensity(
    "BCF10_Polysterene", "G4_POLYSTYRENE", 1.05 * g / cm3);

  // Set Birk's constant
  PolystereneCore->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

  G4MaterialPropertiesTable* BCF10_Polysterene_MPT =
    new G4MaterialPropertiesTable();

  BCF10_Polysterene_MPT->AddConstProperty("RINDEX", core_RefractiveIndex);

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

  // PMMA C5 O2 H8
  std::vector<G4int> PMMAnatoms = { 5, 8, 2 };
  std::vector<G4String> PMMAelements = { "C", "H", "O" };
  G4double PMMAdensity = 1.190 * g / cm3;

  // Clad1
  G4Material* PMMAClad1 = sNistMan->ConstructNewMaterial(
    "BCF10_PMMAClad1", PMMAelements, PMMAnatoms, PMMAdensity);

  G4MaterialPropertiesTable* BCF10_PMMAClad1_MPT =
    new G4MaterialPropertiesTable();
  BCF10_PMMAClad1_MPT->AddConstProperty("RINDEX", clad1_RefractiveIndex);

  BCF10_PMMAClad1_MPT->AddProperty(
    "ABSLENGTH", clad_photonEnergy, clad_pathLenght, NUMENTRIESCLAD);
  PMMAClad1->SetMaterialPropertiesTable(BCF10_PMMAClad1_MPT);

  // Clad 2
  G4Material* PMMAClad2 = sNistMan->ConstructNewMaterial(
    "BCF10_PMMAClad2", PMMAelements, PMMAnatoms, PMMAdensity);
  G4MaterialPropertiesTable* BCF10_PMMAClad2_MPT =
    new G4MaterialPropertiesTable();
  BCF10_PMMAClad2_MPT->AddConstProperty("RINDEX", clad2_RefractiveIndex);

  BCF10_PMMAClad2_MPT->AddProperty(
    "ABSLENGTH", clad_photonEnergy, clad_pathLenght, NUMENTRIESCLAD);
  PMMAClad2->SetMaterialPropertiesTable(BCF10_PMMAClad2_MPT);

  //
  return { PolystereneCore, PMMAClad1, PMMAClad2 };
}
