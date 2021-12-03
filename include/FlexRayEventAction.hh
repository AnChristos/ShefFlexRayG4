/// \file FlexRayEventAction.hh
/// \brief Definition of the FlexRayEventAction class

#ifndef FlexRayEventAction_h
#define FlexRayEventAction_h 

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "FlexRayRunAction.hh"
#include "globals.hh"
#include <map>

/// Event action class
///

class FlexRayEventAction : public G4UserEventAction
{
public:
  FlexRayEventAction(FlexRayRunAction *runAction);
  virtual ~FlexRayEventAction();
  virtual void BeginOfEventAction(const G4Event* event) override;
  virtual void EndOfEventAction(const G4Event* event) override;

  void LogDetection(int detectorIndex, G4double energy, G4double time, G4double x, G4double y);
  void LogXRay(G4double energy, G4double x, G4double y);
  void AddEnergyDeposit(G4double energy, G4int eholes);

private:
  FlexRayRunAction *fRunAction;
  G4int fDetected;
  G4AnalysisManager *fAnalysisManager;
  G4int fEventID;
  G4bool fIgnoreXRays;
  G4double fEnergyDeposited;
  G4int fEHoles;

  std::map<G4int, G4int> mHitPhotons;
  std::map<G4int, G4double> mHitEnergy;
  std::map<G4int, G4double> mHitTime;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

