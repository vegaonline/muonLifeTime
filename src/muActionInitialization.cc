//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muActionInitialization.cc                                 *
//* ****************************************************
//  $Id: muActionInitialization.cc Feb 02, 2019 11:47:08Z vega $

#include "muActionInitialization.hh"

muActionInitialization::muActionInitialization(muDetectorConstruction* detector) : G4VUserActionInitialization(), fDetector(detector){

}

muActionInitialization::~muActionInitialization(){

}

void muActionInitialization::BuildForMaster() const {
  muRunAction* runAction = new muRunAction(fDetector, 0);
  SetUserAction(runAction);
}

void muActionInitialization::Build() const {
  muPrimaryGeneratorAction* muPrimary = new muPrimaryGeneratorAction();
  SetUserAction(muPrimary);

  muRunAction* muRAction = new muRunAction(fDetector, muPrimary);
  SetUserAction(muRAction);

}
