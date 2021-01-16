#ifndef FlexRayDetectorConstruction_h
#define FlexRayDetectorConstruction_h 1

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

private:
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

