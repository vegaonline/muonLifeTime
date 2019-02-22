//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muMagnetMessenger.hh                      *
//* ****************************************************
//  $Id: muMagnetMessenger.hh Feb 22, 2019 15:05:08Z vega $
#ifndef muMagnetMessenger_H
#define muMagnetMessenger_H 1

#include "G4UImessenger.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4SystemOfUnits.hh"

#include "muMagneticField.hh"


class muMagneticField;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithoutParameter;

class muMagnetMessenger : public G4UImessenger {
public:
  muMagnetMessenger(muMagneticField*);
  virtual ~muMagnetMessenger();
  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  muMagneticField* fMagSetup;

  G4UIdirectory* fFieldDir;

  G4UIcmdWithAnInteger*       fStepperCmd;
  G4UIcmdWithADoubleAndUnit*  fMagFieldYCmd;
  G4UIcmdWith3VectorAndUnit*  fMagFieldCmd;
  G4UIcmdWith3VectorAndUnit*  fLocalMagneticFieldCmd;
  G4UIcmdWithADoubleAndUnit*  fMinStepCmd;
  G4UIcmdWithoutParameter*    fUpdateCmd;

};


#endif
