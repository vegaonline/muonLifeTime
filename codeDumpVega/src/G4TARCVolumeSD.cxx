#include "G4TARCVolumeSD.hh"

G4TARCVolumeSD::G4TARCVolumeSD(const G4String& name)
 : G4VSensitiveDetector(name){
   // fHisto = G4TARCHistoManager::GetPointer();
 }


G4bool G4TARCVolumeSD::ProcessHits(G4Step* myStep, G4TouchableHistory*){
  const G4Track* myTrack = myStep->GetTrack();

  // fHisto->AddLeakingParticle(myTrack);

  if (myTrack->GetTrackID() > 1) {
    //fHisto->AddNzero(myTrack);
    // fHisto->NeutFinalState(myTrack);   //, myStep);
    // fHisto->TargetProfile(myTrack);    //, myStep); (was originally here )
    //fHisto->AddEnergyTime(myTrack);  //, myStep);
  } else {
    // fHisto->AddNzero(myTrack);
    //fHisto->TargetProfile(myTrack);   //, myStep);
  }
  return true;
}
