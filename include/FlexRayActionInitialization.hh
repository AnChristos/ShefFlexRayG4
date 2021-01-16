/// \file FlexRayActionInitialization.hh
/// \brief Definition of the FlexRayActionInitialization class

#ifndef FlexRayActionInitialization_h
#define FlexRayActionInitialization_h

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class FlexRayActionInitialization : public G4VUserActionInitialization
{
public:
  FlexRayActionInitialization();
  virtual ~FlexRayActionInitialization();

  virtual void BuildForMaster() const override;
  virtual void Build() const override;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

