//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muSteppingAction.cc                              *
//* ****************************************************
//  $Id: muSteppingAction.cc Feb 28, 2019 21:47:08Z vega $

#include "muSteppingAction.hh"

//muSteppingAction::muSteppingAction(muRunAction* myRunAction, muHistoManager* muHistM)
muSteppingAction::muSteppingAction(const muDetectorConstruction* detConst,  muEventAction* anEvent)
 : G4UserSteppingAction(), fDetector(detConst), fEventAction(anEvent) { //fRunAction(myRunAction), muHisto(muHistM){

}

muSteppingAction::~muSteppingAction(){

}

void muSteppingAction::UserSteppingAction(const G4Step* myStep){
  G4double edep = 0.0, stepLength = 0.0;

  G4Track* myTrack = myStep->GetTrack();
  // G4int StepNum = myTrack->GetCurrentStepNumber();
  G4StepPoint* startPoint = myStep->GetPreStepPoint();
  G4StepPoint* endPoint = myStep->GetPostStepPoint();
  auto volume = startPoint->GetTouchableHandle()->GetVolume();
  G4ParticleDefinition* thisParticleType = myTrack->GetDefinition();
  G4String thisParticleName = thisParticleType->GetParticleName();

  if (volume){
    if (thisParticleName == "mu+"){
      edep = myStep->GetTotalEnergyDeposit();
      if (myStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.0) stepLength = myStep->GetStepLength();
      if (volume == fDetector->GetD0PV())  fEventAction->AddEdepMuD0(edep, stepLength);
      if (volume == fDetector->GetD1PV())  fEventAction->AddEdepMuD1(edep, stepLength);
      //if (volume->GetName() == "Detector1")  fEventAction->AddEdepMuD1(edep, stepLength);
    }
  }

  //G4String procName = endPoint->GetProcessDefinedStep()->GetProcessName();
  //G4double E0 = startPoint->GetKineticEnergy();
  //G4double timeStamp = startPoint->GetGlobalTime();
  //G4double posPre = startPoint->GetPosition().mag();
  //G4double posPost =endPoint->GetPosition().mag();
/*
  G4int id = 0;
  if (procName == "muIoni")          id = 1;
  else if (procName == "muPairProd") id = 2;
  else if (procName == "muBrems")    id = 3;
  //  else if (procName == "muNucl")     id = 4;
  else if (procName == "muonNuclear")id = 4;
  else if (procName == "hIoni")      id = 5;
  else if (procName == "hPairProd")  id = 6;
  else if (procName == "hBrems")     id = 7;
  */
  //muHisto->FillHisto(id,edep);
  //G4cout << thisParticleName << " process: " << procName << " ID # " << id
  //       << " Energy deposition: " << edep/keV << " keV." << G4endl;

}
