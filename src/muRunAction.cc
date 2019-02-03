//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muRunAction.cc                                 *
//* ****************************************************
//  $Id: muRunAction.cc Feb 02, 2019 11:47:08Z vega $

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
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;

  G4String Particle = fPrimary->GetParticleGun()->GetParticleDefinition()->GetParticleName();
  G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();

}
