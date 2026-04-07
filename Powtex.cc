// version 1.00 Irina Stefanescu, ESS  
// 26-05-2022
// ************************************
//
#include <fstream>
#include <math.h>
#include "G4ios.hh"


#include "G4RunManager.hh"
#include "PowtexConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"
#include "QGSP_BERT_HP.hh"

// geant4 includes
//
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "Randomize.hh"

// visualization
#include "G4VisExecutive.hh"

//------------------------------------------------------------------------------
int main(int argc,char** argv) {

//  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
//  CLHEP::Random::setTheEngine(new CLHEP::RanecuEngine);

  G4String filename;
  bool interactive=false;
  const G4String interactiveFlag = "-i";

//....Last option is filename (unless it is -i)
  if(argc>1 && G4String(argv[argc-1]) != interactiveFlag) 
    filename = G4String(argv[argc-1]); 

//....Make session interactive if "-i"
  if(argc==1 || G4String(argv[1]) == interactiveFlag) interactive = true;

//  remove("myfile_info.txt");  
  if (remove("myfile_info.txt") !=0 )
    puts ("File not found, continue....");
  else 
    puts("Output file myfile_info.txt succesfully deleted at the begining of the run, continue....");


//  remove("LookupTablePowtex.txt");
  if (remove("LookupTablePowtex.txt") !=0 )
    puts ("File not found, continue....");
  else 
    puts("LookupTablePowtex.txt succesfully deleted at the begining of the run, continue....");

  // Run manager
  //------------
  G4RunManager * theRunManager = new G4RunManager;
  

  // UserInitialization classes
  //---------------------------
  PowtexConstruction* theDetector = new PowtexConstruction;
  theRunManager->SetUserInitialization(theDetector);

  G4VUserPhysicsList* physics = new QGSP_BERT_HP;
  physics->SetVerboseLevel(1);
  theRunManager->SetUserInitialization(physics);

  // UserAction classes
  //-------------------
  SteppingAction* step_action = new SteppingAction();
  PrimaryGeneratorAction* prim = new PrimaryGeneratorAction();

  theRunManager->SetUserAction(prim);

  theRunManager->SetUserAction(step_action);
  
  G4UIsession* session=0;

  if (argc==1)   // Define UI session for interactive mode.
    {
#ifdef G4UI_USE_XM
      session = new G4UIXm(argc,argv);
#else
#ifdef G4UI_USE_TCSH
      session = new G4UIterminal(new G4UItcsh);
#else
      session = new G4UIterminal();
#endif
#endif
    }

  // Visualization, if you choose to have it!
  //-----------------------------------------
  G4VisManager* theVisManager = new G4VisExecutive;
  theVisManager->Initialize();

   
  // Initialize G4 kernel 
  // comment this line if you use the exdrm physics list

 theRunManager->Initialize();
  

  // get the pointer to the User Interface manager
  // want to be in the PreInit state of the kernel, irina
  
  G4UImanager* UI = G4UImanager::GetUIpointer();
  if (session)   // Define UI session for interactive mode.
    {
      session->SessionStart();
      delete session;
    }
  else           // Batch mode
    {
      const G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UI->ApplyCommand(command+fileName);
    }

  // User interactions
  //------------------
//  if(filename) 
//    G4UImanager::GetUIpointer()->ApplyCommand("/control/execute "+filename);

  //....Start an interactive session
  if(interactive){
    G4cout << "\nType 'exit' to end the program.\n";
    G4UIterminal(new G4UItcsh).SessionStart();
  }

  delete theVisManager;
  delete theRunManager;

  return EXIT_SUCCESS;
}
