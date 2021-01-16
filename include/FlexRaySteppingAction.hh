/// \file FlexRaySteppingAction.hh
/// \brief Definition of the FlexRaySteppingAction class

#ifndef FlexRaySteppingAction_h
#define FlexRaySteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class FlexRayEventAction;

class G4LogicalVolume;

/// Stepping action class

class FlexRaySteppingAction : public G4UserSteppingAction
{
  public:
    FlexRaySteppingAction(FlexRayEventAction* eventAction);
    virtual ~FlexRaySteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*) override;

  private:
    FlexRayEventAction*  fEventAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
