//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muEventAction.cc                 *
//* ****************************************************
//  $Id: muEventAction.cc Mar 03, 2019 11:47:08Z vega $

#include "muEventAction.hh"

muEventAction::muEventAction()
  : G4UserEventAction(), fEnergyMuD0(0.0), fEnergyMuD1(0.0),
    fTrackMuD0(0.0), fTrackMuD1(0.0){

}

muEventAction::~muEventAction(){

}

void muEventAction::BeginOfEventAction(const G4Event* evt){
  fEventID = evt->GetEventID();
  G4cout << " Begin of Event #" << fEventID << G4endl;
  fEnergyMuD0 = 0.0;
  fEnergyMuD1 = 0.0;
  fTrackMuD0 = 0.0;
  fTrackMuD1 = 0.0;
}

void muEventAction::EndOfEventAction(const G4Event* ) {
  G4AnalysisManager* fAnalysisMan = G4AnalysisManager::Instance();
  fAnalysisMan->FillH1(0, fEnergyMuD0);
  fAnalysisMan->FillH1(1, fEnergyMuD1);
  fAnalysisMan->FillH1(2, fTrackMuD0);
  fAnalysisMan->FillH1(3, fTrackMuD1);

  fAnalysisMan->FillNtupleDColumn(0, fEnergyMuD0);
  fAnalysisMan->FillNtupleDColumn(1, fEnergyMuD1);
  fAnalysisMan->FillNtupleDColumn(2, fTrackMuD0);
  fAnalysisMan->FillNtupleDColumn(3, fTrackMuD1);
  fAnalysisMan->AddNtupleRow();
}
