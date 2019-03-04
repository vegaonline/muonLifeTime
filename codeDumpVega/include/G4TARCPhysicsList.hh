#ifndef G4TARC_PHYSICSLIST_H
#define G4TARC_PHYSICSLIST_H

//#include "G4TARCPhysicsListMessenger.hh"
#include "G4VModularPhysicsList.hh"
#include "globals.hh"

#include "G4RadioactiveDecayPhysics.hh"
#include "G4RadioactiveDecaymessenger.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"

#include "G4ChargeExchangePhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4IonPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4EmProcessOptions.hh"

#include "G4ParticleHPElasticData.hh"
#include "G4ParticleHPElastic.hh"
#include "G4ParticleHPInelasticData.hh"
#include "G4ParticleHPInelastic.hh"
#include "G4ParticleHPThermalScatteringData.hh"
#include "G4ParticleHPThermalScattering.hh"
#include "G4ParticleHPCaptureData.hh"
#include "G4ParticleHPCapture.hh"
#include "G4ParticleHPFissionData.hh"
#include "G4ParticleHPFission.hh"

#include "G4NeutronInelasticProcess.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4NeutronCrossSectionXS.hh"

#include "G4HadronElasticPhysics.hh"
#include "G4HadronHElasticPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronCaptureProcess.hh"
#include "G4HadronFissionProcess.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsFTF_BIC.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGS_BIC.hh"
#include "G4HadronPhysicsINCLXX.hh"
#include "G4HadronPhysicsQGSP_FTFP_BERT.hh"

#include "G4PionMinusInelasticProcess.hh"
#include "G4PionPlusInelasticProcess.hh"
#include "G4ProtonInelasticProcess.hh"

#include "G4KaonMinusInelasticProcess.hh"
#include "G4KaonPlusInelasticProcess.hh"
#include "G4KaonZeroSInelasticProcess.hh"
#include "G4KaonZeroLInelasticProcess.hh"
#include "G4AntiProtonInelasticProcess.hh"

#include "G4LossTableManager.hh"

#include "G4ProcessManager.hh"
#include "G4ProcessTable.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"

#include "G4BaryonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4SystemOfUnits.hh"


class G4VPhysicsConstructor;
class G4TARCPhysicsListMessenger;

class G4TARCPhysicsList : public G4VModularPhysicsList {
public:
  G4TARCPhysicsList();
  virtual ~G4TARCPhysicsList();

  virtual void ConstructParticle();
  virtual void ConstructProcess();
  virtual void SetCuts();
  virtual void neutronProcess();

  void SetCut4Gamma(G4double);
  void SetCut4Electron(G4double);
  void SetCut4Positron(G4double);
  void SetCut4Proton(G4double);

  void AddPhysicsList( const G4String& );
  void List();

  virtual void SetThermalPhysics(G4bool flag) { fThermal = flag;}

private:
  void SetBuilderList0(G4bool flagHP = false);
  void SetBuilderList1(G4bool flagHP = false);
  void SetBuilderList3(G4bool flagHP = false);
  void SetBuilderList4(G4bool flagHP = false);
  void SetBuilderList2();

  G4VPhysicsConstructor*              fEmPhysicsList;
  G4VPhysicsConstructor*              fParticleList;

  std::vector<G4VPhysicsConstructor*> fHadronPhys;

  G4double defCutValue = 0.7*mm;

  G4double fCut4Gamma;
  G4double fCut4Electron;
  G4double fCut4Positron;
  G4double fCut4Proton;

  G4bool   fThermal;
};


#endif
