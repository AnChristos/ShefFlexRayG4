#include "FlexRayRunAction.hh"
#include "FlexRayGeometry.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
FlexRayRunAction::FlexRayRunAction()
: G4UserRunAction(),
  fNumDetected(0),
  fAnalysisManager(G4Analysis::ManagerInstance("csv")) // also try "csv", "root", "xml", "hdf5"
{
  // record indiviual photons on SiPM
  fAnalysisManager->CreateNtuple("Photons", "Optical Photons Detected");
  fAnalysisManager->CreateNtupleIColumn(0, "event");
  fAnalysisManager->CreateNtupleIColumn(0, "detector");
  fAnalysisManager->CreateNtupleDColumn(0, "energy");
  fAnalysisManager->CreateNtupleDColumn(0, "time");
  fAnalysisManager->CreateNtupleDColumn(0, "x");
  fAnalysisManager->CreateNtupleDColumn(0, "y");
  fAnalysisManager->FinishNtuple(0);

  // compile photons into total hits on SiPM
  fAnalysisManager->CreateNtuple("Digi", "SiPM Hits");
  fAnalysisManager->CreateNtupleIColumn(1, "event");
  fAnalysisManager->CreateNtupleIColumn(1, "detector");
  fAnalysisManager->CreateNtupleIColumn(1, "photons");
  fAnalysisManager->CreateNtupleDColumn(1, "energy");
  fAnalysisManager->CreateNtupleDColumn(1, "time");
  fAnalysisManager->FinishNtuple(1);

  // detectors lit up from a single x-ray:
  // 0: none
  // 1, 2, 4, 8: one end of one fiber
  // 3: both ends of an x-measurement
  // 12: both ends of a y-measurement
  // 15: all four sides
  fAnalysisManager->CreateH1("Detectors", "Detectors Hit", 16, -0.5, 15.5);

  // energy deposition in a fiber core.  Is this the correct measurement to use for radiation hardness calculations?
  fAnalysisManager->CreateH1("Edep", "Energy Deposited [keV]", 120, 0, 120);

  // electron-ion pairs.  useful for semiconductors.  what does it tell me in scintillators?
  fAnalysisManager->CreateH1("EHoles", "Electron-Hole pairs produced", 100, 0, 500000);

  // electron-hole pairs created in a (semiconductor) fiber core.
  //fAnalysisManager->CreateH1("Edep", "Energy Deposited [keV]", 100, 0, 1000);

  // MC truth
  fAnalysisManager->CreateNtuple("Xrays", "X-Ray hits");
  fAnalysisManager->CreateNtupleIColumn(2, "event");
  fAnalysisManager->CreateNtupleDColumn(2, "energy");
  fAnalysisManager->CreateNtupleDColumn(2, "x");
  fAnalysisManager->CreateNtupleDColumn(2, "y");
  fAnalysisManager->FinishNtuple(2);

  fAnalysisManager->CreateNtuple("Params", "Detector Parameters");
  fAnalysisManager->CreateNtupleDColumn(3, "scint_yield");
  fAnalysisManager->CreateNtupleDColumn(3, "scint_time");
  fAnalysisManager->CreateNtupleDColumn(3, "scint_index");
  fAnalysisManager->CreateNtupleDColumn(3, "n_fibers");
  fAnalysisManager->CreateNtupleDColumn(3, "fiber_spacing");
  fAnalysisManager->CreateNtupleDColumn(3, "layer_offset_x");
  fAnalysisManager->CreateNtupleDColumn(3, "layer_offset_y");
  fAnalysisManager->CreateNtupleDColumn(3, "num_layers");
  fAnalysisManager->CreateNtupleDColumn(3, "detector_spacing");
  fAnalysisManager->FinishNtuple(3);

  fAnalysisManager->CreateNtuple("Flex", "FlexPoints");
  fAnalysisManager->CreateNtupleDColumn(4, "x_local");
  fAnalysisManager->CreateNtupleDColumn(4, "y_local");
  fAnalysisManager->CreateNtupleDColumn(4, "x_global");
  fAnalysisManager->CreateNtupleDColumn(4, "y_global");
  fAnalysisManager->CreateNtupleDColumn(4, "z_global");
  fAnalysisManager->CreateNtupleDColumn(4, "x_normal");
  fAnalysisManager->CreateNtupleDColumn(4, "y_normal");
  fAnalysisManager->CreateNtupleDColumn(4, "z_normal");
  fAnalysisManager->FinishNtuple(4);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FlexRayRunAction::~FlexRayRunAction()
{
  delete fAnalysisManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FlexRayRunAction::BeginOfRunAction(const G4Run*)
{
  fNumDetected = 0;
  fAnalysisManager->OpenFile("FlexRay");
  // This method is called at the beginning of each run.
  // We can use it to initialize variables, arrays, etc.

  G4MaterialPropertiesTable *core = G4LogicalVolumeStore::GetInstance()->GetVolume("Core")->GetMaterial()->GetMaterialPropertiesTable();

  if(core->ConstPropertyExists("SCINTILLATIONYIELD")) fAnalysisManager->FillNtupleDColumn(3, 0, core->GetConstProperty("SCINTILLATIONYIELD"));
  else fAnalysisManager->FillNtupleDColumn(3, 0, 0);

  if(core->ConstPropertyExists("FASTTIMECONSTANT")) fAnalysisManager->FillNtupleDColumn(3, 1, core->GetConstProperty("FASTTIMECONSTANT"));
  else fAnalysisManager->FillNtupleDColumn(3, 1, 0);

  if(core->GetProperty("RINDEX")) fAnalysisManager->FillNtupleDColumn(3, 2, core->GetProperty("RINDEX")->GetMinValue());
  else fAnalysisManager->FillNtupleDColumn(3, 2, 0);

  fAnalysisManager->FillNtupleDColumn(3, 3, geo::numFibers);
  fAnalysisManager->FillNtupleDColumn(3, 4, geo::fiberSpacing);

  // layer offsets
  fAnalysisManager->FillNtupleDColumn(3, 5, -geo::layerSpacing/2);
  fAnalysisManager->FillNtupleDColumn(3, 6, geo::layerSpacing/2);

  fAnalysisManager->FillNtupleDColumn(3, 7, geo::numLayers);
  fAnalysisManager->FillNtupleDColumn(3, 8, geo::detectorSpacing);

  fAnalysisManager->AddNtupleRow(3);

  // fill FlexPoints
  double size = geo::numFibers * geo::fiberSpacing;

  for(int i=0; i<5; i++){
    for(int j=0; j<5; j++){
      double xl = (i/4.0 - 0.5)*size;
      double yl = (j/4.0 - 0.5)*size;
      double theta = xl / geo::bendRadius;
      double xg = geo::bendRadius * sin(theta);
      double yg = yl;
      double zg = geo::bendRadius * cos(theta);
      double xn = -sin(theta);
      double yn = 0;
      double zn = -cos(theta);

      fAnalysisManager->FillNtupleDColumn(4,0,xl);
      fAnalysisManager->FillNtupleDColumn(4,1,yl);
      fAnalysisManager->FillNtupleDColumn(4,2,xg);
      fAnalysisManager->FillNtupleDColumn(4,3,yg);
      fAnalysisManager->FillNtupleDColumn(4,4,zg);
      fAnalysisManager->FillNtupleDColumn(4,5,xn);
      fAnalysisManager->FillNtupleDColumn(4,6,yn);
      fAnalysisManager->FillNtupleDColumn(4,7,zn);
      fAnalysisManager->AddNtupleRow(4);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FlexRayRunAction::EndOfRunAction(const G4Run* run)
{
  // This method is called at the end of each run.
  // We can use it to make calculations or simply to write collected data
  // to the output file.

  G4int nEvents = run->GetNumberOfEvent();

  G4cout << G4endl << "--------------------End of Run-----------------------" << G4endl;
  G4cout << nEvents << " Events, " << fNumDetected << " Detected" << G4endl;
  G4cout << "Efficiency: " << (G4double)fNumDetected / nEvents << G4endl;
  G4cout << G4endl << "------------------------------------------------------------" << G4endl;

  fAnalysisManager->Write();
  fAnalysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
