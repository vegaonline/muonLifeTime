//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muActionInitialization.hh                   *
//* ****************************************************
//  $Id: muActionInitialization.hh Mar 03, 2019 17:25:08Z vega $
#ifndef muActionInitialization_H
#define muActionInitialization_H 1

#include "G4VUserActionInitialization.hh"
#include "muDetectorConstruction.hh"
#include "muPrimaryGeneratorAction.hh"
#include "muRunAction.hh"
#include "muSteppingAction.hh"
#include "muStackingAction.hh"
#include "muEventAction.hh"

class muRunAction;
class muDetectorConstruction;
class muEventAction;

class muActionInitialization : public G4VUserActionInitialization{
public:
  muActionInitialization();
  virtual ~muActionInitialization();

public:
  virtual void BuildForMaster() const;
  virtual void Build() const;
private:
  muDetectorConstruction* fDetector;
  muRunAction* fMasterRunAction;

};

#endif
