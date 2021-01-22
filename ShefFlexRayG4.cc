#include "FlexRayDetectorConstruction.hh"
#include "FlexRayRunAction.hh"
#include "FlexRayEventAction.hh"
#include "FlexRayPhysicsList.hh"
#include "FlexRayPrimaryGeneratorAction.hh"
#include "FlexRaySteppingAction.hh"
#include "FlexRayTrackingAction.hh"

#include "G4OpticalPhysics.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int
main(int argc, char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Run manager
  //
  G4RunManager* runManager = new G4RunManager;

  // User Initialization classes (mandatory)
  //
  FlexRayDetectorConstruction* detector = new FlexRayDetectorConstruction;
  runManager->SetUserInitialization(detector);
  //
  // Using one of the pre-packaged physics lists
  // Shielding *physics = new Shielding();
  // physics->RegisterPhysics(new G4OpticalPhysics());

  // Or using our own personalised physics list
  G4VUserPhysicsList* physics = new FlexRayPhysicsList;
  runManager->SetUserInitialization(physics);
   
  // User Action classes
  //
  G4VUserPrimaryGeneratorAction* genAction = new FlexRayPrimaryGeneratorAction(detector);
  runManager->SetUserAction(genAction);
  //
  FlexRayRunAction* runAction = new FlexRayRunAction();
  runManager->SetUserAction(runAction);
  //
  FlexRayEventAction* eventAction = new FlexRayEventAction(runAction);
  runManager->SetUserAction(eventAction);
  //
  G4UserSteppingAction* steppingAction = new FlexRaySteppingAction(eventAction);
  runManager->SetUserAction(steppingAction);
  //
  G4UserTrackingAction* trackingAction = new FlexRayTrackingAction(eventAction);
  runManager->SetUserAction(trackingAction);

  // Initialize G4 kernel
  //
  runManager->Initialize();
  ((FlexRayTrackingAction*)trackingAction)->Initialize(detector);

  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
     
  // Get the pointer to the User Interface manager
  //
  G4UImanager * UImanager = G4UImanager::GetUIpointer();  

// Process macro or start UI session
  //
  if ( ! ui ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute gui.mac");
    }
    ui->SessionStart();
    delete ui;
  }

  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not
  //be deleted in the main() program !

  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
