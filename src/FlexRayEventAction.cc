#include "FlexRayEventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4Trajectory.hh"
#include "G4TrajectoryContainer.hh"
#include "G4ios.hh"

#include <cmath>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FlexRayEventAction::FlexRayEventAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayEventAction::~FlexRayEventAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void
FlexRayEventAction::BeginOfEventAction(const G4Event*)
{

  // This method is called at the beginning of each event.
  // We can use it to initialize variables, arrays, etc.
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void
FlexRayEventAction::EndOfEventAction(const G4Event* /*evt*/)
{

  // This method is called at the end of each event.
  // We can use it to make calculations or simply to write collected data
  // to the output file.
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
