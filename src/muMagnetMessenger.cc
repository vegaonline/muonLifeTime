//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muMagnetMessenger.cc                        *
//* ****************************************************
//  $Id: muMagnetMessenger.cc Feb 02, 2019 10:47:08Z vega $

#include "muMagnetMessenger.hh"

muMagnetMessenger::muMagnetMessenger(muMagneticField* fieldSetup)
  : G4UImessenger(), fMagSetup(fieldSetup), fFieldDir(0), fStepperCmd(0),
    fMagFieldYCmd(0), fMagFieldCmd(0), fLocalMagneticFieldCmd(0), fMinStepCmd(0), fUpdateCmd(0){
      fFieldDir = new G4UIdirectory("/muondir/magnet/");
      fFieldDir->SetGuidance("Muons Magnet control.");

      fStepperCmd = new G4UIcmdWithAnInteger("/muondir/magnet/setStepperType", this);
      fStepperCmd->SetGuidance("Select stepper type for magnetic field");
      fStepperCmd->SetParameterName("choice", true);
      fStepperCmd->SetDefaultValue(4);
      fStepperCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

      fUpdateCmd = new G4UIcmdWithoutParameter("/muondir/magnet/update", this);
      fUpdateCmd->SetGuidance("Update geometry, if it has changed.");
      fUpdateCmd->AvailableForStates(G4State_Idle);

      fMagFieldYCmd = new G4UIcmdWithADoubleAndUnit("/muondir/magnet/setFieldY", this);
      fMagFieldYCmd->SetGuidance("Define global magnetic field (along Y direction).");
      fMagFieldYCmd->SetParameterName("By", false, false);
      fMagFieldYCmd->SetDefaultUnit("tesla");
      fMagFieldYCmd->AvailableForStates(G4State_Idle);

      fMagFieldCmd = new G4UIcmdWith3VectorAndUnit("/muondir/magnet/setField", this);
      fMagFieldCmd->SetGuidance("Define global magnetic field (Bx, By, Bz)");
      fMagFieldCmd->SetParameterName("Bx", "By", "Bz", false, false);
      fMagFieldCmd->SetDefaultUnit("tesla");
      fMagFieldCmd->AvailableForStates(G4State_Idle);

      fLocalMagneticFieldCmd = new G4UIcmdWith3VectorAndUnit("/muondir/magnet/setLocalField", this);
      fLocalMagneticFieldCmd->SetGuidance("Define local magnetic field");
      fLocalMagneticFieldCmd->SetParameterName("Blx", "Bly", "Blz", false, false);
      fLocalMagneticFieldCmd->SetDefaultUnit("tesla");
      fLocalMagneticFieldCmd->AvailableForStates(G4State_Idle);

      fMinStepCmd = new G4UIcmdWithADoubleAndUnit("/muondir/magnet/setMinStep", this);
      fMinStepCmd->SetGuidance("Define minimal step and field is along Y.");
      fMinStepCmd->SetParameterName("min step", false, false);
      fMinStepCmd->SetDefaultUnit("mm");
      fMinStepCmd->AvailableForStates(G4State_Idle);
    }

    muMagnetMessenger::~muMagnetMessenger() {
      delete fStepperCmd;
      delete fMagFieldYCmd;
      delete fMagFieldCmd;
      delete fLocalMagneticFieldCmd;
      delete fMinStepCmd;
      delete fFieldDir;
      delete fUpdateCmd;
    }

void muMagnetMessenger::SetNewValue(G4UIcommand* comm, G4String newValue) {
  if (comm == fStepperCmd)            fMagSetup->SetStepperType(fStepperCmd->GetNewIntValue(newValue));
  if (comm == fUpdateCmd)             fMagSetup->UpdateField();
  if (comm == fMagFieldYCmd)          fMagSetup->SetFieldYValue(fMagFieldYCmd->GetNewDoubleValue(newValue));
  if (comm == fMagFieldCmd)           fMagSetup->SetFieldValue(fMagFieldCmd->GetNew3VectorValue(newValue));
  if (comm == fLocalMagneticFieldCmd) fMagSetup->SetLocalFieldValue(fLocalMagneticFieldCmd->GetNew3VectorValue(newValue));
  fMagSetup->UpdateField();
  if (comm == fMinStepCmd)            fMagSetup->SetMinStep(fMinStepCmd->GetNewDoubleValue(newValue));
}
