//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muPrimaryGeneratorMessenger.cc                 *
//* ****************************************************
//  $Id: muPrimaryGeneratorMessenger.cc Feb 26, 2019 11:47:08Z vega $

#include "muPrimaryGeneratorMessenger.hh"


muPrimaryGeneratorMessenger::muPrimaryGeneratorMessenger(muPrimaryGeneratorAction* action)
   : G4UImessenger(), fAction(action){

}


muPrimaryGeneratorMessenger::~muPrimaryGeneratorMessenger() {

}

void muPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* comm, G4String newValue){
  
}
