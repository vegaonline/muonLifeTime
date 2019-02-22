//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muNuclearBuilder.cc                         *
//* ****************************************************
//  $Id: muNuclearBuilder.cc Feb 11, 2019 09:47:08Z vega $
#include "muNuclearBuilder.hh"

muNuclearBuilder::muNuclearBuilder(const G4String& name) : G4VPhysicsConstructor(name) {

}

muNuclearBuilder::~muNuclearBuilder() {

}

void muNuclearBuilder::ConstructProcess() {
  G4ProcessManager* pManager = 0;
  G4MuonNuclearProcess* muNucProcess = new G4MuonNuclearProcess();
  G4MuonVDNuclearModel* muNucModel = new G4MuonVDNuclearModel();
  muNucProcess->RegisterMe(muNucModel);

  pManager = G4MuonPlus::MuonPlus()->GetProcessManager();
  pManager->AddDiscreteProcess(muNucProcess);

  pManager = G4MuonMinus::MuonMinus()->GetProcessManager();
  pManager->AddDiscreteProcess(muNucProcess);
/*
  G4ParticleDefinition* particle = G4MuonPlus::MuonPlus();       // Add standard EM process for MUON
  pManager = particle->GetProcessManager();
  pManager->AddProcess(new G4MuNuclearInteraction("muNucl"), -1, -1, 4);

  particle = G4MuonMinus::MuonMinus();
  pManager = particle->GetProcessManager();

  pManager->AddProcess(new G4MuNuclearInteraction("muNucl"), -1, -1, 4);
  */
}
