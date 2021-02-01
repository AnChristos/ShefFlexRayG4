/// \file FlexRayTrackingAction.hh
/// \brief Definition of the FlexRayTrackingAction class

#ifndef FlexRayTrackingAction_h
#define FlexRayTrackingAction_h

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"
#include "FlexRayEventAction.hh"
#include "FlexRayGeometry.hh"
#include "globals.hh"

class FlexRayEventAction;

class G4LogicalVolume;

/// Tracking action class

class FlexRayTrackingAction : public G4UserTrackingAction
{
public:
  FlexRayTrackingAction(FlexRayEventAction* eventAction);
  virtual ~FlexRayTrackingAction();

  // method from the base class
  virtual void PostUserTrackingAction(const G4Track* track) override;

  void LogOpticalPhoton(const G4Track *track);
  void LogXRay(const G4Track *track);

private:
  FlexRayEventAction* fEventAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
