//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muPhysicsList.cc                                 *
//* ****************************************************
//  $Id: muPhysicsList.cc Feb 02, 2019 22:47:08Z vega $

#include "muPhysicsList.hh"

muPhysicsList::muPhysicsList() : G4VModularPhysicsList(), fEmPhysicsList(0), fMuNuclPhysicsList(0){
  fEmName = G4String("emstandard_opt0");         // EM Physics
  fEmPhysicsList = new G4EmStandardPhysics();
  G4EmParameters* param = G4EmParameters::Instance();
  param->SetMinEnergy(100.0*eV);
  param->SetMaxEnergy(1000.0*PeV);
  fMuNuclPhysicsList = 0;
}

muPhysicsList::~muPhysicsList() {

}

void muPhysicsList::ConstructParticle() {
    G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();
}

void muPhysicsList::ConstructProcess() {
  AddTransportation();
  fEmPhysicsList->ConstructProcess();
  if (fMuNuclPhysicsList) fMuNuclPhysicsList->ConstructProcess();
}

void muPhysicsList::AddPhysicsList(const G4String& name) {
  if (name == fEmName) return;
  if (name == "emstandard_opt0"){
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics();
  } else if (name == "emstandard_opt4"){
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option4();
  } else if (name == "local") {
    fEmName = name;
    delete fEmPhysicsList;
    fEmPhysicsList = new PhysListEmStandard(name);
  } else if (name == "muNucl") {
    fMuNuclPhysicsList = new MuNuclearBuilder(name);
  } else {
    G4cout << "muPhysicsList::AddPhysicsList: < " << name << " >  is not defined." << G4endl;
  }
}
