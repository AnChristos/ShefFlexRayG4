/// \file FlexRayPrimaryGeneratorAction.hh
/// \brief Definition of the FlexRayPrimaryGeneratorAction class

#ifndef FlexRayPrimaryGeneratorAction_h
#define FlexRayPrimaryGeneratorAction_h

#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

#include <memory>

class G4ParticleGun;
class G4Event;
class FlexRayDetectorConstruction;

class FlexRayPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  FlexRayPrimaryGeneratorAction(FlexRayDetectorConstruction* detector);
  virtual ~FlexRayPrimaryGeneratorAction();

  // method from the base class
  virtual void GeneratePrimaries(G4Event* event) override;

  // method to access particle gun
  const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

private:
  // owning pointer a to G4 gun class
  G4ParticleGun* fParticleGun;
  // pointer to the FlexRayDetector
  FlexRayDetectorConstruction* fFlexRayDetector;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
