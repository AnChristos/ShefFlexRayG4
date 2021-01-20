#include "FlexRayPhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "FTFP_BERT.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4OpticalPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayPhysicsList::FlexRayPhysicsList()
  : G4VModularPhysicsList()
{
  SetVerboseLevel(1);

  // Default physics
  RegisterPhysics(new G4DecayPhysics());

  // EM physics
  G4EmStandardPhysics_option4* emPhysics = new G4EmStandardPhysics_option4();
  emPhysics->SetVerboseLevel(2);
  RegisterPhysics(emPhysics);

  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // Optical Physics
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  opticalPhysics->SetVerboseLevel(2);
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
