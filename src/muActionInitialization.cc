//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muActionInitialization.cc                 *
//* ****************************************************
//  $Id: muActionInitialization.cc Mar 03, 2019 11:47:08Z vega $

#include "muActionInitialization.hh"

muActionInitialization::muActionInitialization(muDetectorConstruction* detConst)
   : G4VUserActionInitialization(), fDetector(detConst), fMasterRunAction(new muRunAction()){

   }
muActionInitialization::~muActionInitialization(){

}

void muActionInitialization::Build() const {
  SetUserAction(new muPrimaryGeneratorAction);
  SetUserAction(new muRunAction);
  auto eventAction = new muEventAction;
  SetUserAction(eventAction);
  SetUserAction(new muSteppingAction(fDetector, eventAction));
  SetUserAction(new muStackingAction(eventAction));
}

void muActionInitialization::BuildForMaster() const {
  G4UserRunAction* run_Action = new muRunAction;
  SetUserAction(run_Action);
}
