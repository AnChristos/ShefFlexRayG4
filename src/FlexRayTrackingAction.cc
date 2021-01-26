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

void FlexRayTrackingAction::Initialize(FlexRayDetectorConstruction *detector){
  fFiberSpacing = detector->GetFiberSpacing();
  fLayerSpacing = detector->GetLayerSpacing();
  fFiberLength = detector->GetFiberLength();
  fNumFibers = detector->GetNumFibers();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FlexRayTrackingAction::PostUserTrackingAction(const G4Track* track)
{
  // get final position of track
  G4ThreeVector pos = track->GetPosition();
  
  // this should probably be done with G4SensitiveDetectors instead of raw position once we know more about what we're simulating
  if(fabs(pos.x()) < fFiberLength*0.45 && fabs(pos.y()) < fFiberLength*0.45) return;
  
  G4String vol = track->GetVolume()->GetName();
  if(vol != "Core" && vol != "InnerCladding" && vol != "OuterCladding") return;

  int detectorIndex=0;

  double internX=0, internY=0; // internal x and y inside the fiber, to tell which SPAD was hit

  if(fabs(pos.y()) > fFiberLength*0.45){ // it's an x measurement
    detectorIndex = pos.x()/fFiberSpacing + fNumFibers*0.5;

    internX = pos.x() - (detectorIndex - (fNumFibers-1)*0.5) * fFiberSpacing;
    internY = pos.z() - fLayerSpacing*0.5;

    if(pos.y() > 0) detectorIndex |= (1<<8);
  }else{ // it's a y measurement
    detectorIndex = pos.y()/fFiberSpacing + fNumFibers*0.5;

    internX = pos.y() - (detectorIndex - (fNumFibers-1)*0.5) * fFiberSpacing;
    internY = pos.z() + fLayerSpacing*0.5;

    detectorIndex |= (1<<9);
    if(pos.x() > 0) detectorIndex |= (1<<8);
  }

  fEventAction->LogDetection(detectorIndex, track->GetKineticEnergy(), track->GetGlobalTime(), internX, internY);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
