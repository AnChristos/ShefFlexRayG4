#include "FlexRayEventAction.hh"

#include "G4Event.hh"
#include "G4SystemOfUnits.hh"

#include <cmath>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FlexRayEventAction::FlexRayEventAction(FlexRayRunAction *runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fDetected(false),
  fAnalysisManager(G4RootAnalysisManager::Instance())
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayEventAction::~FlexRayEventAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FlexRayEventAction::BeginOfEventAction(const G4Event* event)
{
  fEventID = event->GetEventID();
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

  if(fDetected) fRunAction->LogDetected();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FlexRayEventAction::LogDetection(int detectorIndex, G4double energy, G4double time)
{
  // add some histograms or a TTree of energy and time
  fDetected = true;
  fAnalysisManager->FillNtupleIColumn(0, fEventID);
  fAnalysisManager->FillNtupleIColumn(1, detectorIndex);
  fAnalysisManager->FillNtupleDColumn(2, energy/eV);
  fAnalysisManager->FillNtupleDColumn(3, time/ns);
  fAnalysisManager->AddNtupleRow();
}
