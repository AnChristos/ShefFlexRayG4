#include "FlexRaySteppingAction.hh"
#include "FlexRayEventAction.hh"
#include "FlexRayGeometry.hh"
#include "G4SteppingManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRaySteppingAction::FlexRaySteppingAction(FlexRayEventAction* eventAction)
  : fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 FlexRaySteppingAction::~FlexRaySteppingAction(){

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FlexRaySteppingAction::UserSteppingAction(const G4Step* step)
{
  // log first interaction of each X-ray in the detector

  G4Track* track = step->GetTrack();
  if(track->GetParticleDefinition()->GetPDGEncoding() != 22) return;
  if(track->GetUserInformation() && track->GetUserInformation()->GetType() == "XRayAlreadyInteracted") return;

  G4String volName = track->GetTouchable()->GetVolume()->GetName();
  if(volName != "Core" && volName != "InnerCladding" && volName != "OuterCladdingX" && volName != "OuterCladdingY") return;

  G4ThreeVector pos = track->GetPosition();
  G4double x = pos.x();

  if(geo::bendTheta > 0.01 * deg){
    x = atan2(x, pos.z()+geo::bendRadius) * geo::bendRadius;
  }

  fEventAction->LogXRay(x, pos.y());

  track->SetUserInformation(new G4VUserTrackInformation("XRayAlreadyInteracted"));

  //G4cout << "X-ray:\t" << x/mm << '\t' << pos.y()/mm << '\t' << pos.z()/mm << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
