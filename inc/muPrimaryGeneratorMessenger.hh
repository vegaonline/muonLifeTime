//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muPrimaryGeneratorMessenger.hh                 *
//* ****************************************************
//  $Id: muPrimaryGeneratorMessenger.hh Feb 26, 2019 17:25:08Z vega $
#ifndef muPrimaryGeneratorMessenger_H
#define muPrimaryGeneratormessenger_H 1

#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4SystemOfUnits.hh"
#include "muPrimaryGeneratorAction.hh"

class muPrimaryGeneratorAction;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

class muPrimaryGeneratorMessenger : public G4UImessenger {
public:
  muPrimaryGeneratorMessenger(muPrimaryGeneratorAction*);
  virtual ~muPrimaryGeneratorMessenger();
  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  muPrimaryGeneratorAction* fAction;
};

#endif
