//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muRunAction.cc                              *
//* ****************************************************
//  $Id: muRunAction.cc Feb 02, 2019 11:47:08Z vega $

#include "muRunAction.hh"

muRunAction::muRunAction(muDetectorConstruction* det, muPrimaryGeneratorAction* prim)
 : G4UserRunAction(), fDetector(det), fPrimary(prim) {

}

muRunAction::~muRunAction(){

}

void muRunAction::BeginOfRunAction(const G4Run* aRun) {
  G4cout << "### Run " << aRun->GetRunID() << " start. " << G4endl;

  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  CLHEP::HepRandom::showEngineStatus();
}


void muRunAction::EndOfRunAction(const G4Run* aRun){
  G4Material* material;
  G4double density = 0.0, energy = 0.0, lengthY = 0.0;
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;

  material = fDetector->GetDetectorMaterial();
  lengthY = fDetector->GetDetectorThickness();
  density = material->GetDensity();
  G4String Particle = fPrimary->GetParticleGun()->GetParticleDefinition()->GetParticleName();
  energy = fPrimary->GetParticleGun()->GetParticleEnergy();
  G4cout << "\n The run consists of "  << NbOfEvents << " "<< Particle << " of "
         << energy / MeV   << " MeV "  << " through "
         << lengthY / mm   << " mm "   << " of "
         << material->GetName()                 << " (density: "
         << " )" << G4endl;

}
