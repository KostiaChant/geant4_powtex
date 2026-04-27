// version 1.00 Irina Stefanescu, ESS  
// 26-05-2022
// ************************************
//
#include <fstream>
#include <math.h>
#include <cstdio>
#include <cerrno>
#include "G4ios.hh"


#include "G4RunManager.hh"
#include "PowtexConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"
#include "QGSP_BERT_HP.hh"

// geant4 includes
//
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

// visualization
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4VGraphicsSystem.hh"

namespace {
void PrintUsage(const char* executableName) {
  G4cout << "Usage:\n"
         << "  " << executableName << " [macroFile] [inputDataFile] [-i] [--gui|--cli]\n"
         << "  " << executableName << " [macroFile] [--input-file inputDataFile] [-i] [--gui|--cli]\n"
         << "Options:\n"
         << "  --gui, --qt  Force GUI interactive session (Qt).\n"
         << "  --cli        Force terminal interactive session.\n"
         << G4endl;
}

G4bool FileExists(const G4String& filePath) {
  std::ifstream file(filePath);
  return file.good();
}

void RemoveOutputFileIfPresent(const char* filePath) {
  if (std::remove(filePath) == 0) {
    G4cout << "Deleted startup output file: " << filePath << G4endl;
    return;
  }

  if (errno == ENOENT) {
    G4cout << "Startup output file not found: " << filePath << G4endl;
    return;
  }

  G4cerr << "WARNING: Failed to delete startup output file: "
         << filePath << " (errno=" << errno << ")" << G4endl;
}

G4String SelectVisualizationMacro(const G4bool preferQtMacro) {
  const G4String candidates[] = {
      "macros/vis_qt_single.mac",
      "vis_qt_single.mac",
      "macros/vis.mac",
      "vis.mac"};

  if (preferQtMacro) {
    for (const auto& candidate : candidates) {
      if (FileExists(candidate)) {
        return candidate;
      }
    }
    return "";
  }

  if (FileExists("macros/vis.mac")) {
    return "macros/vis.mac";
  }
  if (FileExists("vis.mac")) {
    return "vis.mac";
  }
  return "";
}

G4bool HasUsableGraphicsDriver(G4VisManager* visManager) {
  if (visManager == nullptr) {
    return false;
  }

  const auto& graphicsSystems = visManager->GetAvailableGraphicsSystems();
  if (graphicsSystems.empty()) {
    return false;
  }

  for (const auto* graphicsSystem : graphicsSystems) {
    if (graphicsSystem != nullptr && graphicsSystem->GetNickname() != "TSG") {
      return true;
    }
  }

  return false;
}
}

//------------------------------------------------------------------------------
int main(int argc,char** argv) {

//  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
//  CLHEP::Random::setTheEngine(new CLHEP::RanecuEngine);

  G4String macroFile;
  G4String inputFileFromCommandLine;
  G4bool interactive = false;
  G4bool forceQtSession = false;
  G4bool forceCliSession = false;
  G4bool positionalOnly = false;

  for (G4int i = 1; i < argc; ++i) {
    const G4String arg = argv[i];

    if (!positionalOnly && arg == "--") {
      positionalOnly = true;
      continue;
    }

    if (!positionalOnly) {
      if (arg == "-h" || arg == "--help") {
        PrintUsage(argv[0]);
        return EXIT_SUCCESS;
      }

      if (arg == "-i") {
        interactive = true;
        continue;
      }

      if (arg == "--qt" || arg == "--gui") {
        if (forceCliSession) {
          G4cerr << "ERROR: --gui/--qt and --cli cannot be used together."
                 << G4endl;
          PrintUsage(argv[0]);
          return EXIT_FAILURE;
        }

        forceQtSession = true;
        interactive = true;
        continue;
      }

      if (arg == "--cli") {
        if (forceQtSession) {
          G4cerr << "ERROR: --cli and --gui/--qt cannot be used together."
                 << G4endl;
          PrintUsage(argv[0]);
          return EXIT_FAILURE;
        }

        forceCliSession = true;
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

      if (!arg.empty() && arg[0] == '-') {
        G4cerr << "ERROR: Unknown option: " << arg << G4endl;
        PrintUsage(argv[0]);
        return EXIT_FAILURE;
      }
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
  RemoveOutputFileIfPresent("myfile_info.txt");


//  remove("LookupTablePowtex.txt");
  RemoveOutputFileIfPresent("LookupTablePowtex.txt");

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
    G4UIExecutive* session = nullptr;
    if (forceQtSession) {
      session = new G4UIExecutive(argc, argv, "Qt");
    } else if (forceCliSession) {
      session = new G4UIExecutive(argc, argv, "tcsh");
    } else {
      session = new G4UIExecutive(argc, argv);
    }

    if (forceQtSession && !session->IsGUI()) {
      G4cerr << "WARNING: GUI session requested, but no GUI backend is active."
             << G4endl;
    }

    if (macroFile.empty()) {
      if (session->IsGUI()) {
        const G4bool hasGraphicsDriver = HasUsableGraphicsDriver(theVisManager);
        const G4String visMacro = SelectVisualizationMacro(true);

        if (!hasGraphicsDriver) {
          G4cerr << "WARNING: No usable graphics driver found (TSG fallback only)."
                 << G4endl;
          G4cerr << "         Skipping visualization macro execution."
                 << G4endl;
        }

        if (hasGraphicsDriver && !visMacro.empty()) {
          const G4String command = "/control/execute ";
          const G4int visStatus = UI->ApplyCommand(command + visMacro);
          if (visStatus != 0) {
            G4cerr << "WARNING: Failed to execute visualization macro file: "
                   << visMacro << G4endl;
          }
        } else if (hasGraphicsDriver) {
          G4cerr << "WARNING: No vis macro found in macros/ or current directory."
                 << G4endl;
        }
      } else {
        G4cout << "Terminal interactive session selected."
               << " Skipping visualization macro auto-load."
               << G4endl;
      }
    }

    G4cout << "\nType 'exit' to end the program.\n";
    session->SessionStart();
    delete session;
  }

  delete theVisManager;
  delete theRunManager;

  return EXIT_SUCCESS;
}
