#include "FlexRayPrimaryGeneratorAction.hh"
#include "FlexRayDetectorConstruction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4VPhysicalVolume.hh"
#include "Randomize.hh"
#include "globals.hh"

#include <memory>
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayPrimaryGeneratorAction::FlexRayPrimaryGeneratorAction(
  FlexRayDetectorConstruction* detector)
  : fFlexRayDetector(detector)
{
  fParticleGun = new G4ParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayPrimaryGeneratorAction::~FlexRayPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void
FlexRayPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  fParticleGun->GeneratePrimaryVertex(event);
}
