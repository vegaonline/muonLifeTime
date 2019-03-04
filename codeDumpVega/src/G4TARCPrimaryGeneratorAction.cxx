/*******************************************************
 * @file      G4TARCPrimaryGeneratorAction.cxx
 * @author    Abhijit Bhattacharyya
 * @brief     Primary Generator using GPS
 ******************************************************/

#include "G4TARCPrimaryGeneratorAction.hh"

G4TARCPrimaryGeneratorAction::G4TARCPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(), fGPS(0), fEnergy(0.0*MeV)//,  fHisto(0)
 ,fGunMess(0), fBeamFlag(false), fCurrent(0)
{
  //	G4AutoLock lock(&myPrimGenMutex);
    fGPS = new G4GeneralParticleSource();
    //fHisto = G4TARCHistoManager::GetPointer();
    fGunMess = new G4TARCPrimaryMessenger(this);
    //fGunMess = new G4TARCPrimaryMessenger();
}

G4TARCPrimaryGeneratorAction::~G4TARCPrimaryGeneratorAction() {
  //G4AutoLock lock(&myPrimGenMutex);
  delete fGPS;
}

void G4TARCPrimaryGeneratorAction::GeneratePrimaries( G4Event* anEvent ){
  //	G4AutoLock lock(&myPrimGenMutex);
  G4double tmp = fGPS->GetParticleEnergy();
  if (DefaultBeamPosition()){
    G4cout << " Ok in Gun Mess\n";
  }
  fGPS->GeneratePrimaryVertex(anEvent);
  if (!GetEnergy())  SetEnergy(tmp);
}
