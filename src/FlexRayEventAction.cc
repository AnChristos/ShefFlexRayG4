#include "FlexRayEventAction.hh"

#include "G4Event.hh"

#include <cmath>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FlexRayEventAction::FlexRayEventAction(FlexRayRunAction *runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fDetected(false)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayEventAction::~FlexRayEventAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FlexRayEventAction::BeginOfEventAction(const G4Event*)
{
  fDetected = false;
  // This method is called at the beginning of each event.
  // We can use it to initialize variables, arrays, etc.
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FlexRayEventAction::EndOfEventAction(const G4Event*)
{
  // This method is called at the end of each event.
  // We can use it to make calculations or simply to write collected data
  // to the output file.

  if(fDetected)
    fRunAction->LogDetected();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FlexRayEventAction::LogDetection(G4double energy, G4double time)
{
  // add some histograms or a TTree of energy and time
  fDetected = true;
}
