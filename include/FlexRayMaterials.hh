//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef FlexRayMaterials_h
#define FlexRayMaterials_h

#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "globals.hh"
#include <vector>

class FlexRayMaterials{
public:
  FlexRayMaterials();
  ~FlexRayMaterials() {};

  // cladding materials
  G4Material *PMMA149;
  G4Material *PMMA142;

  // scintillating materials
  G4Material *BCF10;
  G4Material *BCF12; // improved transmission for long fibers
  G4Material *BCF60; // improved radiation hardness
  G4Material *BC505; // high-light-yield liquid scintillator

private:
  std::vector<G4double> CurveToHistX(std::vector<G4double> curveX);
  std::vector<G4double> CurveToHistY(std::vector<G4double> curveX, std::vector<G4double> curveY);

  void FillPropertyFromCurve(G4MaterialPropertiesTable *table, G4String name, std::vector<G4double> curveX, std::vector<G4double> curveY, G4int bins);
  void FillConstProperty(G4MaterialPropertiesTable *table, G4String name, G4double value);
};

#endif
