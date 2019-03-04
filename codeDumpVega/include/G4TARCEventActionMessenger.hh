#ifndef G4TARC_EVENTACTION_MESSENGER_H
#define G4TARC_EVENTACTION_MESSENGER_H

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4TARCEventAction.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"

class G4TARCEventAction;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

class G4TARCEventActionMessenger : public G4UImessenger{
public:
  G4TARCEventActionMessenger(G4TARCEventAction*);
  virtual ~G4TARCEventActionMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  G4TARCEventAction*     fEventAction;
  G4UIcmdWithAnInteger*  fPrintCmd;
  G4UIcmdWithAnInteger*  fCmd;
};

#endif
