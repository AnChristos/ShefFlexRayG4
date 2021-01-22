#include "FlexRayRunAction.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FlexRayRunAction::FlexRayRunAction()
: G4UserRunAction(),
  fNumDetected(0),
  fAnalysisManager(G4RootAnalysisManager::Instance())
{
  fAnalysisManager->CreateNtuple("Photons", "Optical Photons Detected");
  fAnalysisManager->CreateNtupleIColumn("event");
  fAnalysisManager->CreateNtupleIColumn("detector");
  fAnalysisManager->CreateNtupleDColumn("energy");
  fAnalysisManager->CreateNtupleDColumn("time");
  fAnalysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayRunAction::~FlexRayRunAction()
{
  delete fAnalysisManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FlexRayRunAction::BeginOfRunAction(const G4Run*)
{
  fNumDetected = 0;
  fAnalysisManager->OpenFile("FlexRay.root");
  // This method is called at the beginning of each run.
  // We can use it to initialize variables, arrays, etc.
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FlexRayRunAction::EndOfRunAction(const G4Run* run)
{
  // This method is called at the end of each run.
  // We can use it to make calculations or simply to write collected data
  // to the output file.

  G4int nEvents = run->GetNumberOfEvent();

  G4cout << G4endl << "--------------------End of Run-----------------------" << G4endl;
  G4cout << nEvents << " Events, " << fNumDetected << " Detected" << G4endl;
  G4cout << "Efficiency: " << (G4double)fNumDetected / nEvents << G4endl;
  G4cout << G4endl << "------------------------------------------------------------" << G4endl;

  fAnalysisManager->Write();
  fAnalysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
