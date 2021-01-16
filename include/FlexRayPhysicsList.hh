#ifndef FlexRayPhysicsList_h
#define FlexRayPhysicsList_h

#include "G4VModularPhysicsList.hh"

/// Modular physics list
///
/// includes
///
/// - G4DecayPhysics
/// - G4RadioactiveDecayPhysics
/// - G4EmStandardPhysics
/// - G4OpticalPhysics

class FlexRayPhysicsList: public G4VModularPhysicsList
{
public:
  FlexRayPhysicsList();
  virtual ~FlexRayPhysicsList();

  virtual void SetCuts() override;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

