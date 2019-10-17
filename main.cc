//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// --------------------------------------------------------------
//                 GEANT 4 - therapy example
// --------------------------------------------------------------
//
// Code developed currently by:
//  S.Guatelli & D. Cutajar

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "G4UImanager.hh"
#include "HadrontherapyPhysicsList.hh"

#include "G4ScoringManager.hh"
#include "ScoreWriter.hh"

#include <thread>

int main(int argc, char **argv) {

#ifdef G4MULTITHREADED
    G4MTRunManager *pRunManager = new G4MTRunManager;
    auto nThreads = std::thread::hardware_concurrency();
    pRunManager->SetNumberOfThreads(static_cast<G4int>(nThreads));
#else
    G4RunManager *pRunManager = new G4RunManager;
#endif

    G4cout << "***********************" << G4endl;
    G4cout << "*** " << G4Random::getTheSeed() << " ***" << G4endl;
    G4cout << "***********************" << G4endl;
    // Access to the Scoring Manager pointer

    G4ScoringManager *scoringManager = G4ScoringManager::GetScoringManager();

    // Overwrite the default output file with user-defined one
    scoringManager->SetScoreWriter(new ScoreWriter());

    // Initialize the physics component
    pRunManager->SetUserInitialization(new HadrontherapyPhysicsList);

    // 32Initialize the detector component
    DetectorConstruction *pDetectorConstruction =
        new DetectorConstruction(argv[1]);
    pRunManager->SetUserInitialization(pDetectorConstruction);

    // User action initialization
    ActionInitialization *actions = new ActionInitialization();
    pRunManager->SetUserInitialization(actions);

    // Initialize G4 kernel
    pRunManager->Initialize();

//// Initialize the Visualization component
#ifdef G4VIS_USE
    // Visualization manager
    G4VisManager *visManager = new G4VisExecutive;
    visManager->Initialize();
#endif

    // get the pointer to the User Interface manager
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    // implement batch mode selection using gdml files
    if (argc == 2) // Define UI session for interactive mode.
    {
#ifdef G4UI_USE
        G4UIExecutive *ui = new G4UIExecutive(argc, argv);
        G4cout << " UI session starts ..." << G4endl;
        G4String fileName = "init.mac";
        UImanager->ApplyCommand("/control/execute " + fileName);
        ui->SessionStart();
        delete ui;
#endif
    } else if (argc == 3) // Batch mode
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[2];
        UImanager->ApplyCommand(command + fileName);
    }

// Job termination
#ifdef G4VIS_USE
    delete visManager;
#endif

    delete pRunManager;

    return 0;
}
