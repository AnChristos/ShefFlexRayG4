/// \file FlexRayEventAction.hh
/// \brief Definition of the FlexRayEventAction class

#ifndef FlexRayEventAction_h
#define FlexRayEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class FlexRayRunAction;

/// Event action class
///

class FlexRayEventAction : public G4UserEventAction
{
public:
  FlexRayEventAction();
  virtual ~FlexRayEventAction();
  virtual void BeginOfEventAction(const G4Event* event) override;
  virtual void EndOfEventAction(const G4Event* event) override;

private:
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

