#include "G4TARCEventAction.hh"

G4TARCEventAction::G4TARCEventAction(): fNeutronStack(0)  {
    //fDebugStarted = false;
    // fEventMessenger = new G4TARCEventActionMessenger (this);
    //fUITARC = G4UImanager::GetUIpointer();
    //fHisto = G4TARCHistoManager::GetPointer();
    //auto fAnalysisManager = G4AnalysisManager::Instance();
    //fSelected = 0;
    SetPrintModulo(1);
    fPrimarySet = false;
    fMaxRadCount = 0;
}

G4TARCEventAction::~G4TARCEventAction() {
  // delete fEventMessenger;
}

void G4TARCEventAction::BeginOfEventAction( const G4Event* evt ){
  fEventID = evt->GetEventID();
  fNeutronStack = 0;
 
  // Following three lines are required for passing value to SteppingAction variables.
  G4TARCRun* thisRun = static_cast<G4TARCRun*> (G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  fExptRadiiTables = thisRun->fExptRadiiTables;
  fMaxRadCount = thisRun->fMaxRadCount;
  thisRun->SetNumberOfEvents(fEventID);
  
  //if (fEventID % fPrintModulo == 0)
  G4cout << " Begin of Event:  " << fEventID << G4endl;
}

void G4TARCEventAction::EndOfEventAction( const G4Event* evt) {
  G4TARCRun* thisRun = static_cast<G4TARCRun*> (G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  G4AnalysisManager*  fAnalysisManager = G4AnalysisManager::Instance();

  fAnalysisManager->FillH1(5, fNeutronStack);
  if (!fPrimarySet){
    auto primary = evt->GetPrimaryVertex(0)->GetPrimary(0);
    thisRun->SetBeamParticleName(primary->GetG4code()->GetParticleName());
    thisRun->SetBeamEnergy(primary->GetKineticEnergy());
    fPrimarySet = true;
  }
}

void G4TARCEventAction::analyseSecondaries(G4double energyL, G4String nameL, G4double timeL, G4double momentumL,
  G4int ParentIDL, G4double primaryEnergyL, G4double parentEnergyL, G4String parentParticleL, G4bool reduced_fluxL,
  G4int number_generationsL){
    //G4cout << " Analyse Secondary started" << G4endl;

  G4TARCRun* thisRun = static_cast<G4TARCRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());

  G4int Iparticle=-9;
  G4double temp_time = timeL / microsecond;
  G4double temp_energy = energyL / eV;
  G4double temp_momentum = momentumL;

  if(nameL == "gamma") {
    thisRun->AddFlux(nameL);
    Iparticle = 1;
  } else if(nameL == "neutron") {
    Iparticle = 2;
    thisRun->AddFlux(nameL);
    if(!reduced_fluxL)  thisRun->AddFlux("neutron_check");
    if(reduced_fluxL)   Iparticle = -2;
    if(energyL > 0.1*eV && energyL < 10.0*keV) thisRun->AddFlux("neutron_fluence");
  } else if(nameL == "e-") {
    thisRun->AddFlux(nameL);
    Iparticle = 3;
  } else if(nameL == "pi-") {
    thisRun->AddFlux(nameL);
    Iparticle = 4;
  } else if(nameL == "pi+") {
    thisRun->AddFlux(nameL);
    Iparticle = 5;
  } else if(nameL == "pi0") {
    thisRun->AddFlux(nameL);
    Iparticle = 6;
  } else if(nameL == "e+") {
    thisRun->AddFlux(nameL);
    Iparticle = 7;
  } else if(nameL == "proton") {
    thisRun->AddFlux(nameL);
    Iparticle = 8;
  } else if(nameL == "mu-") {
    thisRun->AddFlux(nameL);
    Iparticle = 9;
  } else if(nameL == "mu+") {
    thisRun->AddFlux(nameL);
    Iparticle = 10;
  } else if (nameL == "anti_proton") {
    thisRun->AddFlux(nameL);
  } else if (nameL == "alpha"){
    thisRun->AddFlux(nameL);
  }  else if (nameL == "He3"){
    thisRun->AddFlux(nameL);
  } else if (nameL == "deuteron"){
    thisRun->AddFlux(nameL);
  } else if (nameL == "triton"){
    thisRun->AddFlux(nameL);
  }else  {
    thisRun->AddFlux("other");
    Iparticle = 99;
    return;
  }
  G4int iParent = 0;
  if (parentParticleL == "gamma")        iParent = 1;
  else if (parentParticleL == "neutron") iParent = 2;
  else if(reduced_fluxL)                 iParent = -2;
  else if (parentParticleL == "e-")      iParent = 3;
  else if (parentParticleL == "pi-")     iParent = 4;
  else if (parentParticleL == "pi+")     iParent = 5;
  else if (parentParticleL == "pi0")     iParent = 6;
  else if (parentParticleL == "e+")      iParent = 7;
  else if (parentParticleL == "proton")  iParent = 8;
  else if (parentParticleL == "anti_proton")  iParent = 8;
  else if (parentParticleL == "mu-")     iParent = 9;
  else if (parentParticleL == "mu+")     iParent = 10;
  else if (parentParticleL == "alpha")     iParent = 11;
  else if (parentParticleL == "He3")     iParent = 12;
  else if (parentParticleL == "deuteron")     iParent = 13;
  else if (parentParticleL == "triton")     iParent = 14;

  auto fAnalysisManager = G4AnalysisManager::Instance();
  
  fAnalysisManager->FillNtupleDColumn(0,0, temp_energy);
  fAnalysisManager->FillNtupleDColumn(0,1, temp_time);
  fAnalysisManager->FillNtupleIColumn(0,2, Iparticle);
  fAnalysisManager->FillNtupleDColumn(0,3, temp_momentum);
  fAnalysisManager->FillNtupleIColumn(0,4, ParentIDL);
  fAnalysisManager->FillNtupleDColumn(0,5, primaryEnergyL);
  fAnalysisManager->FillNtupleIColumn(0,6, iParent);
  fAnalysisManager->FillNtupleDColumn(0,7, parentEnergyL);
  fAnalysisManager->FillNtupleIColumn(0,8, number_generationsL);
  fAnalysisManager->FillNtupleIColumn(0,9, fEventID);
  
  fAnalysisManager->AddNtupleRow(0);
  
}

void G4TARCEventAction::NeutronEnergyTime(G4double thisE, G4double thisT, G4double E0){
  auto fAnalysisManager = G4AnalysisManager::Instance();
  G4double tempT = thisT / microsecond;
  G4double tempE = thisE / eV;
  G4double tempE0 = E0 / eV;
  
  if (tempT > 0.0 && tempE > 0.0) fAnalysisManager->FillH2(0, log10(tempT), log10(tempE), 1.0);
  fAnalysisManager->FillNtupleDColumn(1, 0, tempE);
  fAnalysisManager->FillNtupleDColumn(1, 1, tempT);
  fAnalysisManager->FillNtupleDColumn(1, 2, tempE0);
  fAnalysisManager->AddNtupleRow(1);
  
}

void G4TARCEventAction::otherEnergyTime(G4double thisE, G4double thisT, G4double E0){
  auto fAnalysisManager = G4AnalysisManager::Instance();
  G4double tempT = thisT / microsecond;
  G4double tempE = thisE / eV;
  G4double tempE0 = E0 / eV;
  
  if (tempT > 0.0 && tempE > 0.0) fAnalysisManager->FillH2(1, log10(tempT), log10(tempE), 1.0);
  fAnalysisManager->FillNtupleDColumn(12, 0, tempE);
  fAnalysisManager->FillNtupleDColumn(12, 1, tempT);
  fAnalysisManager->FillNtupleDColumn(12, 2, tempE0);
  fAnalysisManager->AddNtupleRow(12);     // ID 14 becomes 12 after commenting out 12 and 13
  
}


void G4TARCEventAction::exitingTally(G4bool exiting_flag, G4double energyL){
  auto fAnalysisManager = G4AnalysisManager::Instance();
  if(exiting_flag) {
    G4TARCRun* thisRun = static_cast<G4TARCRun*> (G4RunManager::GetRunManager()->GetNonConstCurrentRun());
    thisRun->CalcExitingFlux(energyL);
    
    fAnalysisManager->FillNtupleDColumn(2, 0, energyL / eV);
    fAnalysisManager->AddNtupleRow(2);
    
  }
}

void G4TARCEventAction::exitingTallyCheck(G4bool exiting_flag_check){
  if (exiting_flag_check){
    G4TARCRun* thisRun = static_cast<G4TARCRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
    thisRun->ExitingTallyCheck();
  }
}

void G4TARCEventAction::analysePS(G4double fParticleEnergy, G4String fParticleName, G4double fParticleMomentum){
  G4TARCRun* thisRun = static_cast<G4TARCRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());

  auto fAnalysisManager = G4AnalysisManager::Instance();
  if(fParticleName == "gamma") {
    fAnalysisManager->FillH1(0, fParticleEnergy/eV);
  } else if(fParticleName == "neutron") {
    //++fNeutCap;
    //G4cout << fNeutCap << "     " << fNeutronStack << G4endl;
    fAnalysisManager->FillH1(1, fParticleEnergy / eV, 1.0 / fParticleMomentum);
    if(fParticleEnergy / MeV < 2.0) {
      thisRun->fNeutflux[0] += 1.0;
      thisRun->fENflux[0] += fParticleEnergy;
    } else if(fParticleEnergy / MeV >= 2.0 && fParticleEnergy / MeV < 20.0) {
      thisRun->fNeutflux[1] += 1.0;
      thisRun->fENflux[1] += fParticleEnergy / MeV;
    } else if(fParticleEnergy / MeV >= 20.0) {
      thisRun->fNeutflux[2] += 1.0;
      thisRun->fENflux[2] += fParticleEnergy / MeV;
    }
    if(fParticleEnergy / MeV >= 1000.0) {
      thisRun->fNeutflux[3] += 1.0;
      thisRun->fENflux[3] += fParticleEnergy / MeV;
    }
  } else if(fParticleName == "e-") {
    fAnalysisManager->FillH1(2, fParticleEnergy / eV);
  } else if(fParticleName == "e+") {
    fAnalysisManager->FillH1(3, fParticleEnergy / eV);
  } else {   //(fParticleName == "other") {
    fAnalysisManager->FillH1(4, fParticleEnergy / eV);
  }
  //  G4cout << " Exiting  analysePS" << G4endl;
}

void G4TARCEventAction::analyseNeutronRadialFluence(G4double fParticleEnergyL, //G4double fParticleTimeL,
  G4double StepLengthL, G4int ishellL){
    G4TARCRun* thisRun = static_cast<G4TARCRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
    thisRun->analyseNeutronRadialFluence(fParticleEnergyL, StepLengthL, ishellL);
}


void G4TARCEventAction::analyseNeutronShellFluence(G4double energyL, G4double StepLengthL){
  G4TARCRun* thisRun = static_cast<G4TARCRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  thisRun->analyseNeutronShellFluence(energyL, StepLengthL);
}

  //void G4TARCHistoManager::analyseNeutronFluence(G4double energyL, G4String& nameL, G4double timeL, G4double momentumL,
  //  G4int thisTrackIDL, G4double radiusL, G4double thisStepL,  G4int ParentIDL, G4double primaryEnergyL,
  //  G4double parentEnergyL, G4String& parentParticleL, G4bool reduced_fluxL,  G4int number_generationsL){


void G4TARCEventAction::analyseNeutronFluence(G4double energyL,
   //G4double timeL, G4int thisTrackIDL,  G4double radiusL,
   G4double thisStepL  //,  G4int ParentIDL, G4double parentEnergyL, G4String& parentParticleL
 ){
      G4TARCRun* thisRun = static_cast<G4TARCRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
      thisRun->analyseNeutronFluence(energyL, thisStepL);
}


void G4TARCEventAction::analyseNeutronFlux(G4double n_EnergyL, G4int thisTrackIDL, G4double radiusL, G4double cosAngleL, G4String fParticleNameL)
    //G4double zPosL,G4double cosAngleL, G4String fParticleNameL)
    {
      G4TARCRun* thisRun = static_cast<G4TARCRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
      thisRun->analyseNeutronFlux(n_EnergyL,  thisTrackIDL, radiusL, cosAngleL, fParticleNameL);
}

void G4TARCEventAction::ExtraNeutronProcs(const G4String& procID){
  G4TARCRun* thisRun = static_cast<G4TARCRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  thisRun->NeutronExtraCalc(procID);
}
