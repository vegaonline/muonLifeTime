//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muEventAction.hh                   *
//* ****************************************************
//  $Id: muEventAction.hh Mar 04, 2019 12:25:08Z vega $
#ifndef muEventAction_H
#define muEventAction_H 1

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
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4THitsMap.hh"
#include "G4RunManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4Run.hh"
#include "G4ios.hh"
#include "globals.hh"

#include "muAnalysis.hh"

class muEventAction : public G4UserEventAction{
public:
  muEventAction();
  virtual ~muEventAction();

public:
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);

private: G4int fEventID;
};

#endif
