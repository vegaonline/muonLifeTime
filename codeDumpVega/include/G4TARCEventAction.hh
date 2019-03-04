/*****************************************************************
 * @file    G4TARCEventAction.hh
 * @author  Abhijit Bhattacharyya
 * @brief   data members holds energy deposit and track length
 ****************************************************************/
#ifndef G4TARC_EVENTACTION_H
#define G4TARC_EVENTACTION_H

#include <vector>
#include <fstream>
#include <iomanip>

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4UImanager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4THitsMap.hh"
#include "G4RunManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4Run.hh"

#include "G4ios.hh"
#include "globals.hh"

#include "G4TARCRun.hh"
#include "G4TARCEventActionMessenger.hh"
#include "G4TARCAnalysis.hh"
//#include "G4TARCHistoManager.hh"

/*
class G4Event;
class G4UImanager;
class G4TARCEventActionMessenger;
//class G4TARCHistoManager;
*/
class G4TARCEventAction : public G4UserEventAction {
public:
  G4TARCEventAction();
  virtual ~G4TARCEventAction();

  virtual void BeginOfEventAction( const G4Event* );
  virtual void EndOfEventAction( const G4Event* );

  void SetPrintModulo ( G4int val) {fPrintModulo = val;}
  //inline void AddEventToDebug ( G4int val){fSelectedEvents.push_back(val);  ++fSelected;};

  void analyseSecondaries(G4double, G4String, G4double, G4double, G4int, G4double, G4double, G4String, G4bool, G4int);
  void NeutronEnergyTime(G4double, G4double, G4double);
  void otherEnergyTime(G4double, G4double, G4double);
  void exitingTally(G4bool, G4double);
  void exitingTallyCheck(G4bool);
  inline void Adding2NeutronStack() {++fNeutronStack;}

  void analyseNeutronFlux(G4double, G4int, G4double, G4double,  G4String);
  void analyseNeutronShellFluence(G4double, G4double);
  void analyseNeutronRadialFluence(G4double, G4double, G4int); //G4double, G4int);
  void analysePS(G4double, G4String, G4double); // , G4double, G4double);
  void analyseNeutronFluence(G4double, G4double); //G4int, G4double, G4double,  G4int, G4double, G4String&);
  void ExtraNeutronProcs(const G4String&);

public:
  std::vector< std::vector<G4double> >   fExptRadiiTables;
  G4int fMaxRadCount;

private:
  G4int                                                     fEventID;
  G4int                                                     fPrintModulo;
  G4int                                                     fNeutronStack;
  G4bool                                                  fPrimarySet;
};



#endif
