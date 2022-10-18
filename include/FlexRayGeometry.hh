#ifndef FlexRayGeometry_h
#define FlexRayGeometry_h

#include "G4SystemOfUnits.hh"
#include "G4Types.hh"

namespace geo
{

  const G4bool squareFibers = true;
  const G4double fiberLength = 10 * cm;
  const G4double fiberRadius = 1 * mm / 2;
  const G4double fiberInnerRadius2 = 0.99 * fiberRadius; // inner radius of second cladding
  const G4double fiberInnerRadius1 = 0.96 * fiberRadius; // inner radius of first cladding

  // join to optical fiber: 0: none, 1: direct, 2: narrow, 3: two-hole
  const G4int fiberJoin = 0;
  const G4double fillGapLength = 50 * um; // used for type 3
  const G4double fillHolePosition = 80 * um; // used for type 3
  const G4double fillHoleRadius = 25 * um; // used for type 2 and type 3
  const G4double intermediateLength = 5 * mm; // used for type 2 and type 3.  these lengths are included in fiberLength above.
  const G4double endFiberLength = 5 * cm; // these lengths are included in fiberLength above.
  const G4double endFiberInnerRadius = 85 * um / 2; //used for type 1, type 2, type 3
  const G4double housingLength = 2.5 * cm; // used for type 3
  const G4double housingRadius = 320 * um / 2; // used for type 3

  const G4double fiberRoughness = 1; // 1 is perfectly smooth

  const G4int numFibers = 8;
  const G4double fiberSpacing = fiberRadius*2 + 0.4*mm;

  const G4int numLayers = 2;
  const G4double layerSpacing = fiberRadius*2 +0.4*mm;
  const G4double detectorSpacing = 5 * cm; // distance between 2-layer groups

  // ----------------
  //Either give a theta and calculate the radius or give a radius and calculate the theta.
  const G4double bendTheta = 0 * deg; // bend in the X direction, measured at the center plane between the two layers
  const G4double bendRadius = fiberLength / (bendTheta / rad); // radius calculated from a given theta

  //const G4double bendRadius = 0.15 * m; // radius measured at the center plane between the two layers
  //const G4double bendTheta = fiberLength / bendRadius; // theta calculated from a given radius
  // -----------------

  // helper functions to position layers
  //static G4double layerPosition(int layer) {return (layer + 0.5 - numLayers*0.5) * layerSpacing; }
  static G4double layerPosition(int layer) {return layer * layerSpacing + (layer/2)*detectorSpacing - (numLayers-1)/2*layerSpacing - ((numLayers-1)/2) * detectorSpacing; }
  static G4double layerRadius(int layer) { return bendRadius + layerPosition(layer); } // calculated from radius or theta

  const G4double foilThickness = -1 * um; // less than 0 for no foil, standard household Al foil is 16 um
}

#endif
