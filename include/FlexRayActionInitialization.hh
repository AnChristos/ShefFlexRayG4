#ifndef FlexRayActionInitialization_h
#define FlexRayActionInitialization_h

#include "G4VUserActionInitialization.hh"
class FlexRayDetectorConstruction;
class FlexRayEventAction;
/// Action initialization class.

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class FlexRayActionInitialization : public G4VUserActionInitialization
{
public:
  FlexRayActionInitialization(FlexRayDetectorConstruction* detector,
                              FlexRayEventAction* eventAction);
  virtual ~FlexRayActionInitialization();

  virtual void BuildForMaster() const override;
  virtual void Build() const override;

private:
  FlexRayDetectorConstruction* fDetector;
  FlexRayEventAction* fEventAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

