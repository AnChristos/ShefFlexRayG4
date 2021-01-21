/// \file FlexRayEventAction.hh
/// \brief Definition of the FlexRayEventAction class

#ifndef FlexRayEventAction_h
#define FlexRayEventAction_h 

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "FlexRayRunAction.hh"
#include "globals.hh"

/// Event action class
///

class FlexRayEventAction : public G4UserEventAction
{
public:
  FlexRayEventAction(FlexRayRunAction *runAction);
  virtual ~FlexRayEventAction();
  virtual void BeginOfEventAction(const G4Event* event) override;
  virtual void EndOfEventAction(const G4Event* event) override;

  void LogDetection(G4double energy, G4double time);

private:
  FlexRayRunAction *fRunAction;
  G4bool fDetected;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

