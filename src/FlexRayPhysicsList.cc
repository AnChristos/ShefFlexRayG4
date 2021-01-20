#include "FlexRayPhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayPhysicsList::FlexRayPhysicsList()
  : G4VModularPhysicsList()
{
  // Default physics
  RegisterPhysics(new G4DecayPhysics());

  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // EM physics
  G4EmStandardPhysics_option4* emPhysics = new G4EmStandardPhysics_option4();
  RegisterPhysics(emPhysics);

  // Optical Physics
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  //  opticalPhysics->SetVerboseLevel(2);
  RegisterPhysics(opticalPhysics);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayPhysicsList::~FlexRayPhysicsList() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void
FlexRayPhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}
