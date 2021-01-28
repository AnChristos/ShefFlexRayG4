#ifndef FlexRayGeometry_h
#define FlexRayGeometry_h

#include "G4SystemOfUnits.hh"
#include "G4Types.hh"

namespace geo
{
  const G4double fiberLength = 0.2 * m;
  const G4double fiberRadius = 1 * mm;
  const G4double fiberInnerRadius2 = 0.99 * fiberRadius; // inner radius of second cladding
  const G4double fiberInnerRadius1 = 0.96 * fiberRadius; // inner radius of first cladding
  const G4int numFibers = 4;
  const G4double fiberSpacing = fiberRadius*2 + 0.2 * mm;
  const G4double layerSpacing = fiberRadius*2 + 1 * mm;
}

#endif
