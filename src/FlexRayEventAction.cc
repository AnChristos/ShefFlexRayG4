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
  fEnergyDeposited = 0;
  fEHoles = 0;

  mHitTime.clear();
  mHitEnergy.clear();
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
  if(fEnergyDeposited > 0) fAnalysisManager->FillH1(1, fEnergyDeposited/keV);
  if(fEHoles > 0) fAnalysisManager->FillH1(2, fEHoles);

  for(std::map<G4int, G4double>::iterator i = mHitTime.begin(); i != mHitTime.end(); i++){
    G4int det = i->first;
    fAnalysisManager->FillNtupleIColumn(1, 0, fEventID);
    fAnalysisManager->FillNtupleIColumn(1, 1, det);
    fAnalysisManager->FillNtupleIColumn(1, 2, mHitPhotons[det]);
    fAnalysisManager->FillNtupleDColumn(1, 3, mHitEnergy[det]/eV);
    fAnalysisManager->FillNtupleDColumn(1, 4, mHitTime[det]/ns);

    fAnalysisManager->AddNtupleRow(1);
  }
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

  // add up energies in each SiPM
  // add quantum efficiency number here instead of in analysis step?
  if(mHitPhotons[detectorIndex] == 0){
    mHitPhotons[detectorIndex] = 1;
    mHitEnergy[detectorIndex] = energy;
    mHitTime[detectorIndex] = time;
  }else{
    mHitPhotons[detectorIndex] += 1;
    mHitEnergy[detectorIndex] += energy;
  }
}

void FlexRayEventAction::LogXRay(G4double energy, G4double x, G4double y)
{
  if(fIgnoreXRays) return;

  fAnalysisManager->FillNtupleIColumn(2, 0, fEventID);
  fAnalysisManager->FillNtupleDColumn(2, 1, energy/eV);
  fAnalysisManager->FillNtupleDColumn(2, 2, x/mm);
  fAnalysisManager->FillNtupleDColumn(2, 3, y/mm);
  fAnalysisManager->AddNtupleRow(2);

  fIgnoreXRays = true; // only record first X-ray in event, not secondaries after scintillation.
}

void FlexRayEventAction::AddEnergyDeposit(G4double energy, G4int eholes){
  fEnergyDeposited += energy;
  fEHoles += eholes;
}
