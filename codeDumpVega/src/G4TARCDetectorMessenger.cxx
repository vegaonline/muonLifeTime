#include "G4TARCDetectorMessenger.hh"

G4TARCDetectorMessenger::G4TARCDetectorMessenger(G4TARCDetectorConstruction* det)
: //G4UImessenger(), fTestDir(0),
  fDetector(det)//,
  //,fPhysListCmd(0), fListCmd(0), fThermalCmd(0), fPhysDir(0)
 //fBeamCmd(0), fCurrentCmd(0), fEdepCmd(0), fBinCmd(0), fVerbCmd(0)
 {
  fTestDir = new G4UIdirectory("/tarc/");
  fTestDir->SetGuidance("TARC Experiment");

  fPhysDir = new G4UIdirectory("/tarc/phys/");
  fPhysDir->SetGuidance("TARC Physics related commands");

  fPhysListCmd = new G4UIcmdWithAString("/tarc/phys/physics",this);
  fPhysListCmd->SetGuidance("Add Modular Physics List ");
  fPhysListCmd->SetParameterName("PList", false);
  fPhysListCmd->AvailableForStates(G4State_PreInit);

  fListCmd = new G4UIcmdWithoutParameter("/tarc/phys/listphysics",this);
  fListCmd->SetGuidance("Available Physics List ");
  fListCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fThermalCmd = new G4UIcmdWithABool("/tarc/phys/thermalScattering", this);
  fThermalCmd->SetGuidance("set thermal scattering model");
  fThermalCmd->SetParameterName("thermal", false);
  fThermalCmd->AvailableForStates(G4State_PreInit);
}

G4TARCDetectorMessenger::~G4TARCDetectorMessenger() {
  delete fTestDir;
  delete fPhysDir;
  delete fPhysListCmd;
  delete fListCmd;
  delete fThermalCmd;
}


void G4TARCDetectorMessenger::SetNewValue(G4UIcommand* comm, G4String newValue) {
  if (comm == fThermalCmd){
    G4cout << "here in thermalCmd\n";
    fPhysList->SetThermalPhysics(fThermalCmd->GetNewBoolValue(newValue));
  } else if( comm == fPhysListCmd ) {
    if(fPhysList) {
      G4String name = newValue;
      if(name == "PHYSLIST") {
        char* path = std::getenv(name);
        G4cout << " ENV::name---> " << path << G4endl;
        if (path) name = G4String(path);
        else {
          G4cout << "### PhysicsListMessenger WARNING: "
                 << " environment variable PHYSLIST is not defined"
                 << G4endl;
          return;
        }
      }
      fPhysList->AddPhysicsList(name);
    } else {
      G4cout << "### PhysicsListMessenger WARNING: "
             << " /tarc/phys/physics UI command is not available "
             << "for reference Physics List" << G4endl;
    }
  } else if( comm == fListCmd ) {
    if(fPhysList) {
      fPhysList->List();
    } else {
      G4cout << "### PhysicsListMessenger WARNING: "
             << " /tarc/phys/listphysics UI command is not available "
             << "for reference Physics List" << G4endl;
    }
  }
}
