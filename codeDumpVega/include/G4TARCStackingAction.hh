#ifndef G4TARCStackingAction_H
#define G4TARCStackingAction_H

#include "G4UserStackingAction.hh"
#include "G4VPhysicalVolume.hh"
#include "globals.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4VProcess.hh"
#include "G4ProcessVector.hh"
#include "G4ProcessManager.hh"
#include "G4HadronicProcess.hh"
#include "G4HadronInelasticProcess.hh"
#include "G4HadronCrossSections.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4TARCDetectorConstruction.hh"
//#include "G4TARCHistoManager.hh"
#include "G4TARCStackingMessenger.hh"
#include "G4TARCAnalysis.hh"
#include "G4TARCEventAction.hh"



//class G4TARCHistoManager;
class G4TARCStackingMessenger;
class G4Track;
class G4ParticleDefinition;
class G4Navigator;
class G4Element;


class G4TARCStackingAction : public G4UserStackingAction {
public:
  G4TARCStackingAction(G4TARCEventAction*);
  virtual ~G4TARCStackingAction();

  void SetKillStatus(G4bool);
  void SetKill(const G4String&);
  inline void SetKillGammasFlag(G4bool val)  { fKillGammas = val; }
  virtual G4ClassificationOfNewTrack ClassfyNewTrack(const G4Track*);
  virtual void PrepareNewEvent() {};


private:
  //G4TARCHistoManager*         fHistoManager;
  G4TARCStackingMessenger*    fStackMessenger;
  G4bool                      fKillSecondary;
  G4bool                      fKillGammas;
  G4int                       fNumber_newTracks;
  G4int                       fNeutron;
  G4int                       fProton;
  G4int                       fDeuteron;
  G4int                       fOther;
  G4int                 partType;

  G4Navigator*                fNavigator;
  G4TARCEventAction*          fEventAction;

  const G4ParticleDefinition* fParticle;
};


#endif
