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

  const G4double fiberRoughness = 0.93;

  const G4int numFibers = 4;
  const G4double fiberSpacing = fiberRadius*2 + 0.2 * mm;
  const G4double layerSpacing = fiberRadius*2 + 1 * mm;

  // ----------------
  //Either give a theta and calculate the radius or give a radius and calculate the theta.
  const G4double bendTheta = 0 * deg; // bend in the X direction, measured at the center plane between the two layers
  const G4double bendRadius = fiberLength / (bendTheta / rad); // radius calculated from a given theta

  //const G4double bendRadius = 0.15 * m; // radius measured at the center plane between the two layers
  //const G4double bendTheta = fiberLength / bendRadius; // theta calculated from a given radius

  const G4double bendRadiusX = bendRadius + layerSpacing/2; // calculated from radius or theta
  const G4double bendRadiusY = bendRadius - layerSpacing/2; // calculated from radius or theta
  // --------------------
}

#endif
