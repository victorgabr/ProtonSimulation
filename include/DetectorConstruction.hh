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
//    *    DetectorConstruction.hh                *
//    *                             *
//    *******************************

#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

#include "G4GDMLParser.hh"
#include "G4VUserDetectorConstruction.hh"

#include "G4Material.hh"

#include "globals.hh"

class DetectorMessenger;

// ----------------------------------------------------------------------------

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    // Constructor and destructor
    //
    DetectorConstruction();
    DetectorConstruction(const G4String& File);
    ~DetectorConstruction();

    // Volume Constructor Method
    G4VPhysicalVolume* Construct();

    // Writing and Reading GDML
    void SetNumberOfVolumes(G4int n);
    G4int GetNumberOfScoreVolumes();

    void readGDML(G4String fileName);
    void setScoring();

private:
    // GDMLparser
    G4GDMLParser parser;

    G4int nScoreVols;
    // Reading and Writing Settings
    G4String fReadFile;

    G4VPhysicalVolume* fWorldPhysVol;



public:
    // position mapping
    // Output map - energy binning on x axis, theta on y
    std::map<G4int, CLHEP::Hep3Vector> scorerPositions;
    std::map<G4int, CLHEP::Hep3Vector> GetScorerPositions();

    void DescriptionFcnPtr(G4VPhysicalVolume* aPV, int aDepth, int replicaNo,
                           const G4Transform3D& aTransform);

private:
    void TraverseReplicas(G4VPhysicalVolume* aPV, int aDepth,
                          const G4Transform3D& aTransform);

    void DescribeAndDescendGeometry(G4VPhysicalVolume* aPV, int aDepth,
                                    int replicaNo,
                                    const G4Transform3D& aTransform);
};

// ----------------------------------------------------------------------------

#endif
