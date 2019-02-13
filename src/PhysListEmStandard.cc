//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file PhysListEmStandard.cc                                 *
//* ****************************************************
//  $Id: PhysListEmStandard.cc Feb 13, 2019 11:47:08Z vega $

#include "PhysListEmStandard.hh"

PhysListEmStandard::PhysListEmStandard(const G4String& name) : G4VPhysicsConstructor(name) {
  G4EmParameters* param = G4EmParameters::Instance();
  param->SetDefaults();
  param->SetMinEnergy(100.0*eV);
  param->SetMaxEnergy(1000.0*PeV);
}

PhysListEmStandard::~PhysListEmStandard() {

}

void PhysListEmStandard::ConstructProcess() {
  // mu+
  G4ParticleDefinition* particle = G4MuonPlus::MuonPlus();
  G4ProcessManager* pmanager = particle->GetProcessManager();
  pmanager->AddProcess(new G4MuIonisation(),     -1, 2, 2);
  pmanager->AddProcess(new G4MuBremsstrahlung(), -1, 3, 3);
  pmanager->AddProcess(new G4MuPairProduction(), -1, 4, 4);

  // mu-
  particle = G4MuonMinus::MuonMinus();
  pmanager = particle->GetProcessManager();
  pmanager->AddProcess(new G4MuIonisation(),     -1, 2, 2);
  pmanager->AddProcess(new G4MuBremsstrahlung(), -1, 3, 3);
  pmanager->AddProcess(new G4MuPairProduction(), -1, 4, 4);

  // pi+
  particle = G4PionPlus::PionPlus();
  pmanager = particle->GetProcessManager();
  pmanager->AddProcess(new G4hIonisation(),     -1, 2, 2);
  pmanager->AddProcess(new G4hBremsstrahlung(), -1, 3, 3);
  pmanager->AddProcess(new G4hPairProduction(), -1, 4, 4);

  // pi-
  particle = G4PionMinus::PionMinus();
  pmanager = particle->GetProcessManager();
  pmanager->AddProcess(new G4hIonisation(),     -1, 2, 2);
  pmanager->AddProcess(new G4hBremsstrahlung(), -1, 3, 3);
  pmanager->AddProcess(new G4hPairProduction(), -1, 4, 4);

  // proton
  particle = G4Proton::Proton();
  pmanager = particle->GetProcessManager();
  pmanager->AddProcess(new G4hIonisation(),     -1, 2, 2);
  pmanager->AddProcess(new G4hBremsstrahlung(), -1, 3, 3);
  pmanager->AddProcess(new G4hPairProduction(), -1, 4, 4);

}
