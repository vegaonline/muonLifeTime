//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muDetectorMessemger.hh                      *
//* ****************************************************
//  $Id: muDetectorMessenger.hh Feb 01, 2019 15:25:08Z vega $
#ifndef muDetectorMessenger_H
#define muDetectorMessenger_H 1

#include "G4UImessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

#include "globals.hh"

class muDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

class muDetectorMessenger : public G4UImessenger{
public:
  muDetectorMessenger(muDetectorConstruction*);
  ~muDetectorMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  muDetectorConstruction* fDetector;
  G4UIdirectory* fMuonDir;
  G4UIdirectory* fMuonDetectorDir;
  G4UIdirectory* fMuonMagnetDir;
  G4UIcmdWithAString* fMatterCmd;
  G4UIcmdWithADoubleAndUnit* fSizeCmd;
  G4UIcmdWithADoubleAndUnit* fMagFieldCmd;
  G4G4UIcmdWithoutParameter* fUpdateCmd;
};

#endif
