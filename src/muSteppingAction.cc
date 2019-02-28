//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muSteppingAction.cc                              *
//* ****************************************************
//  $Id: muSteppingAction.cc Feb 28, 2019 21:47:08Z vega $

#include "muSteppingAction.hh"

muSteppingAction::muSteppingAction(muRunAction* myRunAction) : G4UserSteppingAction(), fRunAction(myRunAction){

}

muSteppingAction::~muSteppingAction(){

}

void muSteppingAction::UserSteppingAction(const G4Step* myStep){
  G4Track* myTrack = myStep->GetTrack();
  G4int StepNum = myTrack->GetCurrentStepNumber();
  G4StepPoint* startPoint = myStep->GetPreStepPoint();
  G4StepPoint* endPoint = myStep->GetPostStepPoint();
  G4ParticleDefinition* thisParticleType = myTrack->GetDefinition();
  G4String thisParticleName = thisParticleType->GetParticleName();

  G4String procName = endPoint->GetProcessDefinedStep()->GetProcessName();
  G4double E0 = startPoint->GetKineticEnergy();
  G4double timeStamp = startPoint->GetGlobalTime();
  G4double edep = myStep->GetTotalEnergyDeposit();
  G4double posPre = startPoint->GetPosition().mag();
  G4double posPost =endPoint->GetPosition().mag();
}
