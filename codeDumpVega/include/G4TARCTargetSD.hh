#ifndef G4TARCTARGETSD_HH
#define G4TARCTARGETSD_HH

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

//#include "G4TARCHistoManager.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4Track.hh"

class G4Step;
class G4TouchableHistory;
class G4HCofThisEvent;
//class G4TARCHistoManager;

class G4TARCTargetSD : public G4VSensitiveDetector {
public:
  G4TARCTargetSD(const G4String&);
  virtual ~G4TARCTargetSD(){};

public:
  virtual void Initialize(G4HCofThisEvent*){};
  virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

private:
  //G4TARCHistoManager* fHisto;
};

#endif
