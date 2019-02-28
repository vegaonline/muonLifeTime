//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muStackingAction.cc                              *
//* ****************************************************
//  $Id: muStackingAction.cc Feb 28, 2019 21:47:08Z vega $

#include "muStackingAction.hh"

muStackingAction::muStackingAction() : G4UserStackingAction(){

}

muStackingAction::~muStackingAction(){

}

G4ClassificationOfNewTrack muStackingAction::ClassifyNewTrack(const G4Track* aTrack){
  G4ClassificationOfNewTrack classification = fUrgent;
  if (aTrack->GetParentID() !=0) classification = fKill;
  return classification;
}
