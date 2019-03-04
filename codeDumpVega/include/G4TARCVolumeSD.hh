#ifndef G4TARCVOLUMESD_HH
#define G4TARCVOLUMESD_HH

//#include "G4TARCHistoManager.hh"

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "globals.hh"

class G4Step;
class G4TouchableHistory;
class G4HCofThisEvent;
//class G4TARCHistoManager;

class G4TARCVolumeSD : public G4VSensitiveDetector{
public:
  G4TARCVolumeSD(const G4String&);
  virtual ~G4TARCVolumeSD(){};

public:
  virtual void Initialize(G4HCofThisEvent*){};
  virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  virtual void EndOfEvent(G4HCofThisEvent*){};

private:
  //G4TARCHistoManager* fHisto;

};


#endif
