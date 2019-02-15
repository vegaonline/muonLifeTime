//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muActionInitialization.hh                   *
//* ****************************************************
//  $Id: muActionInitialization.hh Feb 15, 2019 09:52:08Z vega $
#ifndef muActionInitialization_H
#define muActionInitialization_H 1

#include "G4VUserActionInitialization.hh"
#include "muDetectorConstruction.hh"
#include "muPrimaryGeneratorAction.hh"
#include "muRunAction.hh"

class muDetectorConstruction;
class muRunAction;

class muActionInitialization : public G4VUserActionInitialization{
public:
  muActionInitialization(muDetectorConstruction*);
  virtual ~muActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;

private:
  muDetectorConstruction* fDetector;
};

#endif
