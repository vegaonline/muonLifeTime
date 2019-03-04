/***************************************************************
 * @file       G4TARCPhysicsList.hh
 * @author     Abhijit Bhattacharyya
 * @brief      Physics list
 **************************************************************/

#include "G4TARCPhysicsList.hh"

G4TARCPhysicsList::G4TARCPhysicsList()
: G4VModularPhysicsList()
//, fEmPhysicsList(0), fThermal(true), fParticleList(0) 
{
    verboseLevel     = 1;
    fThermal         = true;

    G4LossTableManager::Instance();
    fCut4Gamma       = defCutValue;
    fCut4Electron    = defCutValue;
    fCut4Positron    = defCutValue;
    fCut4Proton      = defCutValue; //0.0*mm;

    // Radioactive Decay Physics
    RegisterPhysics(new G4RadioactiveDecayPhysics);
    // Particles
    fParticleList    = new G4DecayPhysics("decays");   //"Decay");
    // EM Physics
    fEmPhysicsList   = new G4EmStandardPhysics(verboseLevel);
}

G4TARCPhysicsList::~G4TARCPhysicsList() {
  delete fParticleList;
  delete fEmPhysicsList;

  for (size_t i = 0; i < fHadronPhys.size(); i++)
    delete fHadronPhys[i];
}

void G4TARCPhysicsList::ConstructParticle() {
  G4BaryonConstructor::ConstructParticle();
  G4MesonConstructor::ConstructParticle();
  G4LeptonConstructor::ConstructParticle();
  G4BosonConstructor::ConstructParticle();
  G4IonConstructor::ConstructParticle();
  G4ShortLivedConstructor::ConstructParticle();
  fParticleList->ConstructParticle();
}

void G4TARCPhysicsList::ConstructProcess() {
  AddTransportation();
  fEmPhysicsList->ConstructProcess();
  fParticleList->ConstructProcess();
  neutronProcess();
  G4cout << " Hadron Physics number: " << fHadronPhys.size() << G4endl;
  for ( size_t i = 0; i < fHadronPhys.size(); i++ ){
    fHadronPhys[i]->ConstructProcess();
    fHadronPhys[i]->SetVerboseLevel(1);
  }
}

void G4TARCPhysicsList::neutronProcess(){
  G4ParticleDefinition* neutron = G4Neutron::Neutron();
  G4ProcessManager* pManager = neutron->GetProcessManager();

  // delete all neutron processes if already registered
  //
  G4ProcessTable* processTable = G4ProcessTable::GetProcessTable();
  G4VProcess* process = 0;
  process = processTable->FindProcess("hadElastic", neutron);
  if (process) pManager->RemoveProcess(process);
  //
  process = processTable->FindProcess("neutronInelastic", neutron);
  if (process) pManager->RemoveProcess(process);
  //
  process = processTable->FindProcess("nCapture", neutron);
  if (process) pManager->RemoveProcess(process);
  //
  process = processTable->FindProcess("nFission", neutron);
  if (process) pManager->RemoveProcess(process);

  //process->SetVerboseLevel(0);
  // (re) create process: elastic
  //
  G4HadronElasticProcess* process1 = new G4HadronElasticProcess();
  process1->SetVerboseLevel(1);
  pManager->AddDiscreteProcess(process1);
  //
  // model1a
  G4ParticleHPElastic*  model1a = new G4ParticleHPElastic();
  process1->RegisterMe(model1a);
  process1->AddDataSet(new G4ParticleHPElasticData());
  //
  // model1b
  if (fThermal) {
    model1a->SetMinEnergy(4.0*eV);
    G4ParticleHPThermalScattering* model1b = new G4ParticleHPThermalScattering();
    process1->RegisterMe(model1b);
    process1->AddDataSet(new G4ParticleHPThermalScatteringData());
  }
  // (re) create process: inelastic
  //
  G4NeutronInelasticProcess* process2 = new G4NeutronInelasticProcess();
  process2->SetVerboseLevel(1);
  pManager->AddDiscreteProcess(process2);
  //
  // cross section data set
  G4ParticleHPInelasticData* dataSet2 = new G4ParticleHPInelasticData();
  process2->AddDataSet(dataSet2);
  //
  // models
  G4ParticleHPInelastic* model2 = new G4ParticleHPInelastic();
  process2->RegisterMe(model2);

  // (re) create process: nCapture
  //
  G4HadronCaptureProcess* process3 = new G4HadronCaptureProcess();
  process3->SetVerboseLevel(1);
  pManager->AddDiscreteProcess(process3);
  //
  // cross section data set
  G4ParticleHPCaptureData* dataSet3 = new G4ParticleHPCaptureData();
  process3->AddDataSet(dataSet3);
  //
  // models
  G4ParticleHPCapture* model3 = new G4ParticleHPCapture();
  process3->RegisterMe(model3);
  // (re) create process: nFission
  //
  G4HadronFissionProcess* process4 = new G4HadronFissionProcess();
  process4->SetVerboseLevel(1);
  pManager->AddDiscreteProcess(process4);
  //
  // cross section data set
  G4ParticleHPFissionData* dataSet4 = new G4ParticleHPFissionData();
  process4->AddDataSet(dataSet4);
  //
  // models
  G4ParticleHPFission* model4 = new G4ParticleHPFission();
  process4->RegisterMe(model4);
}


void G4TARCPhysicsList::AddPhysicsList(const G4String& name) {
  if ( verboseLevel > 1 ) {
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
  }

  if ( name == "emstandard_opt0" ) {
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics();
  } else if ( name == "emstandard_opt1" ) {
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option1();
  } else if (name == "emstandard_opt2") {
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option2();
  } else if (name == "emstandard_opt3") {
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option3();
  } else if (name == "emstandard_opt4") {
    delete fEmPhysicsList;
    fEmPhysicsList = new G4EmStandardPhysics_option4();
  } else if (name == "FTFP_BERT_EMV") {
    AddPhysicsList("emstandard_opt1");
    AddPhysicsList("FTFP_BERT");
  } else if (name == "FTFP_BERT_EMX") {
    AddPhysicsList("emstandard_opt2");
    AddPhysicsList("FTFP_BERT");
  } else if (name == "FTFP_BERT_EMZ") {
    AddPhysicsList("emstandard_opt4");
    AddPhysicsList("FTFP_BERT");
  } else if (name == "FTFP_BERT") {
    //SetBuilderList1();
    fHadronPhys.push_back( new G4HadronPhysicsFTFP_BERT() );
  } else if (name == "FTFPlowBERT") {
    //SetBuilderList1();
    //fHadronPhys.push_back( new G4HadronInFTFPlowBERT() );
  } else if (name == "FTFP_BERT_TRV") {
    SetBuilderList3();
    fHadronPhys.push_back( new G4HadronPhysicsFTFP_BERT());
  } else if (name == "FTF_BIC") {
    //SetBuilderList0();
    fHadronPhys.push_back( new G4HadronPhysicsFTF_BIC());
    fHadronPhys.push_back( new G4NeutronCrossSectionXS(verboseLevel));
  } else if (name == "QBBC") {
    // AddPhysicsList("emstandard_opt0");
    SetBuilderList2();
    fHadronPhys.push_back( new G4HadronInelasticQBBC());
  } else if (name == "QGSP_BERT") {
    SetBuilderList1();
    fHadronPhys.push_back( new G4HadronPhysicsQGSP_BERT());
  } else if (name == "QGSP_FTFP_BERT") {
    SetBuilderList1();
    fHadronPhys.push_back( new G4HadronPhysicsQGSP_FTFP_BERT());
  } else if (name == "QGSP_FTFP_BERT_EMV") {
    AddPhysicsList("emstandard_opt1");
    AddPhysicsList("QGSP_FTFP_BERT");
  } else if (name == "QGSP_BERT_EMV") {
    AddPhysicsList("emstandard_opt1");
    AddPhysicsList("QGSP_BERT");
  } else if (name == "QGSP_BERT_EMX")  {
    AddPhysicsList("emstandard_opt2");
    AddPhysicsList("QGSP_BERT");
  } else if (name == "QGSP_BERT_HP")  {
    SetBuilderList1(true);
    fHadronPhys.push_back( new G4HadronPhysicsQGSP_BERT_HP());
  } else if (name == "QGSP_BIC") {
    SetBuilderList0();
    fHadronPhys.push_back( new G4HadronPhysicsQGSP_BIC());
  } else if (name == "QGSP_BIC_EMY") {
    AddPhysicsList("emstandard_opt3");
    SetBuilderList0();
    fHadronPhys.push_back( new G4HadronPhysicsQGSP_BIC());
  } else if (name == "QGSP_BIC") {
    SetBuilderList0();
    fHadronPhys.push_back( new G4HadronPhysicsQGS_BIC());
    fHadronPhys.push_back( new G4NeutronCrossSectionXS(verboseLevel));
  } else if (name == "QGSP_BIC_HP") {
    // AddPhysicsList("emstandard_opt0");
    SetBuilderList0(true);
    fHadronPhys.push_back( new G4HadronPhysicsQGSP_BIC_HP());
  } else if (name == "QGSP_INCLXX_HP") {
    // AddPhysicsList("emstandard_opt0");
    SetBuilderList0(true);
    fHadronPhys.push_back( new G4HadronPhysicsINCLXX("inclxx-hp",true,true,false));
  } else if (name == "QGSP_INCLXX") {
    // AddPhysicsList("emstandard_opt0");
    SetBuilderList0(false);
    fHadronPhys.push_back( new G4HadronPhysicsINCLXX("inclxx",true,false,false));
  } else {
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
          << " is not defined"
          << G4endl;
  }
}

void G4TARCPhysicsList::SetBuilderList0( G4bool flagHP) {
  fHadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));

  if(flagHP) {
    fHadronPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
  } else {
    fHadronPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
  }
  fHadronPhys.push_back( new G4StoppingPhysics(verboseLevel));
  fHadronPhys.push_back( new G4IonBinaryCascadePhysics(verboseLevel));
  fHadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));

// Neutron tracking cut
  RegisterPhysics( new G4NeutronTrackingCut(verboseLevel) );
}

void G4TARCPhysicsList::SetBuilderList1( G4bool flagHP) {
  fHadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
  if(flagHP) {
    fHadronPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
  } else  {
    fHadronPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
    // activate G4LMsdGenerator:
    // fHadronPhys.push_back( new G4HadronHElasticPhysics(verboseLevel,true) );
  }
  fHadronPhys.push_back( new G4StoppingPhysics(verboseLevel));
  fHadronPhys.push_back( new G4IonPhysics(verboseLevel));
  fHadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}

void G4TARCPhysicsList::SetBuilderList2() {
  fHadronPhys.push_back( new G4EmExtraPhysics(verboseLevel) );
  fHadronPhys.push_back( new G4DecayPhysics(verboseLevel) );
  fHadronPhys.push_back( new G4HadronElasticPhysicsXS(verboseLevel) );
  fHadronPhys.push_back( new G4StoppingPhysics(verboseLevel) );
  fHadronPhys.push_back( new G4IonPhysics(verboseLevel) );
  fHadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel) );
}


void G4TARCPhysicsList::SetBuilderList3( G4bool flagHP) {
  fHadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
  if(flagHP) {
    fHadronPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
  }  else  {
    //  fHadronPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
    // activate G4LMsdGenerator:
    fHadronPhys.push_back( new G4HadronHElasticPhysics(verboseLevel,true) );
  }
  fHadronPhys.push_back( new G4StoppingPhysics(verboseLevel));
  fHadronPhys.push_back( new G4IonPhysics(verboseLevel));
  fHadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}


void G4TARCPhysicsList::SetBuilderList4( G4bool flagHP) {
  fHadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
  if(flagHP) {
    fHadronPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
  } else   {
    //  fHadronPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
    // activate G4LMsdGenerator:
    // fHadronPhys.push_back( new G4QqElasticPhysics(verboseLevel,true) );
  }
  fHadronPhys.push_back( new G4StoppingPhysics(verboseLevel));
  fHadronPhys.push_back( new G4IonPhysics(verboseLevel));
  fHadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}


void G4TARCPhysicsList::SetCuts()  {
  if ( verboseLevel > 0 )   {
    G4cout << "PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }
  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(fCut4Gamma, "gamma");
  SetCutValue(fCut4Electron, "e-");
  SetCutValue(fCut4Positron, "e+");
  SetCutValue(fCut4Proton, "proton");
  //                    if ( verboseLevel > 0 ) { DumpCutValuesTable(); }
}

void G4TARCPhysicsList::SetCut4Gamma(G4double cut)
{
  fCut4Gamma = cut;
  SetParticleCuts(fCut4Gamma, G4Gamma::Gamma());
}

void G4TARCPhysicsList::SetCut4Electron(G4double cut)
{
  fCut4Electron = cut;
  SetParticleCuts(fCut4Electron, G4Electron::Electron());
}

void G4TARCPhysicsList::SetCut4Positron(G4double cut)
{
  fCut4Positron = cut;
  SetParticleCuts(fCut4Positron, G4Positron::Positron());
}

void G4TARCPhysicsList::SetCut4Proton(G4double cut)
{
  fCut4Proton = cut;
  SetParticleCuts(fCut4Proton, G4Proton::Proton());
}


void G4TARCPhysicsList::List() {
  G4cout << "### PhysicsLists available: FTFP_BERT FTFP_BERT_EMV "
         << "FTFP_BERT_EMX FTFP_BERT_EMZ"
         << G4endl;
  G4cout << "                            FTF_BIC QBBC QGSP_BERT "
         << "QGSP_BERT_EMV QGSP_BERT_EMX"
         << G4endl;
  G4cout << "                            QGSP_BERT_HP QGSP_FTFP_BERT "
         << "QGSP_FTFP_BERT_EMV"
         << G4endl;
  G4cout << "                            QGS_BIC QGSP_BIC QGSP_BIC_EMY "
         << "QGSP_BIC_HP"
         << G4endl;
}
