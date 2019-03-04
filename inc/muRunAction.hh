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
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4ProductionCutsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4UserRunAction.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "Randomize.hh"

#include "globals.hh"

#include "muDetectorConstruction.hh"
#include "muPrimaryGeneratorAction.hh"
#include "muAnalysis.hh"
#include "muRun.hh"
//#include "muHistoManager.hh"
#include <fstream>
#include <iomanip>


class G4Run;
class G4Material;

class muRunAction : public G4UserRunAction{
public:
  //muRunAction(muDetectorConstruction*, muPrimaryGeneratorAction*, muHistoManager*);
  muRunAction();
  virtual ~muRunAction();

public:
  virtual G4Run* GenerateRun();
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

private:
  G4double ComputeTheory(G4String, G4int) ;
  G4double GetEnergyCut(G4Material*, G4int);
  void BookHistogram();

  template <typename T>  void Check10s(T, T&, G4String&);

private:
  G4int fEventNum;
  G4String  fAnalysisFileName = "Muon_output";
  G4bool fHistoBooked = false;

  //muHistoManager* muHisto;
};

#endif
