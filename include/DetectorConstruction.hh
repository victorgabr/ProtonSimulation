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
//                 DetectorConstruction.hh
// --------------------------------------------------------------
//
// Code developed by:  Victor Gabriel Leandro Alves

#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

#include <G4GDMLParser.hh>
#include <G4VUserDetectorConstruction.hh>

class DetectorMessenger;

// ----------------------------------------------------------------------------

class DetectorConstruction : public G4VUserDetectorConstruction {
  public:
    // Constructor and destructor
    //
    DetectorConstruction();
    DetectorConstruction(const G4String File);
    ~DetectorConstruction();

    // Volume Constructor Method
    G4VPhysicalVolume *Construct();

    // Writing and Reading GDML
    G4int GetNumberOfScoreVolumes();

    void readGDML();
    void setScoring();

  private:
    // GDMLparser
    G4GDMLParser fParser;

    // Reading and Writing Settings
    G4String fReadFile;

    // Reading and Writing Settings
    const G4String fGDMLFile;
    G4VPhysicalVolume *fWorldPhysVol;
};

// ----------------------------------------------------------------------------

#endif
