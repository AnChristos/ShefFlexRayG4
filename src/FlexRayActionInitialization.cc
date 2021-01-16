/// \file FlexRayActionInitialization.cc
/// \brief Implementation of the FlexRayActionInitialization class

#include "FlexRayActionInitialization.hh"
#include "FlexRayDetectorConstruction.hh"
#include "FlexRayEventAction.hh"
#include "FlexRayPrimaryGeneratorAction.hh"
#include "FlexRaySteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayActionInitialization::FlexRayActionInitialization(
  FlexRayDetectorConstruction* detector,
  FlexRayEventAction* eventAction)
  : G4VUserActionInitialization()
  , fDetector(detector)
  , fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayActionInitialization::~FlexRayActionInitialization() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void
FlexRayActionInitialization::BuildForMaster() const
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void
FlexRayActionInitialization::Build() const
{
  SetUserAction(new FlexRayPrimaryGeneratorAction(fDetector));
  SetUserAction(new FlexRayEventAction);
  SetUserAction(new FlexRaySteppingAction(fEventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
