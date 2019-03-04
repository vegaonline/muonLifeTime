#ifndef G4TARC_DETECTOR_MESSENGER_H
#define G4TARC_DETECTOR_MESSENGER_H

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4TARCDetectorConstruction.hh"
#include "G4TARCPhysicsList.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
//#include "G4TARCHistoManager.hh"

class G4TARCDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;

class G4TARCDetectorMessenger: public G4UImessenger {
public:
  G4TARCDetectorMessenger(G4TARCDetectorConstruction*);
  virtual ~G4TARCDetectorMessenger();
  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  G4TARCDetectorConstruction* fDetector;
  G4TARCPhysicsList*          fPhysList;
  G4UIdirectory*              fTestDir;
  G4UIdirectory*              fPhysDir;
  G4UIcmdWithABool*           fThermalCmd;
  G4UIcmdWithAString*         fPhysListCmd;
  G4UIcmdWithoutParameter*    fListCmd;

};
#endif
