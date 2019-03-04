//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muRun.hh                              *
//* ****************************************************
//  $Id: muRun.hh Feb 14, 2019 15:52:08Z vega $
#ifndef muRun_H
#define muRun_H 1

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4THitsMap.hh"
#include <vector>
#include <regex>


class G4Run;
class G4Event;

class muRun : public G4Run{
public:
  muRun();
  virtual ~muRun();

public:
  virtual void RecordEvent(const G4Event*);
  virtual void Merge(const G4Run*) {};
  G4int fNEvt;
};

#endif
