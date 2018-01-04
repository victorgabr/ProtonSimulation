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
//                 GEANT 4
// --------------------------------------------------------------
//
// Code developed by:  Victor Gabriel Leandro Alves
// Copyright 2008-2017
//    *******************************
//    *                             *
//    *    DetectorConstruction.cc                *
//    *                             *
//    *******************************

#include "DetectorConstruction.hh"
// Scoring
//#include "G4MultiFunctionalDetector.hh"
//#include "G4PSDoseDeposit.hh"
//#include "G4PSEnergyDeposit.hh"
//#include "G4PSEnergyDeposit3D.hh"
//#include "G4SDManager.hh"
//#include "G4SDParticleFilter.hh"
//#include "G4TransportationManager.hh"
//#include "G4VSensitiveDetector.hh"
#include <G4PSCylinderSurfaceCurrent.hh>
#include <G4PSEnergyDeposit.hh>
#include <G4SDManager.hh>
#include <G4SDParticleFilter.hh> // Geant4 includes
//
//#include "G4GeometryManager.hh"
//#include "G4VisAttributes.hh"
//#include "globals.hh"

//DetectorConstruction::DetectorConstruction(G4String &File) {
//    readGDML(File);
//}

// ----------------------------------------------------------------------------
//
// Destructor
//
DetectorConstruction::DetectorConstruction(const G4String File):_gdmlFile(File)
{
    this->readGDML();
}

DetectorConstruction::~DetectorConstruction() {
    //    if(detectorMessenger) delete detectorMessenger;
}

G4VPhysicalVolume *DetectorConstruction::Construct() {
    // set scoring using aux information fom GDML
    setScoring();
    TraverseReplicas(fWorldPhysVol, 0, G4Transform3D());
    return fWorldPhysVol;
}

void DetectorConstruction::readGDML() {
    // **** LOOK HERE*** FOR READING GDML FILES
    parser.Read(_gdmlFile, false);
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
    // Giving World Physical Volume from GDML Parser
    //
    fWorldPhysVol = parser.GetWorldVolume();
}

void DetectorConstruction::setScoring() {
    //--------------------------------------------- ------------------------------
    // Scoring implementation
    //---------------------------------------------------------------------------
    // Detector pointer
    G4MultiFunctionalDetector *detector =
        new G4MultiFunctionalDetector("MyDetector");

    //  SDManager pointer
    G4SDManager *manager = G4SDManager::GetSDMpointer();

    // registering detector
    manager->AddNewDetector(detector);

    ///////////////////////////////////////////////////////////////////////
    // Example how to retrieve Auxiliary Information for sensitive detector
    //
    const G4GDMLAuxMapType *auxmap = parser.GetAuxMap();
    G4cout << "Found " << auxmap->size()
           << " volume(s) with auxiliary information." << G4endl << G4endl;
    for (G4GDMLAuxMapType::const_iterator iter = auxmap->begin();
            iter != auxmap->end(); iter++) {
        G4cout << "Volume " << ((*iter).first)->GetName()
               << " has the following list of auxiliary information: " << G4endl
               << G4endl;
        for (G4GDMLAuxListType::const_iterator vit = (*iter).second.begin();
                vit != (*iter).second.end(); vit++) {
            G4cout << "--> Type: " << (*vit).type << " Value: " << (*vit).value
                   << G4endl;
        }
    }
    G4cout << G4endl;

    for (G4GDMLAuxMapType::const_iterator iter = auxmap->begin();
            iter != auxmap->end(); iter++) {
        G4cout << "Volume " << ((*iter).first)->GetName()
               << " has the following list of auxiliary information: " << G4endl
               << G4endl;
        for (G4GDMLAuxListType::const_iterator vit = (*iter).second.begin();
                vit != (*iter).second.end(); vit++) {
            if ((*vit).type == "SensDet") {
                G4cout << "Attaching sensitive detector " << (*vit).value
                       << " to volume " << ((*iter).first)->GetName() << G4endl
                       << G4endl;

                G4VSensitiveDetector *mydet =
                    manager->FindSensitiveDetector("MyDetector");

                if (mydet) {
                    G4LogicalVolume *myvol = (*iter).first;

                    myvol->SetSensitiveDetector(mydet);
                    G4cout << "Sensisite detector was set " << G4endl;

                } else {
                    G4cout << (*vit).value << " detector not found" << G4endl;
                }
            }
        }
    }

    //    G4PSDoseDeposit* scorer = new G4PSDoseDeposit("myScorer");
    G4PSEnergyDeposit *scorer = new G4PSEnergyDeposit("myScorer");
    //    G4PSCylinderSurfaceCurrent *scorer =
    //        new G4PSCylinderSurfaceCurrent("myScorer", fCurrent_In);
    //    G4PSEnergyDeposit3D *scorer =
    //        new G4PSEnergyDeposit3D("myScorer", 1, 1, nScoreVols);

    // filtering protons
    G4SDParticleFilter *protonFilter = new G4SDParticleFilter("protonFilter");
    protonFilter->add("proton");
    // filtering protons
    scorer->SetFilter(protonFilter);
    // Registering scorer
    detector->RegisterPrimitive(scorer);

    G4cout << "Created G4MultiFunctionalDetector named " << detector->GetName()
           << ", and a G4SDParticleFilter "
           << "named " << scorer->GetName() << G4endl;
}

std::map<G4int, CLHEP::Hep3Vector> DetectorConstruction::GetScorerPositions() {
    return scorerPositions;
}

void DetectorConstruction::DescriptionFcnPtr(G4VPhysicalVolume *aPV,
        G4int aDepth, G4int replicaNo,
        const G4Transform3D &aTransform) {

    if (aPV->GetCopyNo() == -1) {
        G4cout << "Name: " << aPV->GetName() << G4endl;
        G4cout << "copy number: " << aPV->GetCopyNo() << G4endl;
        G4cout << "replicaNo: " << replicaNo << G4endl;
        G4cout << "aDepth: " << aDepth << G4endl;
        G4cout << "Transform: " << aTransform.getTranslation() << G4endl;
        G4cout << "Z pos: " << aTransform.getTranslation()[2] << G4endl;
        scorerPositions[replicaNo] = aTransform.getTranslation();
    }
}

void DetectorConstruction::TraverseReplicas(G4VPhysicalVolume *aPV,
        G4int aDepth,
        const G4Transform3D &aTransform) {
    // Recursively visit all of the geometry below the physical volume
    // pointed to by aPV including replicas.
    G4ThreeVector originalTranslation = aPV->GetTranslation();
    G4RotationMatrix *pOriginalRotation = aPV->GetRotation();

    if (aPV->IsReplicated()) {
        EAxis axis;
        G4int nReplicas;
        G4double width, offset;
        G4bool consuming;

        aPV->GetReplicationData(axis, nReplicas, width, offset, consuming);

        for (G4int n = 0; n < nReplicas; n++) {
            switch (axis) {
            default:
            case kXAxis:
                aPV->SetTranslation(
                    G4ThreeVector(-width * (nReplicas - 1) * 0.5 + n * width, 0, 0));
                aPV->SetRotation(0);
                break;
            case kYAxis:
                aPV->SetTranslation(
                    G4ThreeVector(0, -width * (nReplicas - 1) * 0.5 + n * width, 0));
                aPV->SetRotation(0);
                break;
            case kZAxis:
                aPV->SetTranslation(
                    G4ThreeVector(0, 0, -width * (nReplicas - 1) * 0.5 + n * width));
                aPV->SetRotation(0);
                break;
            case kRho:
                aPV->SetTranslation(G4ThreeVector(0, 0, 0));
                aPV->SetRotation(0);
                break;
            case kPhi: {
                G4RotationMatrix rotation;
                rotation.rotateZ(-(offset + (n + 0.5) * width));
                // Minus Sign because for the physical volume we need the
                // coordinate system rotation.
                aPV->SetTranslation(G4ThreeVector(0, 0, 0));
                aPV->SetRotation(&rotation);
            }
            break;

            } // axis switch
            DescribeAndDescendGeometry(aPV, aDepth, n, aTransform);
        } // num replicas for loop
    }   // if replicated
    else {
        DescribeAndDescendGeometry(aPV, aDepth, aPV->GetCopyNo(), aTransform);
    }
    // Restore original transformation...
    aPV->SetTranslation(originalTranslation);
    aPV->SetRotation(pOriginalRotation);
}

void DetectorConstruction::DescribeAndDescendGeometry(
    G4VPhysicalVolume *aPV, G4int aDepth, G4int replicaNo,
    const G4Transform3D &aTransform) {

    G4Transform3D *transform =
        new G4Transform3D(*(aPV->GetObjectRotation()), aPV->GetTranslation());

    G4Transform3D newTransform = aTransform * (*transform);

    delete transform;
    // Call the routine we use to print out geometry descriptions, make
    // tables, etc. It needs to be a member function of the class
    this->DescriptionFcnPtr(aPV, aDepth, replicaNo, newTransform);
    G4int nDaughters = aPV->GetLogicalVolume()->GetNoDaughters();

    for (G4int iDaughter = 0; iDaughter < nDaughters; iDaughter++) {
        TraverseReplicas(aPV->GetLogicalVolume()->GetDaughter(iDaughter),
                         aDepth + 1, newTransform);
    }
}
