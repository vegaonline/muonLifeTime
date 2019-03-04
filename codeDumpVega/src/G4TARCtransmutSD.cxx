#include "G4TARCtransmutSD.hh"

G4TARCtransmutSD::G4TARCtransmutSD(const G4String& name)
: G4VSensitiveDetector(name){
  // fHisto = G4TARCHistoManager::GetPointer();
}

G4bool G4TARCtransmutSD::ProcessHits(G4Step* myStep, G4TouchableHistory*){
  const G4Track* myTrack = myStep->GetTrack();
  // fHisto->AddLeakingParticle(myTrack);
  // fHisto->AddTargetStep(myStep);
  //fHisto->ScoreNewTrack(myTrack);
  if (myTrack->GetTrackID() > 1){
    // fHisto->AddEnergyTime(myTrack);  // , myStep);
    // fHisto->AddEnergyTimeHole(myTrack); // , myStep);
    // fHisto->NeutFinalState(myTrack);   //, myStep);
  } else {
    // fHisto->GunParticleDistribution(myStep);
  }
  return true;
}
