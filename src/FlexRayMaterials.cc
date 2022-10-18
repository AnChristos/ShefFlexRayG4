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

  // Acrylic
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

  MY130 = sNistMan->ConstructNewMaterial("MY-130", PMMAelements, PMMAnatoms, PMMAdensity);
  G4MaterialPropertiesTable* MPT_MY130 = new G4MaterialPropertiesTable();
  FillConstProperty(MPT_MY130, "RINDEX", 1.3);
  FillConstProperty(MPT_MY130, "ABSLENGTH", 5.40 * m);
  MY130->SetMaterialPropertiesTable(MPT_MY130);

  // Glass
  std::vector<G4String> SiGlassElements = { "Si", "O" };
  std::vector<G4int> SiGlassAtoms = { 1, 2 };
  G4double SiGlassDensity = 2.203 * g / cm3;
  SiGlass = sNistMan->ConstructNewMaterial("SiGlass", SiGlassElements, SiGlassAtoms, SiGlassDensity);
  G4MaterialPropertiesTable* MPT_SiGlass = new G4MaterialPropertiesTable();
  FillConstProperty(MPT_SiGlass, "RINDEX", 1.467);
  FillConstProperty(MPT_SiGlass, "ABSLENGTH", 10 * m); // this is probably an underestimate, but it's long enough not to matter.
  SiGlass->SetMaterialPropertiesTable(MPT_SiGlass);

  std::vector<G4String> CoreGlassElements = { "Si", "O" };
  std::vector<G4int> CoreGlassAtoms = { 1, 2 };
  G4double CoreGlassDensity = 2.203 * g / cm3;
  CoreGlass = sNistMan->ConstructNewMaterial("CoreGlass", CoreGlassElements, CoreGlassAtoms, CoreGlassDensity);
  G4MaterialPropertiesTable* MPT_CoreGlass = new G4MaterialPropertiesTable();
  FillConstProperty(MPT_CoreGlass, "RINDEX", 1.467);
  FillConstProperty(MPT_CoreGlass, "ABSLENGTH", 10 * m); // this is probably an underestimate, but it's long enough not to matter.
  CoreGlass->SetMaterialPropertiesTable(MPT_CoreGlass);

  // Air with refractive index (for bubbles)

  std::vector<G4String> AirElements = { "C", "N", "O", "Ar"};
  std::vector<G4double> AirAtoms = { 0.02, 78.44, 21.07, 0.47 };
  G4double AirDensity = 1.205 * mg / cm3;
  Air = sNistMan->ConstructNewMaterial("AirBubble", AirElements, AirAtoms, AirDensity);
  G4MaterialPropertiesTable* MPT_Air = new G4MaterialPropertiesTable();
  FillConstProperty(MPT_Air, "RINDEX", 1.0003);
  FillConstProperty(MPT_Air, "ABSLENGTH", 1000 * m);
  Air->SetMaterialPropertiesTable(MPT_Air);


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

  // magic material for illustration purposes:
  BCF10_magic = sNistMan->ConstructNewMaterial("BCF10_magic", PolystyreneElements, PolystyreneAtoms, PolystyreneDensity);
  G4MaterialPropertiesTable* MPT_BCF10_magic = new G4MaterialPropertiesTable();
  FillConstProperty(MPT_BCF10_magic, "RINDEX", 1.6);
  FillConstProperty(MPT_BCF10_magic, "ABSLENGTH", 2.2*m);
  FillPropertyFromCurve(MPT_BCF10_magic, "FASTCOMPONENT", BCF10_energy, BCF10_yield, 25);
  BCF10_magic->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);
  MPT_BCF10_magic->AddConstProperty("SCINTILLATIONYIELD", 8000/MeV);
  MPT_BCF10_magic->AddConstProperty("RESOLUTIONSCALE", 1.0); // I think this isn't a property of the material, just of the simulation
  MPT_BCF10_magic->AddConstProperty("FASTTIMECONSTANT", 0.135 * ns);
  BCF10_magic->SetMaterialPropertiesTable(MPT_BCF10_magic);

  //SGC BCF-12: improved transmission for long fibers
  std::vector<G4double> BCF12_energy = {2.15*eV, 2.33*eV, 2.44*eV, 2.56*eV, 2.58*eV,
					2.62*eV, 2.69*eV, 2.84*eV, 2.90*eV, 2.95*eV,
					3.10*eV, 3.20*eV};
  std::vector<G4double> BCF12_yield = { 0.00, 0.08, 0.21, 0.35, 0.42,
					0.47, 0.71, 1.00, 0.94, 0.80,
					0.10, 0.00};

  BCF12 = sNistMan->ConstructNewMaterial("BCF12", PolystyreneElements, PolystyreneAtoms, PolystyreneDensity);
  G4MaterialPropertiesTable* MPT_BCF12 = new G4MaterialPropertiesTable();
  FillConstProperty(MPT_BCF12, "RINDEX", 1.6);
  FillConstProperty(MPT_BCF12, "ABSLENGTH", 2.7*m);
  FillPropertyFromCurve(MPT_BCF12, "FASTCOMPONENT", BCF12_energy, BCF12_yield, 25);
  BCF12->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);
  MPT_BCF12->AddConstProperty("SCINTILLATIONYIELD", 8000/MeV);
  MPT_BCF12->AddConstProperty("RESOLUTIONSCALE", 1.0); // I think this isn't a property of the material, just of the simulation
  MPT_BCF12->AddConstProperty("FASTTIMECONSTANT", 3.2 * ns);
  BCF12->SetMaterialPropertiesTable(MPT_BCF12);

  //SGC BCF-60: increased radiation hardness
  std::vector<G4double> BCF60_energy = {1.91*eV, 2.04*eV, 2.06*eV, 2.10*eV, 2.17*eV,
					2.23*eV, 2.26*eV, 2.33*eV, 2.37*eV, 2.42*eV,
					2.48*eV, 2.52*eV, 2.58*eV};
  std::vector<G4double> BCF60_yield = { 0.00, 0.11, 0.18, 0.20, 0.42,
					0.65, 0.82, 1.00, 0.88, 0.38,
					0.08, 0.03, 0.00};

  BCF60 = sNistMan->ConstructNewMaterial("BCF60", PolystyreneElements, PolystyreneAtoms, PolystyreneDensity);
  G4MaterialPropertiesTable* MPT_BCF60 = new G4MaterialPropertiesTable();
  FillConstProperty(MPT_BCF60, "RINDEX", 1.6);
  FillConstProperty(MPT_BCF60, "ABSLENGTH", 3.5*m);
  FillPropertyFromCurve(MPT_BCF60, "FASTCOMPONENT", BCF60_energy, BCF60_yield, 25);
  BCF60->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);
  MPT_BCF60->AddConstProperty("SCINTILLATIONYIELD", 7100/MeV);
  MPT_BCF60->AddConstProperty("RESOLUTIONSCALE", 1.0); // I think this isn't a property of the material, just of the simulation
  MPT_BCF60->AddConstProperty("FASTTIMECONSTANT", 7 * ns);
  BCF60->SetMaterialPropertiesTable(MPT_BCF60);

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

  BC505 = sNistMan->ConstructNewMaterial("BC505", BC505Elements, BC505Atoms, BC505Density);
  G4MaterialPropertiesTable* MPT_BC505 = new G4MaterialPropertiesTable();
  FillConstProperty(MPT_BC505, "RINDEX", 1.505);
  FillConstProperty(MPT_BC505, "ABSLENGTH", 2.2*m); // this isn't specified in the datasheet, so I picked a number that's big enough not to matter much
  FillPropertyFromCurve(MPT_BC505, "FASTCOMPONENT", BC505_energy, BC505_yield, 25);
  BC505->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);
  MPT_BC505->AddConstProperty("SCINTILLATIONYIELD", 13920/MeV); // given as 80% of anthracene, which is 17400?
  MPT_BC505->AddConstProperty("RESOLUTIONSCALE", 1.0); // I think this isn't a property of the material, just of the simulation
  MPT_BC505->AddConstProperty("FASTTIMECONSTANT", 2.5 * ns);
  BC505->SetMaterialPropertiesTable(MPT_BC505);

  // CeBr3: crystalline, high light yield
  std::vector<G4String> CeBrElements = {"Ce", "Br"};
  std::vector<G4int> CeBrAtoms = {1, 3};
  G4double CeBrDensity = 5.1 * g/cm3;
  std::vector<G4double> CeBr_energy = {2.36*eV, 2.48*eV, 2.56*eV, 2.70*eV, 2.76*eV,
					2.81*eV, 2.87*eV, 2.91*eV, 2.95*eV, 2.97*eV,
					3.05*eV, 3.14*eV, 3.28*eV};
  std::vector<G4double> CeBr_yield = { 0.00, 0.15, 0.27, 0.50, 0.67,
					0.78, 0.82, 0.95, 0.98, 0.96,
					0.80, 0.53, 0};

  CeBr3 = sNistMan->ConstructNewMaterial("CeBr3", CeBrElements, CeBrAtoms, CeBrDensity);
  G4MaterialPropertiesTable* MPT_CeBr = new G4MaterialPropertiesTable();
  FillConstProperty(MPT_CeBr, "RINDEX", 2.09);
  FillConstProperty(MPT_CeBr, "ABSLENGTH", 2.2*m); // unknown
  FillPropertyFromCurve(MPT_CeBr, "FASTCOMPONENT", BC505_energy, BC505_yield, 25);
  CeBr3->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);
  MPT_CeBr->AddConstProperty("SCINTILLATIONYIELD", 60000/MeV);
  MPT_CeBr->AddConstProperty("RESOLUTIONSCALE", 1.0); // I think this isn't a property of the material, just of the simulation
  MPT_CeBr->AddConstProperty("FASTTIMECONSTANT", 19 * ns);
  CeBr3->SetMaterialPropertiesTable(MPT_CeBr);

  // Semiconductor materials
  std::vector<G4String> CdTeElements = {"Cd", "Te"};
  std::vector<G4int> CdTeAtoms = {1, 1};
  G4double CdTeDensity = 5.85 * g/cm3;
  CdTe = sNistMan->ConstructNewMaterial("CdTe", CdTeElements, CdTeAtoms, CdTeDensity);
  G4MaterialPropertiesTable* MPT_CdTe = new G4MaterialPropertiesTable();
  FillConstProperty(MPT_CdTe, "RINDEX", 2.09);
  FillConstProperty(MPT_CdTe, "ABSLENGTH", 1*um);
  CdTe->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);
  MPT_CdTe->AddConstProperty("RESOLUTIONSCALE", 1.0); // I think this isn't a property of the material, just of the simulation
  MPT_CdTe->AddConstProperty("SCINTILLATIONYIELD", 0/MeV);
  MPT_CdTe->AddConstProperty("FASTTIMECONSTANT", 1 * ns);
  MPT_CdTe->AddConstProperty("EHOLEPRODUCTION", 4.43*eV); // https://www.osti.gov/etdeweb/servlets/purl/20267247
  CdTe->SetMaterialPropertiesTable(MPT_CdTe);

  Core = BCF10;
  Clad1 = PMMA149;
  Clad2 = PMMA142;

  EndFiberCore = CoreGlass;
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
