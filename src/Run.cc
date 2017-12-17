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
//    *    Run.cc                  *
//    *                             *
//    *******************************
//    Saves an output.csv file

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
    G4int nMap = fMap.size();
    for (G4int i = 0; i < nMap; i++) {
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
    G4int nCol = fCollID.size();
    for (G4int i = 0; i < nCol; i++) { // Loop over HitsCollection
        G4THitsMap<G4double> *evtMap = 0;
        if (fCollID[i] >= 0) { // Collection is attached to pHCE
            evtMap = (G4THitsMap<G4double> *)(pHCE->GetHC(fCollID[i]));
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
    const Run *localRun = static_cast<const Run *>(aRun);

    //=======================================================
    // Merge HitsMap of working threads
    //=======================================================
    G4int nCol = localRun->fCollID.size();
    for (G4int i = 0; i < nCol; i++) { // Loop over HitsCollection
        if (localRun->fCollID[i] >= 0) {
            *fMap[i] += *localRun->fMap[i];
        }
    }

    G4Run::Merge(aRun);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//  Access method for HitsMap of the RUN
//
//-----
// Access HitsMap.
//  By  MultiFunctionalDetector name and Collection Name.
G4THitsMap<G4double> *Run::GetHitsMap(const G4String &detName,
                                      const G4String &colName) {
    G4String fullName = detName + "/" + colName;
    return GetHitsMap(fullName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//-----
// Access HitsMap.
//  By full description of collection name, that is
//    <MultiFunctional Detector Name>/<Primitive Scorer Name>
G4THitsMap<G4double> *Run::GetHitsMap(const G4String &fullName) {
    G4int nCol = fCollName.size();
    for (G4int i = 0; i < nCol; i++) {
        if (fCollName[i] == fullName) {
            return fMap[i];
        }
    }
    return NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//-----
// - Dump All HitsMap of this RUN. (for debuging and monitoring of quantity).
//   This method calls G4THisMap::PrintAll() for individual HitsMap.
void Run::DumpAllScorer() {

    // - Number of HitsMap in this RUN.
    G4int n = GetNumberOfHitsMap();
    // - GetHitsMap and dump values.
    for (G4int i = 0; i < n; i++) {
        G4THitsMap<G4double> *runMap = GetHitsMap(i);
        if (runMap) {
            G4cout << " PrimitiveScorer RUN " << runMap->GetSDname() << ","
                   << runMap->GetName() << G4endl;
            G4cout << " Number of entries " << runMap->entries() << G4endl;
            std::map<G4int, G4double *>::iterator itr = runMap->GetMap()->begin();
            for (; itr != runMap->GetMap()->end(); itr++) {
                G4cout << "  copy no, " << itr->first << "  ,Run Value, "
                       << *(itr->second) << G4endl;
            }
        }
    }
}

// void Run::DumpData(G4int nThetaBins) const {
//    // Titles
//    std::vector<G4String> title;
//    title.push_back("Theta");

//    // Output map - energy binning on x axis, theta on y
//    std::map<G4int, std::vector<G4double>> output;

//    // Standard 10 bins in theta
//    // TODO replace hardcoded number of bins by number of primitive scorers
//    // TODO implement ROOT scoring classes
//    // Energy bins depends on the number of scorers
//    G4int nEnergyBins = fMap.size();

//    G4int i(0), j(0);

//    // Initialise current to 0 in all bins
//    for (i = 0; i < nThetaBins; i++) {
//        for (j = 0; j < nEnergyBins; j++) {
//            output[i].push_back(0);
//        }
//    }

//    // Fill the output map
//    i = 0;
//    std::map<G4int, G4THitsMap<G4double> *>::const_iterator iter =
//    fMap.begin();

//    while (iter != fMap.end()) {
//        G4THitsMap<G4double> *hitMap = iter->second;

//        title.push_back(hitMap->GetName());

//        std::map<G4int, G4double *> *myMap = hitMap->GetMap();

//        for (j = 0; j < nThetaBins; j++) {
//            G4double *current = (*myMap)[j];
//            if (0 != current)
//                output[j][i] = (*current);
//        }

//        i++;
//        iter++;
//    }

//    Print(title, output);
//}

// void Run::DumpData(std::map<G4int, G4double> scorerPosisions) const {
//    // Titles
//    std::vector<G4String> title;
//    title.push_back("Theta");

//    // Output map - energy binning on x axis, theta on y
//    std::map<G4int, std::vector<G4double>> output;

//    // Standard 10 bins in theta
//    // TODO replace hardcoded number of bins by number of primitive scorers
//    // TODO implement ROOT scoring classes
//    // Energy bins depends on the number of scorers
//    G4int nEnergyBins = fMap.size();

//    G4int i(0), j(0);
//    G4int nThetaBins = 10;
//    // Initialise current to 0 in all bins
//    for (i = 0; i < nThetaBins; i++) {
//        for (j = 0; j < nEnergyBins; j++) {
//            output[i].push_back(0);
//        }
//    }

//    // Fill the output map
//    i = 0;
//    std::map<G4int, G4THitsMap<G4double> *>::const_iterator iter =
//    fMap.begin();

//    while (iter != fMap.end()) {
//        G4THitsMap<G4double> *hitMap = iter->second;

//        title.push_back(hitMap->GetName());

//        std::map<G4int, G4double *> *myMap = hitMap->GetMap();

//        for (j = 0; j < nThetaBins; j++) {
//            G4double *current = (*myMap)[j];
//            if (0 != current)
//                output[j][i] = (*current);
//        }

//        i++;
//        iter++;
//    }

//    Print(title, output);
//}

// void Run::Print(const std::vector<G4String> &title,
//                const std::map<G4int, std::vector<G4double>> &myMap) const {
//    // Print to G4cout and an output file
//    std::ofstream outFile("output.csv");

//    // Print title vector
//    std::vector<G4String>::const_iterator titleIter = title.begin();

//    while (titleIter != title.end()) {
//        G4cout << std::setw(8) << *titleIter << " ";
//        titleIter++;
//    }

//    G4cout << G4endl;

//    // Print current data
//    std::map<G4int, std::vector<G4double>>::const_iterator iter =
//    myMap.begin();

//    while (iter != myMap.end()) {
//        G4cout << std::setw(8) << std::setprecision(3) << iter->first << " ";

//        std::vector<G4double>::const_iterator energyBinIter =
//        iter->second.begin();

//        while (energyBinIter != iter->second.end()) {
//            G4double value = *energyBinIter;
//            G4cout << std::setw(8) << std::setprecision(3) << value << " ";
//            outFile << value << ",";
//            energyBinIter++;
//        }
//        outFile << G4endl;
//        G4cout << G4endl;
//        iter++;
//    }
//}
