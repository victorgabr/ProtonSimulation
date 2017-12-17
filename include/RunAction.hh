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
// Copyright 2008-2017
//    *******************************
//    *                             *
//    *    RunAction.hh              *
//    *                             *
//    *******************************
#ifndef RunAction_HH
#define RunAction_HH

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "Run.hh"
#include "RunAction.hh"
#include <assert.h>

#include "G4ConvergenceTester.hh"
#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class RunAction : public G4UserRunAction {
public:
    // Constructor
    RunAction();
    RunAction(G4int nScoringVolumes);
    RunAction(G4String const outputFilename);
    // Destructor
    virtual ~RunAction();

    // Methods
    virtual G4Run *GenerateRun();

    void BeginOfRunAction(const G4Run *);
    void EndOfRunAction(const G4Run *);
    void fillPerEvent(G4double);
    G4int nScoringVolumes;
    std::map<G4int, CLHEP::Hep3Vector> scorerPositions;

    // utilities
    // Utility method for converting segment number of
    // water phantom to copyNo of HitsMap.
    G4int CopyNo(G4int ix, G4int iy, G4int iz) {
        return (iy * (fNx * fNz) + ix * fNz + iz);
    }

    void saveResults(Run *aRun, const G4String fileName);


private:

    // Data member
    G4String _outFilename;

    // - vector of MultiFunctionalDetecor names.
    std::vector<G4String> fSDName;

    // for conversion of sengment number to copyNo.
    G4int fNx, fNy, fNz;
};

#endif
