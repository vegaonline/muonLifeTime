/********************************************************
 * @file      G4TARCPrimaryGeneratorAction.hh
 * @author    Abhijit Bhattacharyya
 * @brief     Primary Primary Generator Action
 ********************************************************/

#ifndef G4TARC_PRIMARYGENERATORACTION_H
#define G4TARC_PRIMARYGENERATORACTION_H

//#include "G4TARCHistoManager.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Event.hh"
#include "G4GenericMessenger.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"
//#include "G4AutoLock.hh"

#include "G4TARCPrimaryMessenger.hh"
#include "G4TARCRun.hh"

//namespace {G4Mutex myPrimGenMutex = G4MUTEX_INITIALIZER;}

class G4GeneralParticleSource;
class G4Event;

class G4TARCPrimaryMessenger;
//class G4TARCHistoManager;


class G4TARCPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  G4TARCPrimaryGeneratorAction();
  virtual ~G4TARCPrimaryGeneratorAction();
  virtual void GeneratePrimaries( G4Event*);

  void SetEnergy(G4double val)                            { fEnergy = val; }
  void SetMomentum(G4double val)                          { fMomentum = val; }
  void SetDefaultBeamPositionFlag(G4bool f)        { fBeamFlag = f; }
  void SetCurrent(G4double val)                    { fCurrent = val;}


  G4double GetEnergy()                    const           { return fEnergy; }
  G4double GetMomentum()                  const           { return fMomentum; }

  inline G4bool DefaultBeamPosition()     const           { return fBeamFlag; }

private:
  G4TARCPrimaryGeneratorAction & operator=( const G4TARCPrimaryGeneratorAction &right );
  G4TARCPrimaryGeneratorAction( const G4TARCPrimaryGeneratorAction& );

  //G4TARCHistoManager*        fHisto;
  G4GeneralParticleSource*   fGPS;
  G4double                   fEnergy;
  G4double                   fMomentum;
  G4TARCPrimaryMessenger*    fGunMess;// = 0;
  G4bool                     fBeamFlag;
  G4double                   fCurrent;
};


#endif
