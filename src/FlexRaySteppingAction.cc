#include "FlexRaySteppingAction.hh"
#include "FlexRayEventAction.hh"
#include "FlexRayGeometry.hh"
#include "G4SteppingManager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

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
  G4Track* track = step->GetTrack();
  G4String volName = track->GetTouchable()->GetVolume()->GetName();

  // log first interaction of each X-ray in the detector

  if( track->GetParticleDefinition()->GetPDGEncoding() == 22 &&
      !(track->GetUserInformation() && track->GetUserInformation()->GetType() == "XRayAlreadyInteracted") &&
      (volName == "Core" || volName == "InnerCladding" || volName == "OuterCladdingX" || volName == "OuterCladdingY")){

    G4ThreeVector pos = track->GetPosition();
    G4double x = pos.x();

    if(geo::bendTheta > 0.01 * deg){
      x = atan2(x, pos.z()+geo::bendRadius) * geo::bendRadius;
    }

    fEventAction->LogXRay(step->GetPreStepPoint()->GetTotalEnergy(), x, pos.y());

    track->SetUserInformation(new G4VUserTrackInformation("XRayAlreadyInteracted"));
  }

  // find energy deposited. only counts ionizing energy in the fiber core.
  // this should be done with G4ElectronIonPair, but it uses a hardcoded list of materials that doesn't include CdTe.
  if (volName == "Core" && track->GetParticleDefinition()->GetPDGEncoding() == 11){
    G4int eHolePairs = 0;
    G4double ionizingEnergy = step->GetTotalEnergyDeposit() - step->GetNonIonizingEnergyDeposit();

    G4MaterialPropertiesTable *mpt = step->GetPreStepPoint()->GetMaterial()->GetMaterialPropertiesTable();
    if(mpt->ConstPropertyExists("EHOLEPRODUCTION")){
      G4double energyPerEHolePair = mpt->GetConstProperty("EHOLEPRODUCTION");
      //eHolePairs = ionizingEnergy/energyPerEHolePair; // this should be randomized: G4lrint(G4RandGamma::shoot(meanion*invFanoFactor,invFanoFactor)); invFanoFactor = 1.0/0.2
      eHolePairs = G4lrint(G4RandGamma::shoot(ionizingEnergy/energyPerEHolePair*5.0,5.0)); // why 5? copied from G4ElectronIonPair
    }


    fEventAction->AddEnergyDeposit(ionizingEnergy, eHolePairs);
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
