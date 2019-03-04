#include "G4TARCTargetSD.hh"

G4TARCTargetSD::G4TARCTargetSD(const G4String& name)
: G4VSensitiveDetector(name) {
  // fHisto = G4TARCHistoManager::GetPointer();
}

G4bool G4TARCTargetSD::ProcessHits(G4Step* myStep, G4TouchableHistory*){
  const G4Track* myTrack = myStep->GetTrack();
  // fHisto->AddTargetStep(myStep);
  //fHisto->ScoreNewTrack(myTrack);
  if (myTrack->GetTrackID() > 1){
    // fHisto->AddEnergyTime(myTrack);  // , myStep);
  } else {
    // fHisto->GunParticleDistribution(myStep);
  }
  return true;
}
