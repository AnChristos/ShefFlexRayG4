#include "FlexRayRunAction.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FlexRayRunAction::FlexRayRunAction()
: G4UserRunAction(),
  fNumDetected(0),
  fAnalysisManager(G4Analysis::ManagerInstance("csv")) // also try "csv", "root", "xml", "hdf5"
{
  fAnalysisManager->CreateNtuple("Photons", "Optical Photons Detected");
  fAnalysisManager->CreateNtupleIColumn(0, "event");
  fAnalysisManager->CreateNtupleIColumn(0, "detector");
  fAnalysisManager->CreateNtupleDColumn(0, "energy");
  fAnalysisManager->CreateNtupleDColumn(0, "time");
  fAnalysisManager->CreateNtupleDColumn(0, "x");
  fAnalysisManager->CreateNtupleDColumn(0, "y");
  fAnalysisManager->FinishNtuple(0);

  // detectors lit up from a single x-ray:
  // 0: none
  // 1, 2, 4, 8: one end of one fiber
  // 3: both ends of an x-measurement
  // 12: both ends of a y-measurement
  // 15: all four sides
  fAnalysisManager->CreateH1("Detectors", "Detectors Hit", 16, -0.5, 15.5);

  // MC truth
  fAnalysisManager->CreateNtuple("Xrays", "X-Ray hits");
  fAnalysisManager->CreateNtupleIColumn(1, "event");
  //fAnalysisManager->CreateNtupleDColumn(1, "energy");
  fAnalysisManager->CreateNtupleDColumn(1, "x");
  fAnalysisManager->CreateNtupleDColumn(1, "y");
  fAnalysisManager->FinishNtuple(1);
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
  fAnalysisManager->OpenFile("FlexRay");
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
