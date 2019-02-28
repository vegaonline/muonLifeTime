//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muPrimaryGeneratorAction.cc                 *
//* ****************************************************
//  $Id: muPrimaryGeneratorAction.cc Feb 02, 2019 11:47:08Z vega $

#include "muPrimaryGeneratorAction.hh"

muPrimaryGeneratorAction::muPrimaryGeneratorAction(muDetectorConstruction* det)
 : G4VUserPrimaryGeneratorAction(), fParticleGun(0), fDetector(det) {
  G4int n_Particle = 1;
  fParticleGun = new G4ParticleGun(n_Particle);
  auto particleTable = G4ParticleTable::GetParticleTable();
  fMuonP = particleTable->FindParticle("mu+");
  fMuonM = particleTable->FindParticle("mu-");
  G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
  fParticleGun->SetParticleDefinition(fMuonM);
  fParticleGun->SetParticleEnergy(4.0*GeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.0, 100.0*m, 0.0));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0, -1.0, 0.0));
}

muPrimaryGeneratorAction::~muPrimaryGeneratorAction(){
  delete fParticleGun;
}

void muPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
  G4double x0 = (2.0 * G4UniformRand() - 1.0) * 0.9 * (fDetector->GetDetectorLength());
  G4double z0 = (2.0 * G4UniformRand() - 1.0) * 0.9 * (fDetector->GetDetectorWidth());
  G4double y0 = fDetector->GetTopPosition() + fMiniDist;
  //G4cout << "x0 " << x0 << " y0 " << y0 << " z0 " << z0 << G4endl;

  fParticleGun->SetParticleEnergy(4.0*GeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0, -1.0, 0.0));
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
