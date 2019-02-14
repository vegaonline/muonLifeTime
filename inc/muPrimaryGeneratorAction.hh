//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muPrimaryGeneratorAction.hh                 *
//* ****************************************************
//  $Id: muNuclearBuilder.hh Feb 14, 2019 10:25:08Z vega $
#ifndef muPrimaryGeneratorActionr_H
#define muPrimaryGeneratorAction_H 1

#include "muDetectorConstruction.hh"

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "globals.hh"

class G4Event;
class muDetectorConstruction;

class muPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  muPrimaryGeneratorAction(muDetectorConstruction*);
  ~muPrimaryGeneratorAction();
public:
  virtual void GeneratePrimaries(G4Event*);
  G4ParticleGun* GetParticleGun() {return fParticleGun;}

private:
  G4ParticleGun* fParticleGun;
  muDetectorConstruction* fDetector;
};

#endif
