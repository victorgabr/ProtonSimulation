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
// * By using,  copying,  modifying or  distribubuild-RE02-GEANT4-Debugting the
// software (or *
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
//    *    Run.hh                *
//    *                             *
//    *******************************
#ifndef Run_HH
#define Run_HH

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4THitsMap.hh"
#include "G4THitsMap.hh"
#include <vector>

class G4Event;

class Run : public G4Run {
public:
    // Constructor
    Run(const G4String &detectorName);

    // Destructor
    virtual ~Run();

public:
    // Override this method in G4Run
    virtual void RecordEvent(const G4Event *);
    virtual void Merge(const G4Run*);

    // Dump all data
    void DumpData(G4int n) const;
    // Dump all using mappings
    void DumpData(std::map<G4int, G4double> scorerPosisions) const;

    // Access methods for scoring information.
    // - Number of HitsMap for this RUN.
    //   This is equal to number of collections.
    G4int GetNumberOfHitsMap() const {
        return fMap.size();
    }
    // - Get HitsMap of this RUN.
    //   by sequential number, by multifucntional name and collection name,
    //   and by collection name with full path.
    G4THitsMap<G4double> *GetHitsMap(G4int i) {
        return fMap[i];
    }
    G4THitsMap<G4double> *GetHitsMap(const G4String &detName,
                                     const G4String &colName);
    G4THitsMap<G4double> *GetHitsMap(const G4String &fullName);
    // - Dump All HitsMap of this RUN.
    //   This method calls G4THisMap::PrintAll() for individual HitsMap.
    void DumpAllScorer();

private:
    // Helper function
    void Print(const std::vector<G4String> &title,
               const std::map<G4int, std::vector<G4double>> &out) const;

    // Data member
    //    std::map<G4int, G4THitsMap<G4double> *> fMap;
    std::vector<G4THitsMap<G4double> *> fMap;
    std::vector<G4String> fCollName;
    std::vector<G4int> fCollID;
};

#endif
