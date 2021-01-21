#include "FlexRayRunAction.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FlexRayRunAction::FlexRayRunAction()
: G4UserRunAction(),
  fNumDetected(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayRunAction::~FlexRayRunAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FlexRayRunAction::BeginOfRunAction(const G4Run*)
{
  fNumDetected = 0;

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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
