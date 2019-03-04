//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muEventAction.cc                 *
//* ****************************************************
//  $Id: muEventAction.cc Mar 03, 2019 11:47:08Z vega $

#include "muEventAction.hh"

muEventAction::muEventAction() {

}

muEventAction::~muEventAction(){

}

void muEventAction::BeginOfEventAction(const G4Event* evt){
  fEventID = evt->GetEventID();
  G4cout << " Begin of Event #" << fEventID << G4endl;
}

void muEventAction::EndOfEventAction(const G4Event* evt) {
  G4AnalysisManager* fAnalysisMan = G4AnalysisManager::Instance();


}
