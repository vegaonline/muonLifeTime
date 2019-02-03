//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muPrimaryGeneratorAction.cc                                 *
//* ****************************************************
//  $Id: muPrimaryGeneratorAction.cc Feb 02, 2019 11:47:08Z vega $

muPrimaryGeneratorAction::muPrimaryGeneratorAction(muDetectorConstruction* det) : G4VUserPrimaryGeneratorAction(), fParticleGun(0), fDetector(det) {
  G4int n_Particle = 1;
  fParticleGun = new G4ParticleGun(n_Particle);
  G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(4.0*GeV);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0, -1.0, 0.0));
}

muPrimaryGeneratorAction::~muPrimaryGeneratorAction(){
  delete fParticleGun;
}

void muPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
  G4double x0 = (2.0 * G4UniformRand() - 1.0) * 0.9 * (fDetector->GetDetectorLength());
  G4double y0 = fDetector->GetTopPosition();
  G4double z0 = fDetector->GetDetectorWidth();

  fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
