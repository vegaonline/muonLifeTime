//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muPrimaryGeneratorAction.hh                 *
//* ****************************************************
//  $Id: muPrimaryGeneratorAction.hh Feb 14, 2019 10:25:08Z vega $
#ifndef muPrimaryGeneratorAction_H
#define muPrimaryGeneratorAction_H 1

#include "muDetectorConstruction.hh"

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"
#include "globals.hh"

class G4Event;
class G4ParticleGun;
class G4generalParticleSource;
class G4ParticleDefinition;
class muDetectorConstruction;

class muPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  //muPrimaryGeneratorAction(muDetectorConstruction*);
  muPrimaryGeneratorAction();
  virtual ~muPrimaryGeneratorAction();
public:
  virtual void GeneratePrimaries(G4Event*);
  G4ParticleGun* GetParticleGun() {return fParticleGun;}

private:
  muPrimaryGeneratorAction & operator=( const muPrimaryGeneratorAction &right );
  muPrimaryGeneratorAction( const muPrimaryGeneratorAction& );

private:
  G4ParticleGun* fParticleGun;

  G4double fEnergy;
  G4ParticleDefinition* fMuonP;
  G4ParticleDefinition* fMuonM;
  G4ThreeVector fParticlePos;
  G4ThreeVector fParticleMom;
  G4double fMiniDist = 20.0*cm; // this is just a small distance on the top of the topmost detector
};

#endif
