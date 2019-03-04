#include "G4TARCStackingMessenger.hh"

G4TARCStackingMessenger::G4TARCStackingMessenger(G4TARCStackingAction* stk) :G4UImessenger(), fStackingAction(stk), fKillCmd(0), fKCmd(0){
  fKillCmd = new G4UIcmdWithABool("/tarc/KillAllSecondaries", this);
  fKillCmd->SetGuidance("  Choice : true | false");
  fKillCmd->SetParameterName("choice", true);
  fKillCmd->SetDefaultValue(false);

  fKCmd = new G4UIcmdWithAString("/tarc/Kill", this);
  fKCmd->SetGuidance("Kill secondary Particles of defined type");
  fKCmd->SetParameterName("ch", true);
  fKCmd->SetDefaultValue("none");
  fKCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}


G4TARCStackingMessenger::~G4TARCStackingMessenger() {
  delete fKillCmd;
  delete fKCmd;
}

void G4TARCStackingMessenger::SetNewValue(G4UIcommand* comm, G4String newValue) {
  if (comm == fKillCmd) {
    fStackingAction->SetKillStatus(fKillCmd->GetNewBoolValue(newValue));
  } else if (comm == fKCmd){
    fStackingAction->SetKill(newValue);
  }
}
