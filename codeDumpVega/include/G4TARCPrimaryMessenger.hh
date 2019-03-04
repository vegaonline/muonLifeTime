#ifndef G4TARCPRIMARY_MESSENGER_H
#define G4TARCPRIMARY_MESSENGER_H

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "globals.hh"
#include <sstream>
//#include "G4TARCHistoManager.hh"
#include "G4TARCPrimaryGeneratorAction.hh"
//#include "G4TARCHisto.hh"

class G4TARCPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;

class G4TARCPrimaryMessenger : public G4UImessenger {
public:
  G4TARCPrimaryMessenger(G4TARCPrimaryGeneratorAction*);
  virtual ~G4TARCPrimaryMessenger();
  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  G4TARCPrimaryGeneratorAction* fGUN;
  G4UIdirectory*                fGunDir;
  G4UIcmdWithABool*             fBeamCmd;
  G4UIcmdWithADoubleAndUnit*    fCurrentCmd;
};

#endif
