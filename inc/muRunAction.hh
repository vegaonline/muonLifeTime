//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muRunAction.hh                              *
//* ****************************************************
//  $Id: muRunAction.hh Feb 14, 2019 15:52:08Z vega $
#ifndef muRunAction_H
#define muRunAction_H 1

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4ProductionCutsTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "globals.hh"

#include "muDetectorConstruction.hh"
#include "muPrimaryGeneratorAction.hh"

class muDetectorConstruction;
class muPrimaryGeneratorAction;
class G4Run;
class G4Material;

class muRunAction : public G4UserRunAction{
public:
  muRunAction(muDetectorConstruction*, muPrimaryGeneratorAction*);
  ~muRunAction();
public:
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

private:
  G4double ComputeTheory(G4String, G4int)    {};
  G4double GetEnergyCut(G4Material*, G4int)  {};

private:
  muDetectorConstruction* fDetector;
  muPrimaryGeneratorAction* fPrimary;

};

#endif
