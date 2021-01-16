#include "FlexRaySteppingAction.hh"
#include "FlexRayEventAction.hh"
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
void
FlexRaySteppingAction::UserSteppingAction(const G4Step* step)
{
  // G4Track provides lots of useful info. See
  // G4Track.hh for all available methods
  G4Track* thisTrack = step->GetTrack();
  G4VPhysicalVolume* theVolume = thisTrack->GetVolume();
  G4String thisVolumeName = theVolume->GetName();
  const G4ParticleDefinition* thisParticle = thisTrack->GetParticleDefinition();
  G4String thisParticleName = thisParticle->GetParticleName();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
