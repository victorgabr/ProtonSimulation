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
//    *    Run.cc                  *
//    *                             *
//    *******************************

#include "Run.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPrimitiveScorer.hh"
#include <assert.h>

Run::Run(const G4String &detectorName) : G4Run() {
    // Get the sensitive detector manager
    G4SDManager *manager = G4SDManager::GetSDMpointer();

    // Get the sensitive detector
    G4MultiFunctionalDetector *detector =
        dynamic_cast<G4MultiFunctionalDetector *>(
            manager->FindSensitiveDetector(detectorName));

    // Expect the detector to exist
    assert(0 != detector);

    // Loop over primitive scorers registered with the detector
    for (G4int i = 0; i < detector->GetNumberOfPrimitives(); i++) {
        // Get scorer
        G4VPrimitiveScorer *scorer = detector->GetPrimitive(i);

        // Need to form the full collection name = detector name + "/"+ scorer name
        // to get the collection id number
        G4String collectionName = scorer->GetName();
        G4String fullCollectionName = detectorName + "/" + scorer->GetName();
        G4int id = manager->GetCollectionID(fullCollectionName);

        if (id >= 0) {
            G4cout << "++ " << fullCollectionName << " id " << id << G4endl;
            // Store obtained HitsCollection information into data members.
            // And, creates new G4THitsMap for accumulating quantities during RUN.
            fCollName.push_back(fullCollectionName);
            fCollID.push_back(id);
            fMap.push_back(new G4THitsMap<G4double>(detectorName, collectionName));
        } else {
            G4cout << "** collection " << fullCollectionName << " not found. "
                   << G4endl;
        }
    }
}

Run::~Run() {
    // Cleanup
    size_t nMap = fMap.size();
    for (size_t i = 0; i < nMap; i++) {
        if (fMap[i])
            fMap[i]->clear();
    }
    fCollName.clear();
    fCollID.clear();
    fMap.clear();
}

void Run::RecordEvent(const G4Event *aEvent) {
    numberOfEvent++; // This is an original line.

    //=============================
    // HitsCollection of This Event
    //============================
    G4HCofThisEvent *pHCE = aEvent->GetHCofThisEvent();
    if (!pHCE)
        return;

    //=======================================================
    // Sum up HitsMap of this Event  into HitsMap of this RUN
    //=======================================================
    size_t nCol = fCollID.size();
    for (size_t i = 0; i < nCol; i++) { // Loop over HitsCollection
        G4THitsMap<G4double> *evtMap = 0;
        if (fCollID[i] >= 0) { // Collection is attached to pHCE
            // C++11 casting
            evtMap = dynamic_cast<G4THitsMap<G4double> *>(pHCE->GetHC(fCollID[i]));
        } else {
            G4cout << " Error evtMap Not Found " << i << G4endl;
        }
        if (evtMap) {
            //=== Sum up HitsMap of this event to HitsMap of RUN.===
            *fMap[i] += *evtMap;
            //======================================================
        }
    }
}

void Run::Merge(const G4Run *aRun) {

    // casting local run at compile time
    const Run *localRun = static_cast<const Run *>(aRun);

    //=======================================================
    // Merge HitsMap of working threads
    //=======================================================
    size_t nCol = localRun->fCollID.size();
    for (size_t i = 0; i < nCol; i++) { // Loop over HitsCollection
        if (localRun->fCollID[i] >= 0) {
            *fMap[i] += *localRun->fMap[i];
        }
    }

    G4Run::Merge(aRun);
}

size_t Run::GetNumberOfHitsMap() const {
    return fMap.size();
}

G4THitsMap<G4double> *Run::GetHitsMap(size_t i) {
    return fMap[i];
}
//  Access method for HitsMap of the RUN
// Access HitsMap.
//  By  MultiFunctionalDetector name and Collection Name.
G4THitsMap<G4double> *Run::GetHitsMap(const G4String &detName,
                                      const G4String &colName) {
    G4String fullName = detName + "/" + colName;
    return GetHitsMap(fullName);
}

// Access HitsMap.
//  By full description of collection name, that is
//    <MultiFunctional Detector Name>/<Primitive Scorer Name>
G4THitsMap<G4double> *Run::GetHitsMap(const G4String &fullName) {
    size_t nCol = fCollName.size();
    for (size_t i = 0; i < nCol; i++) {
        if (fCollName[i] == fullName) {
            return fMap[i];
        }
    }
    return NULL;
}

// - Dump All HitsMap of this RUN. (for debuging and monitoring of quantity).
//   This method calls G4THisMap::PrintAll() for individual HitsMap.
void Run::DumpAllScorer() {

    // - Number of HitsMap in this RUN.
    size_t n = GetNumberOfHitsMap();
    // - GetHitsMap and dump values.
    for (size_t i = 0; i < n; i++) {
        G4THitsMap<G4double> *runMap = GetHitsMap(i);
        if (runMap) {
            G4cout << " PrimitiveScorer RUN " << runMap->GetSDname() << ","
                   << runMap->GetName() << G4endl;
            G4cout << " Number of entries " << runMap->entries() << G4endl;

            // C++11 auto type iterator
            auto itr = runMap->GetMap()->begin();
            for (; itr != runMap->GetMap()->end(); itr++) {
                G4cout << "  copy no, " << itr->first << "  ,Run Value, "
                       << *(itr->second) << G4endl;
            }
        }
    }
}
