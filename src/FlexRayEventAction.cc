#include "FlexRayEventAction.hh"

#include "G4EventManager.hh"

#include "G4Event.hh"
#include "G4SystemOfUnits.hh"

#include <cmath>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FlexRayEventAction::FlexRayEventAction(FlexRayRunAction *runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fDetected(0),
  fAnalysisManager(runAction->GetAnalysisManager())
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayEventAction::~FlexRayEventAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FlexRayEventAction::BeginOfEventAction(const G4Event* event)
{
  fEventID = event->GetEventID();
  fDetected = 0;
  fIgnoreXRays = false;
  // This method is called at the beginning of each event.
  // We can use it to initialize variables, arrays, etc.
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FlexRayEventAction::EndOfEventAction(const G4Event*)
{
  // This method is called at the end of each event.
  // We can use it to make calculations or simply to write collected data
  // to the output file.

  if(fDetected > 0) fRunAction->LogDetected();
  if(fDetected > 4 && (fDetected != 8 && fDetected != 12)) fpEventManager->KeepTheCurrentEvent();
  fAnalysisManager->FillH1(0, fDetected);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FlexRayEventAction::LogDetection(int detectorIndex, G4double energy, G4double time, G4double x, G4double y)
{
  // add some histograms or a TTree of energy and time
  fDetected |= (1 << (detectorIndex>>8));
  fAnalysisManager->FillNtupleIColumn(0, 0, fEventID);
  fAnalysisManager->FillNtupleIColumn(0, 1, detectorIndex);
  fAnalysisManager->FillNtupleDColumn(0, 2, energy/eV);
  fAnalysisManager->FillNtupleDColumn(0, 3, time/ns);
  fAnalysisManager->FillNtupleDColumn(0, 4, x/mm);
  fAnalysisManager->FillNtupleDColumn(0, 5, y/mm);
  fAnalysisManager->AddNtupleRow(0);
}

void FlexRayEventAction::LogXRay(G4double energy, G4double x, G4double y)
{
  if(fIgnoreXRays) return;

  fAnalysisManager->FillNtupleIColumn(1, 0, fEventID);
  fAnalysisManager->FillNtupleDColumn(1, 1, energy/eV);
  fAnalysisManager->FillNtupleDColumn(1, 2, x/mm);
  fAnalysisManager->FillNtupleDColumn(1, 3, y/mm);
  fAnalysisManager->AddNtupleRow(1);

  fIgnoreXRays = true; // only record first X-ray in event, not secondaries after scintillation.
}
