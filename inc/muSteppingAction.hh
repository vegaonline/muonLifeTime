//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muSteppingAction.hh                 *
//* ****************************************************
//  $Id: muSteppingAction.hh Feb 28, 2019 21:45:08Z vega $

#ifndef muSteppingAction_H
#define muSteppingAction_H 1

#include "G4UserSteppingAction.hh"
#include "G4RunManager.hh"
#include "globals.hh"
#include "muSteppingAction.hh"
#include "muRunAction.hh"
#include "muSteppingAction.hh"

class muRunAction;
class muSteppingAction;

class muSteppingAction : public G4UserSteppingAction{
public:
  muSteppingAction(muRunAction*, muHistoManager*);
  ~muSteppingAction();

public:
  virtual void UserSteppingAction(const G4Step*);
private:
  muRunAction* fRunAction;
  muHistoManager* muHisto;
};

#endif
