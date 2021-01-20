//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef BCF10Material_h
#define BCF10Material_h

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

namespace BCF10 {

/*
 * BCF-10 scintillating fibers,
 * Polysterene core
 * PMMA Claddings
 *
 * Information taken from
 * Saint Cobain Catalogs
 *
 * createMaterials adds
 * BCF10_Polysterene
 * BCF10_PMMAClad1
 * BCF10_PMMAClad2
 * to the G4NistManager
 *
 */

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

G4double core_RefractiveIndex[NUMENTRIES] = { 1.6, 1.6, 1.6, 1.6, 1.6, 1.6,
                                              1.6, 1.6, 1.6, 1.6, 1.6 };

const G4double core_scintilationYield = 8000 / MeV;

const G4double core_YieldRatio = 1.0;

const G4double core_decayTimeConstant = 2.7 * ns;

const G4double core_resolutionScale = 1.0;

G4double clad1_RefractiveIndex[NUMENTRIES] = { 1.49, 1.49, 1.49, 1.49,
                                               1.49, 1.49, 1.49, 1.49,
                                               1.49, 1.49, 1.49 };

G4double clad2_RefractiveIndex[NUMENTRIES] = { 1.42, 1.42, 1.42, 1.42,
                                               1.42, 1.42, 1.42, 1.42,
                                               1.42, 1.42, 1.42 };

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


} // end of namespace BCF10

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
#endif /*BCF10Material_h*/
