#include "FlexRayMaterials.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "globals.hh"

FlexRayMaterials::FlexRayMaterials(){
  // Initialize the materials
  G4NistManager* sNistMan = G4NistManager::Instance();

  // Cladding Materials
  std::vector<G4String> PMMAelements = { "C", "H", "O" };
  std::vector<G4int> PMMAnatoms = { 5, 8, 2 };
  G4double PMMAdensity = 1.190 * g / cm3;

  PMMA149 = sNistMan->ConstructNewMaterial("PMMAClad149", PMMAelements, PMMAnatoms, PMMAdensity);
  G4MaterialPropertiesTable* MPT_PMMA149 = new G4MaterialPropertiesTable();
  FillConstProperty(MPT_PMMA149, "RINDEX", 1.49);
  FillConstProperty(MPT_PMMA149, "ABSLENGTH", 5.40 * m);
  PMMA149->SetMaterialPropertiesTable(MPT_PMMA149);

  PMMA142 = sNistMan->ConstructNewMaterial("PMMAClad142", PMMAelements, PMMAnatoms, PMMAdensity);
  G4MaterialPropertiesTable* MPT_PMMA142 = new G4MaterialPropertiesTable();
  FillConstProperty(MPT_PMMA142, "RINDEX", 1.42);
  FillConstProperty(MPT_PMMA142, "ABSLENGTH", 5.40 * m);
  PMMA142->SetMaterialPropertiesTable(MPT_PMMA142);

  // Scintillating Materials

  //SGC BCF-10: general-purpose
  std::vector<G4String> PolystyreneElements = {"C", "H"};
  std::vector<G4int> PolystyreneAtoms = {8, 8};
  G4double PolystyreneDensity = 1.05 * g/cm3;
  std::vector<G4double> BCF10_energy = {2.32 * eV, 2.45 * eV, 2.59 * eV, 2.65 * eV,
					2.70 * eV, 2.80 * eV, 2.87 * eV, 2.91 * eV,
					2.96 * eV, 3.01 * eV, 3.07 * eV};
  std::vector<G4double> BCF10_yield = { 0.00, 0.06, 0.19, 0.36,
					0.52, 0.71, 1.00, 0.88,
                                        0.69, 0.17, 0.00};

  BCF10 = sNistMan->ConstructNewMaterial("BCF10", PolystyreneElements, PolystyreneAtoms, PolystyreneDensity);
  G4MaterialPropertiesTable* MPT_BCF10 = new G4MaterialPropertiesTable();
  FillConstProperty(MPT_BCF10, "RINDEX", 1.6);
  FillConstProperty(MPT_BCF10, "ABSLENGTH", 2.2*m);
  FillPropertyFromCurve(MPT_BCF10, "FASTCOMPONENT", BCF10_energy, BCF10_yield, 25);
  BCF10->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);
  MPT_BCF10->AddConstProperty("SCINTILLATIONYIELD", 8000/MeV);
  MPT_BCF10->AddConstProperty("RESOLUTIONSCALE", 1.0); // I think this isn't a property of the material, just of the simulation
  MPT_BCF10->AddConstProperty("FASTTIMECONSTANT", 2.7 * ns);
  BCF10->SetMaterialPropertiesTable(MPT_BCF10);

  //SGC BC-505: high-performance liquid scintillator
  std::vector<G4String> BC505Elements = {"C", "H"};
  std::vector<G4double> BC505Atoms = {1, 1.331};
  G4double BC505Density = 0.877 * g/cm3;
  std::vector<G4double> BC505_energy = {2.36*eV, 2.48*eV, 2.56*eV, 2.70*eV, 2.76*eV,
					2.81*eV, 2.87*eV, 2.91*eV, 2.95*eV, 2.97*eV,
					3.05*eV, 3.14*eV, 3.28*eV};
  std::vector<G4double> BC505_yield = { 0.00, 0.15, 0.27, 0.50, 0.67,
					0.78, 0.82, 0.95, 0.98, 0.96,
					0.80, 0.53, 0};

  BCF10 = sNistMan->ConstructNewMaterial("BC505", BC505Elements, BC505Atoms, BC505Density);
  G4MaterialPropertiesTable* MPT_BC505 = new G4MaterialPropertiesTable();
  FillConstProperty(MPT_BC505, "RINDEX", 1.505);
  FillConstProperty(MPT_BC505, "ABSLENGTH", 2.2*m); // this isn't specified in the datasheet, so I picked a number that's big enough not to matter much
  FillPropertyFromCurve(MPT_BC505, "FASTCOMPONENT", BC505_energy, BC505_yield, 25);
  BC505->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);
  MPT_BC505->AddConstProperty("SCINTILLATIONYIELD", 13920/MeV); // given as 80% of anthracene, which is 17400?
  MPT_BC505->AddConstProperty("RESOLUTIONSCALE", 1.0); // I think this isn't a property of the material, just of the simulation
  MPT_BC505->AddConstProperty("FASTTIMECONSTANT", 2.5 * ns);
  BC505->SetMaterialPropertiesTable(MPT_BC505);

}

// take a curve (defined by line segments between (x,y) points) and convert it to a histogram (interpolating between points)
void FlexRayMaterials::FillPropertyFromCurve(G4MaterialPropertiesTable *table, G4String name, std::vector<G4double> curveX, std::vector<G4double> curveY, G4int bins){
  std::vector<G4double> histX, histY;
  G4int points = curveX.size();

  // evenly space the histogram x values
  for(int i=0; i<bins; i++){
    histX.push_back( curveX[0] + ((i+0.0)/bins)*(curveX[points-1] - curveX[0]) );
  }

  int curvePoint = 0; // start at the first segment of the curve, step through other points as needed

  for(int i=0; i<bins; i++){
    // set y value of hist to value of curve at midpoint
    double interX = curveX[0] + ((i+0.5)/bins)*(curveX[points-1] - curveX[0]); // interpolation point to get value of curve
    while (curveX[curvePoint+1] < interX) curvePoint++; //advance to the correct curve segment
    double p = (interX-curveX[curvePoint]) / (curveX[curvePoint+1]-curveX[curvePoint]);
    histY.push_back( p*curveY[curvePoint+1] + (1-p)*curveY[curvePoint]);
  }

  histX.push_back(curveX[points-1]);
  histY.push_back(0.0);

  table->AddProperty(name, histX, histY);
}

// some properties only need a constant value, but they need to be specified as an array for GEANT to understand them.
void FlexRayMaterials::FillConstProperty(G4MaterialPropertiesTable *table, G4String name, G4double value){
  G4double energy[2] = {1.5 * eV ,3.1 * eV}; // full visible spectrum
  G4double values[2] = {value, value};

  table->AddProperty(name, energy, values, 2);
}

/*
NewScintillatorMaterial(G4String name, 

scintillator:
	emission spectrum
	resolution scale = 1
all:
	path length
	index of refraction
	elements
	density
*/
