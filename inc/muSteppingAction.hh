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
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4PhysicalConstants.hh"
#include "G4UnitsTable.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4VProcess.hh"
#include "G4ProcessVector.hh"
#include "G4ProcessManager.hh"
#include "G4ios.hh"
#include "globals.hh"

#include "muSteppingAction.hh"
#include "muRunAction.hh"
#include "muEventAction.hh"
#include "muDetectorConstruction.hh"

class G4Navigator;
class G4Track;
class muEventAction;
class muDetectorConstruction;
class muRunAction;
class muSteppingAction;

class muSteppingAction : public G4UserSteppingAction{
public:
  //muSteppingAction(muRunAction*, muHistoManager*);
  muSteppingAction(muEventAction*);
  virtual ~muSteppingAction();

public:
  virtual void UserSteppingAction(const G4Step*);
private:
  muRunAction* fRunAction;
  muEventAction* fEventAction;
  //muHistoManager* muHisto;
};

#endif
