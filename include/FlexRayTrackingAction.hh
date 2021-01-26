/// \file FlexRayTrackingAction.hh
/// \brief Definition of the FlexRayTrackingAction class

#ifndef FlexRayTrackingAction_h
#define FlexRayTrackingAction_h

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"
#include "FlexRayEventAction.hh"
#include "FlexRayDetectorConstruction.hh"
#include "globals.hh"

class FlexRayEventAction;

class G4LogicalVolume;

/// Tracking action class

class FlexRayTrackingAction : public G4UserTrackingAction
{
public:
  FlexRayTrackingAction(FlexRayEventAction* eventAction);
  virtual ~FlexRayTrackingAction();

  void Initialize(FlexRayDetectorConstruction *detector);

  // method from the base class
  virtual void PostUserTrackingAction(const G4Track* track) override;

private:
  FlexRayEventAction* fEventAction;
  G4int fNumFibers;
  G4double fFiberSpacing;
  G4double fLayerSpacing;
  G4double fFiberLength;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
