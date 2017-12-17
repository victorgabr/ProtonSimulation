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
//
// --------------------------------------------------------------
//                 GEANT 4 - Based on Brachytherapy example
// --------------------------------------------------------------
//
// Code developed by:
// S.Guatelli
// Extended by Victor Alves
//    ********************************************
//    *                                          *
//    *    PrimaryGeneratorAction.cc       *
//    *                                          *
//    ********************************************
//
// $Id: PrimaryGeneratorAction.ccBrachytherapy example 84280 2014-10-13
// 07:21:10Z gcosmo $
//

#include "globals.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "PrimaryGeneratorAction.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() {
    // Use the GPS to generate primary particles,
    // Particle type, energy position, direction are specified in the
    // the macro file primary.mac
    gun = new G4GeneralParticleSource();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete gun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
    gun->GeneratePrimaryVertex(anEvent);
}

G4double PrimaryGeneratorAction::getParticleEnergy() {

    G4double energy = gun->GetParticleEnergy();
    return energy;
}
