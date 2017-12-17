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
//                 GEANT 4 - BrachySourceKerma
// --------------------------------------------------------------
//
// Code developed by:  Victor Gabriel Leandro Alves
// Copyright 2008-2017
//    *******************************
//    *                             *
//    *    RunAction.c              *
//    *                             *
//    *******************************
#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "Run.hh"
#include <assert.h>
#include <fstream>

RunAction::RunAction() : G4UserRunAction(), fNx(1), fNy(1), fNz(1) {}

RunAction::RunAction(G4int n) {
    this->fNx = 1;
    this->fNy = 1;
    fNz = n;
}

RunAction::RunAction(const G4String outputFilename) {
    _outFilename = outputFilename;
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run *aRun) {
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
}

G4Run *RunAction::GenerateRun() {
    G4cout << "Creating user define run class Run" << G4endl;
    return new Run("MyDetector");
}

void RunAction::EndOfRunAction(const G4Run *aRun) {
    //    theRun->DumpData(nScoringVolumes);
    if (!IsMaster())
        return;

    //- water phantom (Detector) Information.
    //-- Number of segments in the water phantom.
    const DetectorConstruction *detector =
        (const DetectorConstruction *)(G4RunManager::GetRunManager()
                                       ->GetUserDetectorConstruction());

    // Getting scorer positions from Detector construction
    scorerPositions = detector->scorerPositions;
    fNz = scorerPositions.size();

    //- RE02Run object.
    Run *theRun = (Run *)aRun;

    G4int NbOfEvents = aRun->GetNumberOfEvent();
    if (NbOfEvents == 0)
        return;
    // Print interesting data
    G4cout << "============================================================="
           << G4endl;
    G4cout << "Number of Events Processed:" << aRun->GetNumberOfEvent()
           << " events. " << G4endl;
    G4cout << "============================================================="
           << G4endl;

    // saving results
    saveResults(theRun, _outFilename);
}

void RunAction::saveResults(Run *theRun, const G4String fileName) {

    G4THitsMap<G4double> *totEdep = theRun->GetHitsMap("MyDetector/myScorer");

    // ITERATE on positions map

    std::map<G4int, CLHEP::Hep3Vector>::iterator iter = scorerPositions.begin();
    // rescaling z corrdinate to get 0 at phantom surface
    G4double zMax = -100000000000000.0;
    while (iter != scorerPositions.end()) {
        // z corrdinate from Hep3Vector
        if (iter->second[2] >= zMax) {
            zMax = iter->second[2];
        }
        iter++;
    }


    std::map<G4int, CLHEP::Hep3Vector>::iterator it = scorerPositions.begin();
    std::ofstream file(fileName);
    G4String header = "replicaNumber,x,y,z,Edep";
    G4cout << header << G4endl;
    file << header << G4endl;
    while (it != scorerPositions.end()) {
        // getting scored quantity from hitsmap
        G4double *totED =
            (*totEdep)[CopyNo(it->second[0], it->second[1], it->first)];

        if (!totED)
            totED = new G4double(0.0);

        //rescale to 0 cm at phantom surface
        G4double zPos = it->second[2] + zMax;
        G4cout << it->first << "," << it->second[0] << "," << it->second[1] << ","
               << zPos<< "," << *totED / MeV << G4endl;
        file << it->first << "," << it->second[0] << "," << it->second[1] << ","
             << zPos << "," << *totED / MeV << G4endl;

        it++;
    }
    // always close files
    file.close();
}
