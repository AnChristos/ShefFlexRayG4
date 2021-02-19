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
  G4int pdg = track->GetParticleDefinition()->GetPDGEncoding(); // 22=gamma, -22 = optical photon
  if(pdg == 22) LogXRay(track);
  if(pdg == -22) LogOpticalPhoton(track);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FlexRayTrackingAction::LogOpticalPhoton(const G4Track* track)
{
  // get final position of track
  G4ThreeVector pos = track->GetPosition();
  
  // check that it ended at the end of a fiber
  if(fabs(pos.y()) < geo::fiberLength * 0.5 * 0.99){
    if(geo::bendTheta < 0.01 * deg && fabs(pos.x()) < geo::fiberLength * 0.5 * 0.99) return;
    if(geo::bendTheta > 0.01 * deg && fabs(atan(pos.x()/(pos.z()+geo::bendRadius))) < geo::bendTheta * 0.5 * 0.99) return;
  }

  const G4VTouchable *touch = track->GetTouchable();
  G4VPhysicalVolume *vol = touch->GetVolume();
  G4String name = touch->GetVolume()->GetName();
  
  if(name == "Core"){
    vol = touch->GetVolume(2);
  }else if(name == "InnerCladding"){
    vol = touch->GetVolume(1);
  }else if(name == "OuterCladdingX" || name == "OuterCladdingY"){
    //vol = vol;
  }else{
    return; // not in a fiber
  }

  name = vol->GetName();
  int detectorIndex = vol->GetCopyNo();

  double internX=0, internY=0; // internal x and y inside the fiber, to tell which SPAD was hit

  if(name == "OuterCladdingX"){ // it's an x measurement
    internX = pos.x() - vol->GetTranslation().x();
    internY = pos.z() - vol->GetTranslation().z();

    if(pos.y() > 0) detectorIndex |= (1<<8);
  }else{ // it's a y measurement
    internX = pos.y() - vol->GetTranslation().y();
    internY = pos.z() - vol->GetTranslation().z();

    if(geo::bendTheta > 0.01 * deg){
      if(pos.x() > 0) internY = (pos.z()+geo::bendRadius)*cos(geo::bendTheta/2) + pos.x()*sin(geo::bendTheta/2) - geo::bendRadiusY;
      if(pos.x() < 0) internY = (pos.z()+geo::bendRadius)*cos(geo::bendTheta/2) - pos.x()*sin(geo::bendTheta/2) - geo::bendRadiusY;
    }

    detectorIndex |= (1<<9);
    if(pos.x() > 0) detectorIndex |= (1<<8);
  }

  //G4cout << "LogOptical  " << detectorIndex << '\t' << track->GetKineticEnergy() << '\t' << track->GetGlobalTime() << G4endl;
  fEventAction->LogDetection(detectorIndex, track->GetKineticEnergy(), track->GetGlobalTime(), internX, internY);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FlexRayTrackingAction::LogXRay(const G4Track* track)
{
  //check that it hit a fiber
  G4String name = track->GetTouchable()->GetVolume()->GetName();
  if(name != "Core" && name != "InnerCladding" && name != "OuterCladdingX" && name != "OuterCladdingY") return;

  G4ThreeVector pos = track->GetPosition();
  G4double x = pos.x();

  if(geo::bendTheta > 0.01 * deg){
    x = atan2(x, pos.z()+geo::bendRadius) * geo::bendRadius;
  }

  fEventAction->LogXRay(pos.x(), pos.y());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
