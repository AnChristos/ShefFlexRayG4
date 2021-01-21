#include "FlexRayTrackingAction.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayTrackingAction::FlexRayTrackingAction(FlexRayEventAction* eventAction)
: G4UserTrackingAction(),
  fEventAction(eventAction)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 FlexRayTrackingAction::~FlexRayTrackingAction()
{
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FlexRayTrackingAction::PostUserTrackingAction(const G4Track* track)
{
  // get final position of track
  G4ThreeVector pos = track->GetPosition();
  
  // this should probably be done with G4SensitiveDetectors instead of raw position once we know more about what we're simulating
  G4double fiberLength = 0.2*m;
  if(fabs(pos.x()) < fiberLength*0.45 && fabs(pos.y()) < fiberLength*0.45) return;

  fEventAction->LogDetection(track->GetKineticEnergy(), track->GetGlobalTime());

  G4cout << "Detection: " << track->GetKineticEnergy() / eV << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
