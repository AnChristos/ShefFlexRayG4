#ifndef FlexRayDetectorConstruction_h
#define FlexRayDetectorConstruction_h 

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class FlexRayDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  FlexRayDetectorConstruction();
  virtual ~FlexRayDetectorConstruction();
  virtual G4VPhysicalVolume* Construct() override;

  G4double GetFiberLength() {return fFiberLength;};
  G4double GetFiberSpacing() {return fFiberSpacing;};
  G4double GetNumFibers() {return fNumFibers;};

private:
  G4double fFiberLength;
  G4double fFiberSpacing;
  G4int fNumFibers;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

