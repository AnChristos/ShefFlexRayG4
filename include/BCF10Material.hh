//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef BCF10Material_h
#define BCF10Material_h

#include "G4Material.hh"
#include "G4NistManager.hh"
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

struct Materials
{
  G4Material* core;
  G4Material* clad1;
  G4Material* clad2;

};

Materials
createMaterials();

} // end of namespace BCF10

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
#endif /*BCF10Material_h*/
