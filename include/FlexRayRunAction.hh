/// \file FlexRayRunAction.hh
/// \brief Definition of the FlexRayRunAction class

#ifndef FlexRayRunAction_h
#define FlexRayRunAction_h 

#include "G4UserRunAction.hh"
#include "g4analysis.hh"
#include "G4Run.hh"
#include "globals.hh"

/// Run action class
///

class FlexRayRunAction : public G4UserRunAction
{
public:
  FlexRayRunAction();
  virtual ~FlexRayRunAction();

  virtual void BeginOfRunAction(const G4Run* run) override;
  virtual void EndOfRunAction(const G4Run* run) override;

  G4AnalysisManager *GetAnalysisManager() {return fAnalysisManager;};

  void LogDetected() {fNumDetected++;};

private:
  G4int fNumDetected;
  G4AnalysisManager *fAnalysisManager;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

