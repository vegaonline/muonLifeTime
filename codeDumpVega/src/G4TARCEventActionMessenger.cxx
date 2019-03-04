#include "G4TARCEventActionMessenger.hh"

G4TARCEventActionMessenger::G4TARCEventActionMessenger(G4TARCEventAction* evAct)
: G4UImessenger(), fEventAction(evAct), fPrintCmd(0), fCmd(0) {
  fPrintCmd = new G4UIcmdWithAnInteger("/TARC/PrintModulo", this);
  fPrintCmd->SetGuidance("Print events modulo n");
  fPrintCmd->SetParameterName("EventNb", false);
  fPrintCmd->SetRange("EventNb > 0");
  fPrintCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fCmd = new G4UIcmdWithAnInteger("/TARC/DebugEvent", this);
  fCmd->SetGuidance("D event to debug");
  fCmd->SetParameterName("fNb", false);
  fCmd->SetRange("fNb > 0");
  fCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

G4TARCEventActionMessenger::~G4TARCEventActionMessenger() {
  delete fPrintCmd;
  delete fCmd;
}

void G4TARCEventActionMessenger::SetNewValue(G4UIcommand* comm, G4String newValue) {
  if(comm == fPrintCmd)
   {fEventAction->SetPrintModulo(fPrintCmd->GetNewIntValue(newValue));}
 //if(comm == fCmd)
   //{fEventAction->AddEventToDebug(fCmd->GetNewIntValue(newValue));}
}
