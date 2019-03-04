#include "G4TARCPrimaryMessenger.hh"

G4TARCPrimaryMessenger::G4TARCPrimaryMessenger(G4TARCPrimaryGeneratorAction* primGen)
: G4UImessenger(), fGUN(primGen)
,fGunDir(0),  fBeamCmd(0) {
  fGunDir =new G4UIdirectory("/tarc/beam/");
  fGunDir->SetGuidance("Gun configuration");

  fBeamCmd = new G4UIcmdWithABool("/tarc/beam/DefaultBeamPosition", this);
  fBeamCmd->SetGuidance("Gun set as default or as specified by the user <ACTIVE>");

  fCurrentCmd = new G4UIcmdWithADoubleAndUnit("/tarc/beam/Current", this);
  fCurrentCmd->SetGuidance("Set proton Current");
  fCurrentCmd->SetParameterName("Current", false);
  fCurrentCmd->SetDefaultUnit("microampere");
  fCurrentCmd->SetRange("Current>0");
  fCurrentCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

G4TARCPrimaryMessenger::~G4TARCPrimaryMessenger() {
  delete fGunDir;
  delete fBeamCmd;
  delete fCurrentCmd;
}

void G4TARCPrimaryMessenger::SetNewValue(G4UIcommand* comm, G4String newVal) {
  if (comm == fBeamCmd) {
    fGUN->SetDefaultBeamPositionFlag(fBeamCmd->GetNewBoolValue(newVal));
  } else if (comm == fCurrentCmd) {
    fGUN->SetCurrent(fCurrentCmd->GetNewDoubleValue(newVal));
  }
}
