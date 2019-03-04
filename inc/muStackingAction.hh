//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muStackingAction.hh                 *
//* ****************************************************
//  $Id: muStackingAction.hh Feb 28, 2019 21:45:08Z vega $

#ifndef muStackingAction_H
#define muStackingAction_H 1

#include "G4UserStackingAction.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4Navigator.hh"
#include "G4VProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4NeutrinoE.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4AntiNeutrinoMu.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "globals.hh"

#include "muDetectorConstruction.hh"
#include "muAnalysis.hh"
#include "muEventAction.hh"

class G4Track;
class G4Navigator;
class G4MuonPlus;
class G4MuonMinus;
class G4NeutrinoE;
class G4NeutrinoMu;
class G4AntiNeutrinoE;
class G4AntiNeutrinoMu;
class G4Electron;
class G4Positron;

class muStackingAction : public G4UserStackingAction{
public:
  //muStackingAction();
  muStackingAction(muEventAction*);
  virtual ~muStackingAction();

public:
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);

private:
  const G4ParticleDefinition* fParticle;
  G4int fNumber_NewTracks;
  G4Navigator* fNavigator;
  muEventAction* fEventAction;

  G4int fNeutrinoE;
  G4int fNeutrinoMu;
  G4int fAntiNeutrinoE;
  G4int fAntiNeutrinoMu;
  G4int fMuonP;
  G4int fMuonM;
  G4int fElectron;
  G4int fPositron;
  G4bool fWillPrint;
};

#endif
