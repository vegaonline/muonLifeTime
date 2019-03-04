/****************************************************
 *  @file    G4TARCActionInitialization.hh
 *  @author  Abhijit Bhattacharyya
 *  @brief   Action Initialization
 ***************************************************/

#include "G4TARCActionInitialization.hh"

//G4TARCActionInitialization::G4TARCActionInitialization(G4TARCDetectorConstruction* det)
//: G4VUserActionInitialization(), fGeomConstruct(det){}
G4TARCActionInitialization::G4TARCActionInitialization(): G4VUserActionInitialization(), fMasterRunAction(new G4TARCRunAction()){}

G4TARCActionInitialization::~G4TARCActionInitialization() {}

void G4TARCActionInitialization::Build() const {
  
  SetUserAction(new G4TARCPrimaryGeneratorAction);
  SetUserAction(new G4TARCRunAction);
  auto eventAction = new G4TARCEventAction;
  SetUserAction(eventAction);
  SetUserAction(new G4TARCSteppingAction(eventAction));
  SetUserAction(new G4TARCStackingAction(eventAction));
}


void G4TARCActionInitialization::BuildForMaster() const {
  G4UserRunAction* run_action = new G4TARCRunAction;
  SetUserAction(run_action);
}

