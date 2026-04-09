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

namespace {
void PrintUsage(const char* executableName) {
  G4cout << "Usage:\n"
         << "  " << executableName << " [macroFile] [inputDataFile] [-i]\n"
         << "  " << executableName << " [macroFile] [--input-file inputDataFile] [-i]\n"
         << G4endl;
}
}

//------------------------------------------------------------------------------
int main(int argc,char** argv) {

//  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
//  CLHEP::Random::setTheEngine(new CLHEP::RanecuEngine);

  G4String macroFile;
  G4String inputFileFromCommandLine;
  G4bool interactive = false;

  for (G4int i = 1; i < argc; ++i) {
    const G4String arg = argv[i];

    if (arg == "-i") {
      interactive = true;
      continue;
    }

    if (arg == "--input-file" || arg == "-f") {
      if (i + 1 >= argc) {
        G4cerr << "ERROR: Missing value for " << arg << G4endl;
        PrintUsage(argv[0]);
        return EXIT_FAILURE;
      }

      inputFileFromCommandLine = argv[++i];
      continue;
    }

    if (macroFile.empty()) {
      macroFile = arg;
      continue;
    }

    if (inputFileFromCommandLine.empty()) {
      inputFileFromCommandLine = arg;
      continue;
    }

    G4cerr << "ERROR: Unrecognized extra argument: " << arg << G4endl;
    PrintUsage(argv[0]);
    return EXIT_FAILURE;
  }

  if (macroFile.empty()) {
    interactive = true;
  }

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
    PrimaryGeneratorAction* prim = new PrimaryGeneratorAction(inputFileFromCommandLine);

  theRunManager->SetUserAction(prim);

  theRunManager->SetUserAction(step_action);

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
  if (!macroFile.empty()) {
    const G4String command = "/control/execute ";
    const G4int status = UI->ApplyCommand(command + macroFile);
    if (status != 0) {
      G4cerr << "ERROR: Failed to execute macro file: " << macroFile << G4endl;
      delete theVisManager;
      delete theRunManager;
      return EXIT_FAILURE;
    }
  }

  // User interactions
  //------------------
  if (interactive) {
    G4UIsession* session = 0;

#ifdef G4UI_USE_XM
    session = new G4UIXm(argc,argv);
#else
#ifdef G4UI_USE_TCSH
    session = new G4UIterminal(new G4UItcsh);
#else
    session = new G4UIterminal();
#endif
#endif

    G4cout << "\nType 'exit' to end the program.\n";
    session->SessionStart();
    delete session;
  }

  delete theVisManager;
  delete theRunManager;

  return EXIT_SUCCESS;
}
