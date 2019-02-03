//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muonDetectorMessemger.cc                                 *
//* ****************************************************
//  $Id: muDetectorMessenger.cc Feb 02, 2019 11:25:08Z vega $

#include "muDetectorMessenger.hh"

muDetectorMessenger::muDetectorMessenger(muDetectorConstruction* det)
 :G4UImessenger(), fDetector(det), fMuonDir(0), fMuonDetectorDir(0), fMuonMagnetDir(0), fUpdateCmd(0){
  fMuonDir = new G4UIdirectory("/muondir/");
  fMuonDir->SetGuidance("Muon decay Experiment commands");

  fMuonDetectorDir = new G4UIdirectory("/muondir/det/");
  fMuonDetectorDir->SetGuidance("Detector Construction");

  fMuonMagnetDir = new G4UIdirectory("/muondir/magnet/");
  fMuonMagnetDir->SetGuidance("Magnet Construction");

  fDetLenCmd = new G4UIcmdWithADoubleAndUnit("/muondir/det/setLen", this);
  fDetLenCmd->SetGuidance("Set Length of the detector (X) ");
  fDetLenCmd->SetParameterName("DetectorLength", false);
  fDetLenCmd->SetRange("DetectorLength > 0.0");
  fDetLenCmd->SetUnitCategory("Length");
  fDetLenCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fDetWidCmd = new G4UIcmdWithADoubleAndUnit("/muondir/det/setWid", this);
  fDetWidCmd->SetGuidance("Set Width of the detector (Z) ");
  fDetWidCmd->SetParameterName("DetectorWidth", false);
  fDetWidCmd->SetRange("DetectorWidth > 0.0");
  fDetWidCmd->SetUnitCategory("Length");
  fDetWidCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fDetThkCmd = new G4UIcmdWithADoubleAndUnit("/muondir/det/setThk", this);
  fDetThkCmd->SetGuidance("Set Thickness of the detector (Y) ");
  fDetThkCmd->SetParameterName("DetectorThickness", false);
  fDetThkCmd->SetRange("DetectorThickness > 0.0");
  fDetThkCmd->SetUnitCategory("Length");
  fDetThkCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fMagLenCmd = new G4UIcmdWithADoubleAndUnit("/muondir/magnet/setLen", this);
  fMagLenCmd->SetGuidance("Set Length of the Magnet Plate (X) ");
  fMagLenCmd->SetParameterName("MagPlateLength", false);
  fMagLenCmd->SetRange("MagPlateLength > 0.0");
  fMagLenCmd->SetUnitCategory("Length");
  fMagLenCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fMagWidCmd = new G4UIcmdWithADoubleAndUnit("/muondir/magnet/setWid", this);
  fMagWidCmd->SetGuidance("Set Length of the Magnet Plate (Z) ");
  fMagWidCmd->SetParameterName("MagPlateWidth", false);
  fMagWidCmd->SetRange("MagPlateWidth > 0.0");
  fMagWidCmd->SetUnitCategory("Length");
  fMagWidCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fMagThkCmd = new G4UIcmdWithADoubleAndUnit("/muondir/magnet/setThk", this);
  fMagThkCmd->SetGuidance("Set Length of the Magnet Plate (Y) ");
  fMagThkCmd->SetParameterName("MagPlateWidth", false);
  fMagThkCmd->SetRange("MagPlateWidth > 0.0");
  fMagThkCmd->SetUnitCategory("Length");
  fMagThkCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fMagPlateGapCmd = new G4UIcmdWithADoubleAndUnit("/muondir/magnet/setPlateGap", this);
  fMagPlateGapCmd->SetGuidance("Set Length of the Magnet Plate  Gap (Y) ");
  fMagPlateGapCmd->SetParameterName("MagPlateGapLength", false);
  fMagPlateGapCmd->SetRange("MagPlateGapLength > 0.0");
  fMagPlateGapCmd->SetUnitCategory("Length");
  fMagPlateGapCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fUpdateCmd = new G4UIcmdWithoutParameter("/muondir/update", this);
  fUpdateCmd->SetGuidance("Update Experimental setup.");
  fUpdateCmd->SetGuidance(" This must be placed before \"beamOn\"  command  ");
  fUpdateCmd->SetGuidance("if any geometry or field has been changed.");
  fUpdateCmd->AvailableForStates(G4State_Idle);
}

muDetectorMessenger::~muDetectorMessenger(){
  delete fDetLenCmd;
  delete fDetWidCmd;
  delete fDetThkCmd;
  delete fMagLenCmd;
  delete fMagWidCmd;
  delete fMagThkCmd;
  delete fMagPlateGapCmd;
  delete fMuonDir;
  delete fMuonDetectorDir;
  delete fMuonMagnetDir;
}

void muDetectorMessenger::SetNewValue(G4UIcommand* comm, G4String newValue){
  if (comm == fDetLenCmd) fDetector->SetDetectorLength(fDetLenCmd->GetNewDoubleValue(newValue));   // Det Length set
  if (comm == fDetWidCmd) fDetector->SetDetectorWidth(fDetWidCmd->GetNewDoubleValue(newValue));   // Det Width set
  if (comm == fDetThkCmd) fDetector->SetDetectorThickness(fDetThkCmd->GetNewDoubleValue(newValue));   // Det Thickness set

  if (comm == fMagLenCmd) fDetector->SetMagnetPlateLength(fMagLenCmd->GetNewDoubleValue(newValue));   // Magnet Plate Length set
  if (comm == fMagWidCmd) fDetector->SetMagnetPlateWidth(fMagWidCmd->GetNewDoubleValue(newValue));   // Magnet Plate Width set
  if (comm == fMagThkCmd) fDetector->SetMagnetPlateThickness(fMagThkCmd->GetNewDoubleValue(newValue));   // Magnet Plate Thickness set
  if (comm == fMagPlateGapCmd) fDetector->SetMagnetPlateGap(fMagPlateGapCmd->GetNewDoubleValue(newValue));   // Magnet Plate Gap Length set

  if (comm == fUpdateCmd) fDetector->UpdateGeometry();
}
