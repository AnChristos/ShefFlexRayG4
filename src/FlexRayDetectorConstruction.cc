#include "FlexRayGeometry.hh"
#include "FlexRayDetectorConstruction.hh"
#include "FlexRayMaterials.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4GeometryManager.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4ios.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4VPhysicalVolume.hh"
#include "G4tgbVolumeMgr.hh"

using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayDetectorConstruction::FlexRayDetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayDetectorConstruction::~FlexRayDetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume*
FlexRayDetectorConstruction::Construct()
{

  /*
   * Definition of materials
   */
  G4NistManager* sNistMan = G4NistManager::Instance();
  //sNistMan->SetVerbose(2);

  G4Material* Air = sNistMan->FindOrBuildMaterial("G4_AIR");

  FlexRayMaterials materials;

  // print fr materials (for debug purposes mainly)
  G4cout << G4endl << "The materials defined are : " << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  G4cout << "Material properties table for BCF10 core : " << G4endl;
  materials.BCF10->GetMaterialPropertiesTable()->DumpTable();
  G4cout << "Material properties table for BCF10 clad1 : " << G4endl;
  materials.PMMA149->GetMaterialPropertiesTable()->DumpTable();
  G4cout << "Material properties table for BCF10 clad2 : " << G4endl;
  materials.PMMA142->GetMaterialPropertiesTable()->DumpTable();

  /*
   * Construct the enclosing world
   */
  G4double worldLength = 1.0 * m;
  G4double halfWorldLength = 0.5 * worldLength;
  G4Box* solidWorld =
    new G4Box("world", halfWorldLength, halfWorldLength, halfWorldLength);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, Air, "World");

  //  Must place the World Physical volume unrotated at (0,0,0).
  G4VPhysicalVolume* physiWorld =
    new G4PVPlacement(0,               // no rotation
                      G4ThreeVector(), // at (0,0,0)
                      logicWorld,      // its logical volume
                      "World",         // its name
                      0,               // its mother  volume
                      false,           // no boolean operations
                      0);              // copy number

  // Visualization attributes
  G4VisAttributes* WorldVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
  // Give visualization attributes to the logical volumes
  logicWorld->SetVisAttributes(WorldVisAtt);

  /*
   * Construct the scintillating fiber geometry
   */
  G4VSolid *fiberCore, *fiberClad1, *fiberClad2;

  G4double coreLength = geo::fiberLength;
  if(geo::fiberJoin == 1) coreLength -= 2*(geo::endFiberLength); // direct join
  if(geo::fiberJoin == 2) coreLength -= 2*(geo::intermediateLength + geo::endFiberLength); // narrow fiber join
  if(geo::fiberJoin == 3) coreLength -= 2*(geo::fillGapLength + geo::intermediateLength + geo::endFiberLength); // two-hole join

  if(geo::squareFibers){
    fiberClad2 = new G4Box("OuterCladding", geo::fiberRadius, geo::fiberRadius, geo::fiberLength/2);
    fiberClad1 = new G4Box("InnerCladding", geo::fiberInnerRadius2, geo::fiberInnerRadius2, geo::fiberLength/2);
    fiberCore = new G4Box("Core", geo::fiberInnerRadius1, geo::fiberInnerRadius1, coreLength/2);
  }else{
    fiberClad2 = new G4Tubs("OuterCladding", 0, geo::fiberRadius, geo::fiberLength/2, 0 * deg, 360 * deg);
    fiberClad1 = new G4Tubs("InnerCladding", 0, geo::fiberInnerRadius2, geo::fiberLength/2, 0 * deg, 360 * deg);
    fiberCore = new G4Tubs("Core", 0, geo::fiberInnerRadius1, coreLength/2, 0 * deg, 360 * deg);
  }

  G4LogicalVolume *logicFiberClad2 = new G4LogicalVolume(fiberClad2, materials.Clad2, "OuterCladding");
  G4LogicalVolume *logicFiberClad1 = new G4LogicalVolume(fiberClad1, materials.Clad1, "InnerCladding");
  G4LogicalVolume *logicFiberCore = new G4LogicalVolume(fiberCore, materials.Core, "Core");

  // construct full fiber (place IC and core inside OC)
  G4VPhysicalVolume *PhysClad1 = new G4PVPlacement(0, G4ThreeVector(), logicFiberClad1, "InnerCladding", logicFiberClad2, false, 0,true);
  G4VPhysicalVolume *PhysCore = new G4PVPlacement(0, G4ThreeVector(), logicFiberCore, "Core", logicFiberClad1, false, 0,true);

  // a rough optical surface helps to match the trapping efficiency in the catalog.  Might need to discuss with SGC.
  G4OpticalSurface *opSurface = new G4OpticalSurface("OpSurface", glisur, ground, dielectric_dielectric, geo::fiberRoughness);
  new G4LogicalBorderSurface("SurfClad1Out", PhysCore, PhysClad1, opSurface);
  new G4LogicalBorderSurface("SurfClad1In", PhysClad1, PhysCore, opSurface);

  if(geo::fiberJoin == 1){
    G4VSolid *endCore;
    endCore = new G4Tubs("EndCore", 0, geo::endFiberInnerRadius, (geo::endFiberLength)/2, 0*deg, 360*deg);
    G4LogicalVolume *logicEndCore = new G4LogicalVolume(endCore, materials.EndFiberCore, "Core");
    new G4PVPlacement(0, G4ThreeVector(0, 0, (geo::fiberLength - geo::endFiberLength)/2), logicEndCore, "Core", logicFiberClad1, false, 0, true);
    new G4PVPlacement(0, G4ThreeVector(0, 0, -(geo::fiberLength - geo::endFiberLength)/2), logicEndCore, "Core", logicFiberClad1, false, 0, true);
  }
  if(geo::fiberJoin == 2){
    G4VSolid *fillHole, *airGap, *airMeniscus, *endCore;

    fillHole = new G4Tubs("FillHole", 0, geo::fillHoleRadius, geo::intermediateLength/2, 0*deg, 360*deg);
    airGap = new G4Tubs("AirGap", 0, geo::fillHoleRadius*1.01, geo::intermediateLength/4, 0*deg, 360*deg);
    airMeniscus = new G4Sphere("AirMeniscus", 0, geo::fillHoleRadius*1.01, 0*deg, 360*deg, 90*deg, 180*deg);
    endCore = new G4Tubs("EndCore", 0, geo::endFiberInnerRadius, geo::endFiberLength/2, 0*deg, 360*deg);

    G4LogicalVolume *logicFillHole = new G4LogicalVolume(fillHole, materials.Core, "FillHole");
    G4LogicalVolume *logicAirGap = new G4LogicalVolume(airGap, materials.Air, "AirGap");
    G4LogicalVolume *logicAirMeniscus = new G4LogicalVolume(airMeniscus, materials.Air, "AirGap");
    G4LogicalVolume *logicEndCore = new G4LogicalVolume(endCore, materials.EndFiberCore, "Core");

    new G4PVPlacement(0, G4ThreeVector(0, 0, (geo::intermediateLength)/4), logicAirGap, "AirGap", logicFillHole, false, 0, true);
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicAirMeniscus, "AirGap", logicFillHole, false, 0, true);

    new G4PVPlacement(0, G4ThreeVector(0, 0, coreLength/2 + geo::intermediateLength/2), logicFillHole, "FillHole", logicFiberClad1, false, 0, true);

    G4RotationMatrix *flip = new G4RotationMatrix();
    flip->rotateX(180*deg);
    new G4PVPlacement(flip, G4ThreeVector(0, 0, -coreLength/2 - geo::intermediateLength/2), logicFillHole, "FillHole", logicFiberClad1, false, 0, true);

    new G4PVPlacement(0, G4ThreeVector(0, 0, (geo::fiberLength - geo::endFiberLength)/2), logicEndCore, "Core", logicFiberClad1, false, 0, true);
    new G4PVPlacement(0, G4ThreeVector(0, 0, -(geo::fiberLength - geo::endFiberLength)/2), logicEndCore, "Core", logicFiberClad1, false, 0, true);
  }
  if(geo::fiberJoin == 3){ // two-hole join
    G4VSolid *liquidGap, *housingCapillary, *fillHole, *airGap, *airMeniscus, *endCore;

    liquidGap = new G4Tubs("LiquidGap", 0, geo::fiberInnerRadius2, geo::fillGapLength/2, 0*deg, 360*deg);
    housingCapillary = new G4Tubs("Housing", geo::fiberInnerRadius2, geo::housingRadius, geo::housingLength, 0*deg, 360*deg);
    fillHole = new G4Tubs("FillHole", 0, geo::fillHoleRadius, geo::intermediateLength/2, 0*deg, 360*deg);
    airGap = new G4Tubs("AirGap", 0, geo::fillHoleRadius*1.01, geo::intermediateLength/4, 0*deg, 360*deg);
    airMeniscus = new G4Sphere("AirMeniscus", 0, geo::fillHoleRadius*1.01, 0*deg, 360*deg, 90*deg, 180*deg);
    endCore = new G4Tubs("EndCore", 0, geo::endFiberInnerRadius, (geo::endFiberLength + geo::intermediateLength)/2, 0*deg, 360*deg);

    G4LogicalVolume *logicLiquidGap = new G4LogicalVolume(liquidGap, materials.Core, "LiquidGap");
    G4LogicalVolume *logicHousingCapillary = new G4LogicalVolume(housingCapillary, materials.Clad1, "Housing");
    G4LogicalVolume *logicFillHole = new G4LogicalVolume(fillHole, materials.Core, "FillHole");
    G4LogicalVolume *logicAirGap = new G4LogicalVolume(airGap, materials.Air, "AirGap");
    G4LogicalVolume *logicAirMeniscus = new G4LogicalVolume(airMeniscus, materials.Air, "AirGap");
    G4LogicalVolume *logicEndCore = new G4LogicalVolume(endCore, materials.EndFiberCore, "Core");

    new G4PVPlacement(0, G4ThreeVector(0, 0, (coreLength+geo::fillGapLength)/2), logicLiquidGap, "LiquidGap", logicFiberClad1, false, 0, true);
    new G4PVPlacement(0, G4ThreeVector(0, 0, -(coreLength+geo::fillGapLength)/2), logicLiquidGap, "LiquidGap", logicFiberClad1, false, 0, true);

    new G4PVPlacement(0, G4ThreeVector(0, 0, (coreLength+geo::fillGapLength)/2), logicHousingCapillary, "Housing", logicFiberClad2, false, 0, true);
    new G4PVPlacement(0, G4ThreeVector(0, 0, -(coreLength+geo::fillGapLength)/2), logicHousingCapillary, "Housing", logicFiberClad2, false, 0, true);

    new G4PVPlacement(0, G4ThreeVector(0, 0, (geo::intermediateLength)/4), logicAirGap, "AirGap", logicFillHole, false, 0, true);
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicAirMeniscus, "AirGap", logicFillHole, false, 0, true);

    new G4PVPlacement(0, G4ThreeVector(0, geo::fillHolePosition, coreLength/2 + geo::fillGapLength + geo::intermediateLength/2), logicFillHole, "FillHole", logicFiberClad1, false, 0, true);
    new G4PVPlacement(0, G4ThreeVector(0, -geo::fillHolePosition, coreLength/2 + geo::fillGapLength + geo::intermediateLength/2), logicFillHole, "FillHole", logicFiberClad1, false, 0, true);

    G4RotationMatrix *flip = new G4RotationMatrix();
    flip->rotateX(180*deg);
    new G4PVPlacement(flip, G4ThreeVector(0, geo::fillHolePosition, -coreLength/2 - geo::fillGapLength - geo::intermediateLength/2), logicFillHole, "FillHole", logicFiberClad1, false, 0, true);
    new G4PVPlacement(flip, G4ThreeVector(0, -geo::fillHolePosition, -coreLength/2 - geo::fillGapLength - geo::intermediateLength/2), logicFillHole, "FillHole", logicFiberClad1, false, 0, true);

    new G4PVPlacement(0, G4ThreeVector(0, 0, (geo::fiberLength - geo::intermediateLength - geo::endFiberLength)/2), logicEndCore, "Core", logicFiberClad1, false, 0, true);
    new G4PVPlacement(0, G4ThreeVector(0, 0, -(geo::fiberLength - geo::intermediateLength - geo::endFiberLength)/2), logicEndCore, "Core", logicFiberClad1, false, 0, true);
  }

  G4LogicalVolume *logicFiberClad2Y = logicFiberClad2; // this will be replaced if the detector is bent.

  for(int iLayer=0; iLayer<geo::numLayers; iLayer+=2){
    // first add y-measuring fibers
    if(geo::bendTheta > 0.01 * deg){ // replace y-measuring fibers with bent fibers
      G4VSolid *fiberClad2Y, *fiberClad1Y, *fiberCoreY;

      if(geo::squareFibers){
        fiberClad2Y = new G4Tubs("OuterCladding", geo::layerRadius(iLayer) - geo::fiberRadius, geo::layerRadius(iLayer) + geo::fiberRadius, geo::fiberRadius, 90*deg - geo::bendTheta/2, geo::bendTheta);
        fiberClad1Y = new G4Tubs("InnerCladding", geo::layerRadius(iLayer) - geo::fiberInnerRadius2, geo::layerRadius(iLayer) + geo::fiberInnerRadius2, geo::fiberInnerRadius2, 90*deg - geo::bendTheta/2, geo::bendTheta);
        fiberCoreY = new G4Tubs("Core", geo::layerRadius(iLayer) - geo::fiberInnerRadius1, geo::layerRadius(iLayer) + geo::fiberInnerRadius1, geo::fiberInnerRadius1, 90*deg - geo::bendTheta/2, geo::bendTheta);
      }else{
        fiberClad2Y = new G4Torus("OuterCladding", 0, geo::fiberRadius, geo::layerRadius(iLayer), 90*deg - geo::bendTheta/2, geo::bendTheta);
        fiberClad1Y = new G4Torus("InnerCladding", 0, geo::fiberInnerRadius2, geo::layerRadius(iLayer), 90*deg - geo::bendTheta/2, geo::bendTheta);
        fiberCoreY = new G4Torus("Core", 0, geo::fiberInnerRadius1, geo::layerRadius(iLayer), 90*deg - geo::bendTheta/2, geo::bendTheta);
      }

      logicFiberClad2Y = new G4LogicalVolume(fiberClad2Y, materials.Clad2, "OuterCladding");
      G4LogicalVolume *logicFiberClad1Y = new G4LogicalVolume(fiberClad1Y, materials.Clad1, "InnerCladding");
      G4LogicalVolume *logicFiberCoreY = new G4LogicalVolume(fiberCoreY, materials.Core, "Core");

      G4VPhysicalVolume *PhysClad1Y = new G4PVPlacement(0, G4ThreeVector(), logicFiberClad1Y, "InnerCladding", logicFiberClad2Y, false, 0,true);
      G4VPhysicalVolume *PhysCoreY = new G4PVPlacement(0, G4ThreeVector(), logicFiberCoreY, "Core", logicFiberClad1Y, false, 0,true);

      new G4LogicalBorderSurface("SurfClad1OutY", PhysCoreY, PhysClad1Y, opSurface);
      new G4LogicalBorderSurface("SurfClad1InY", PhysClad1Y, PhysCoreY, opSurface);
    }

    G4RotationMatrix *yrot = new G4RotationMatrix();
    if(geo::bendTheta > 0.01 * deg){ //bent
      yrot->rotateX(-90*deg);
    }else{ //flat
      yrot->rotateY(90*deg);
    }

    for(G4int i=0; i<geo::numFibers; i++){
      G4double offset = (-geo::numFibers * 0.5 + i + 0.5) * geo::fiberSpacing;

      G4ThreeVector ypos(0, offset, geo::layerPosition(iLayer));

      if(geo::bendTheta > 0.01 * deg){
        ypos = G4ThreeVector(0, offset, -geo::bendRadius);
      }

      new G4PVPlacement(yrot, ypos, logicFiberClad2Y, "OuterCladdingY", logicWorld, false, iLayer/2*geo::numFibers + i, true);
    }

    if(geo::numLayers == iLayer+1){ // odd number of layers, skip x fibers
      break;
    }

    // then add x-measuring fibers
    G4RotationMatrix *xrot = new G4RotationMatrix();
    xrot->rotateX(-90*deg);
    
    for(G4int i=0; i<geo::numFibers; i++){
      G4double offset = (-geo::numFibers * 0.5 + i + 0.5) * geo::fiberSpacing;

      G4ThreeVector xpos(offset, 0, geo::layerPosition(iLayer + 1));

      if(geo::bendTheta > 0.01 * deg){
	xpos = G4ThreeVector(geo::layerRadius(iLayer+1)*sin(offset/geo::bendRadius), 0, geo::layerRadius(iLayer+1)*cos(offset/geo::bendRadius) - geo::bendRadius);
        xrot = new G4RotationMatrix();
        xrot->rotateX( -90*deg );
        xrot->rotateZ( offset / geo::bendRadius );
      }

      new G4PVPlacement(xrot, xpos, logicFiberClad2, "OuterCladdingX", logicWorld, false, iLayer/2*geo::numFibers + i, true); // need the if there in case we're building an odd number of layers
    }
  }

  // add image target
  if(geo::imageTarget != ""){
    G4tgbVolumeMgr::GetInstance()->AddTextFile(geo::imageTarget);
    G4VPhysicalVolume *target = G4tgbVolumeMgr::GetInstance()->ReadAndConstructDetector();
    target->SetTranslation(G4ThreeVector(geo::targetX, geo::targetY, geo::targetZ));
    logicWorld->AddDaughter(target);

    //G4cout << "parent of target: " << G4endl;
    //G4cout << target->GetMotherLogical()->GetName() << '\t' << target->GetTranslation() << '\t' << target->GetRotation() << G4endl;
  }

  // print out fiber location for testing
  G4double offset = (geo::numFibers % 2) == 0 ? geo::fiberSpacing/2 : 0;

  G4cout << "Fiber Center: " << offset/mm << " " << offset/mm << " " << -geo::layerSpacing*0.5/mm << " mm" << G4endl;
  G4cout << "Fiber Edge: " << offset/mm << " " << offset/mm + geo::fiberInnerRadius1*0.99 << " " << -geo::layerSpacing*0.5/mm << " mm" << G4endl;

  // Return world
  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
