//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muPrimaryGeneratorAction.cc                 *
//* ****************************************************
//  $Id: muPrimaryGeneratorAction.cc Feb 02, 2019 11:47:08Z vega $

#include "muPrimaryGeneratorAction.hh"

//muPrimaryGeneratorAction::muPrimaryGeneratorAction(muDetectorConstruction* det)
muPrimaryGeneratorAction::muPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(), fParticleGun(0), fEnergy(0.0*GeV),
   fParticlePos(0.0, 500.0*mm, 0.0), fParticleMom(0, -1, 0){  //, fDetector(det) {

  G4int n_Particle = 1;
  fEnergy = 4.0*GeV;
  fParticleGun = new G4ParticleGun(n_Particle);
  auto particleTable = G4ParticleTable::GetParticleTable();

  fMuonP = particleTable->FindParticle("mu+");
  fMuonM = particleTable->FindParticle("mu-");
  fParticleGun->SetParticleDefinition(fMuonP);
  fParticleGun->SetParticleEnergy(fEnergy);
  fParticleGun->SetParticlePosition(fParticlePos);
  fParticleGun->SetParticleMomentumDirection(fParticleMom);
}

muPrimaryGeneratorAction::~muPrimaryGeneratorAction(){
  delete fParticleGun;
}

void muPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
  G4double fTopPos = 0.0;
  G4double fLength = 0.0;
  G4double fWidth = 0.0;
  G4LogicalVolume* fEnvelopTopL = G4LogicalVolumeStore::GetInstance()->GetVolume("WorldLabL");
  G4Box* fTopDetector = NULL;
  if (fEnvelopTopL) fTopDetector = dynamic_cast<G4Box*>(fEnvelopTopL->GetSolid());
  if (fTopDetector) fTopPos = (fTopDetector->GetYHalfLength()) / 1.4; // Here this is +ve 1/2 of Height

  fEnvelopTopL = NULL;
  fEnvelopTopL = G4LogicalVolumeStore::GetInstance()->GetVolume("Detector0");
  if (fEnvelopTopL) fTopDetector = dynamic_cast<G4Box*>(fEnvelopTopL->GetSolid());
  if (fTopDetector){
    fLength = (2.0 * fTopDetector->GetXHalfLength()) / 1.4; //  total length of magnet
    fWidth = (2.0 * fTopDetector->GetZHalfLength()) / 1.4;  //  total width of magnet
  }

  G4double x0 = (2.0 * G4UniformRand() - 1.0) * 0.9 * fLength;
  G4double z0 = (2.0 * G4UniformRand() - 1.0) * 0.9 * fWidth;
  G4double y0 = fTopPos;

  fParticlePos = G4ThreeVector(x0, y0, z0);
  //fParticleGun->SetParticleEnergy(4.0*GeV);
  fParticleGun->SetParticlePosition(fParticlePos);
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0, -1.0, 0.0));
  fParticleGun->GeneratePrimaryVertex(anEvent);

}
