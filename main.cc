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
//                 GEANT 4 - ProtonSimulation
// --------------------------------------------------------------
//
// Code developed by:  Victor Gabriel Leandro Alves
// Copyright 2017
//    *******************************
//    *                             *
//    *    main.cc                *
//    *                             *
//    *******************************

#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "HadrontherapyPhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "Randomize.hh"
#include "RunAction.hh"
#include "globals.hh"
#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

int main(int argc, char **argv) {
    // Construct the default run manager
    //

    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
    // Set the random number generator manually

    G4long myseed = 1;
    CLHEP::HepRandom::setTheSeed(myseed);

    // Set mandatory initialization and user action classes
    G4String fileName = argv[1];
    G4String outFilename = argv[2];
    G4String macroFilename = argv[3];
    DetectorConstruction *detector = new DetectorConstruction(fileName);

    // Start Run manager
    G4RunManager *runManager = new G4RunManager;
    runManager->SetUserInitialization(detector);
    runManager->SetUserInitialization(new HadrontherapyPhysicsList);
    PrimaryGeneratorAction *particleGun = new PrimaryGeneratorAction;
    runManager->SetUserAction(particleGun);
    // using output filename
    RunAction *runAction = new RunAction(outFilename);
    runManager->SetUserAction(runAction);
    runManager->Initialize();

#ifdef G4VIS_USE
    // Initialize visualization
    G4VisManager *visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();
#endif

    // Get the pointer to the User Interface manager
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

// interactive mode : define UI session
#ifdef G4UI_USE
    G4UIExecutive *ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    //    UImanager->ApplyCommand("/control/execute proton_source.mac");
    //    G4double en = particleGun->getParticleEnergy();
    UImanager->ApplyCommand("/control/execute " + macroFilename);
#else
    UImanager->ApplyCommand("/control/execute " + macroFilename);
#endif
    ui->SessionStart();
    delete ui;
#endif

// Job termination
// Free the store: user actions, physics_list and detector_description are
// owned and deleted by the run manager, so they should not be deleted
// in the main() program !

#ifdef G4VIS_USE
    delete visManager;
#endif
    delete runManager;

    return 0;
}
