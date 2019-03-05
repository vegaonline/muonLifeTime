//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muStackingAction.cc                              *
//* ****************************************************
//  $Id: muStackingAction.cc Feb 28, 2019 21:47:08Z vega $

#include "muStackingAction.hh"

//muStackingAction::muStackingAction()
muStackingAction::muStackingAction(muEventAction* thisEvent)
  : G4UserStackingAction(), fEventAction(thisEvent), fParticle(0), fWillPrint(false){
    fParticle = 0;
    fNumber_NewTracks = 0;
    fNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
}

muStackingAction::~muStackingAction(){

}

G4ClassificationOfNewTrack muStackingAction::ClassifyNewTrack(const G4Track* myTrack){
  G4ClassificationOfNewTrack classification = fWaiting;
  G4VPhysicalVolume* pVol = 0;
  G4LogicalVolume* lVol = 0;
  G4String lvName = "";

  fNumber_NewTracks++;
  fWillPrint = false;

  G4ParticleDefinition* pd = myTrack->GetDefinition();
  G4String name = pd->GetParticleName();
  G4double ke = myTrack->GetKineticEnergy() / eV;
  //G4int thisTrackID = myTrack->GetTrackID();
  const G4VTouchable* touchableTrack = myTrack->GetTouchable();
  if (touchableTrack ) pVol = touchableTrack->GetVolume();
  if (pVol) lVol = pVol->GetLogicalVolume();
  if (lVol) lvName = lVol->GetName();

  if (myTrack->GetParentID() == 0)  return fUrgent;

  if (pd == G4MuonPlus::MuonPlusDefinition()){
    fWillPrint = true;
    ++fMuonP;
  } else if (pd == G4MuonMinus::MuonMinusDefinition()) {
    ++fMuonM;
  } else if (pd == G4NeutrinoE::NeutrinoEDefinition()) {
    fWillPrint = true;
    ++fNeutrinoE;
  } else if (pd == G4NeutrinoMu::NeutrinoMuDefinition()) {
    ++fNeutrinoMu;
  } else if (pd == G4AntiNeutrinoE::AntiNeutrinoEDefinition()) {
    ++fAntiNeutrinoE;
  } else if (pd == G4AntiNeutrinoMu::AntiNeutrinoMuDefinition()) {
    fWillPrint = true;
    ++fAntiNeutrinoMu;
  } else if (pd == G4Electron::ElectronDefinition()) {
    ++fElectron;
  } else if (pd == G4Positron::PositronDefinition()) {
    ++fPositron;
  }

  if (fWillPrint && lVol){
      G4cout << name << "  Energy " << (ke / 1.0e6) << " MeV at " << lvName << G4endl;
  }


  classification = fWaiting;
  return classification;
}
