#ifndef G4TARCSTACKINGMESSENGER_H
#define G4TARCSTACKINGMESSENGER_H

#include "G4UImessenger.hh"
#include "globals.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4TARCStackingAction.hh"

class G4TARCStackingAction;
class G4UIcmdWithABool;
class G4UIcmdWithAString;

class G4TARCStackingMessenger : public G4UImessenger {
public:
  G4TARCStackingMessenger(G4TARCStackingAction*);
  virtual ~G4TARCStackingMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  G4TARCStackingAction*   fStackingAction;

  G4UIcmdWithABool*       fKillCmd;
  G4UIcmdWithAString*     fKCmd;
};


#endif
