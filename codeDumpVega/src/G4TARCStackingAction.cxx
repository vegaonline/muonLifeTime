#include "G4TARCStackingAction.hh"

G4TARCStackingAction::G4TARCStackingAction(G4TARCEventAction* thisEvent)
: G4UserStackingAction(),  fStackMessenger(0),
  fEventAction(thisEvent), fParticle(0) {
  fStackMessenger   = new G4TARCStackingMessenger(this);
  //fHistoManager     = G4TARCHistoManager::GetPointer();
  fKillSecondary    = false;
  fKillGammas       = false;
  fNumber_newTracks = 0;
  fNeutron          = 0;
  fProton           = 0;
  fDeuteron         = 0;
  fParticle         = 0;
  fNavigator        = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
}

G4TARCStackingAction::~G4TARCStackingAction() {
  delete fStackMessenger;
}


G4ClassificationOfNewTrack G4TARCStackingAction::ClassfyNewTrack(const G4Track* myTrack) {

  G4cout << " In StackingAction.........." << G4endl;

  fNumber_newTracks++;

  G4ParticleDefinition* pd = myTrack->GetDefinition();
  G4String name = pd->GetParticleName();
  G4double ke = myTrack->GetKineticEnergy() / eV;
  G4cout << ke << G4endl;
  G4int thisTrackID = myTrack->GetTrackID();
  
  if (myTrack->GetParentID() == 0) return fUrgent;  // keep primary particle


  if (pd == G4Neutron::NeutronDefinition() && thisTrackID != 1) {
    partType = 1;
    ++fNeutron;
    fEventAction->Adding2NeutronStack();
  } else if (pd == G4Proton::ProtonDefinition() && thisTrackID != 1){
    partType = 2;
    ++fProton;
  } else if (pd == G4Deuteron::DeuteronDefinition() && thisTrackID != 1) {
    partType = 3;
  } else if (pd == G4Triton::TritonDefinition() && thisTrackID != 1) {
    partType = 4;
  } else if (pd == G4Gamma::GammaDefinition() && thisTrackID != 1) {
    partType = 5;
  }else if (pd == G4Electron::ElectronDefinition() && thisTrackID != 1) {
    partType = 6;
  } else if (pd == G4Positron::PositronDefinition() && thisTrackID != 1) {
    partType = 7;
  } else if (pd->GetAtomicMass() == 208 && pd->GetAtomicNumber() == 82 && thisTrackID != 1){
    partType = 8;
    name = "Pb-208";
  } else if (pd->GetAtomicMass() == 207 && pd->GetAtomicNumber() == 82 && thisTrackID != 1){
    partType = 8;
    name = "Pb-207";
  } else if (pd->GetAtomicMass() == 206 && pd->GetAtomicNumber() == 82 && thisTrackID != 1){
    partType = 8;
    name = "Pb-206";
  }else if (pd->GetAtomicMass() == 204 && pd->GetAtomicNumber() == 82 && thisTrackID != 1){
    partType = 8;
    name = "Pb-204";
  } else if (pd->GetAtomicMass() == 2 && pd->GetAtomicNumber() == 1 && thisTrackID != 1){
    partType = 9;
    ++fDeuteron;
    name = "deuteron";
  }else if (pd->GetAtomicMass() == 3 && pd->GetAtomicNumber() == 1 && thisTrackID != 1){
    partType = 10;
    name = "triton";
  } else {
    if (pd->GetAtomicMass() > 41)
     partType = 11;
    else
      partType = 12;
    ++fOther;
    name = "other";
  }


  G4cout << " In StackingAction" << G4endl;

  //if (pd == G4Neutron::NeutronDefinition() && myTrack->GetTrackID() != 1)

    // For Primary Track
    //  if (myTrack->GetTrackID() == 1) {
        //fNevt++;
/*
        if (ke > 0){
          G4double enerMean = GetGPSEnergy() / eV;
          enerMean = (0.5 * (ke + enerMean));
          //SetGPSEnergyIN(enerMean);  // already set in primaryGenerator from GPS
        }
        * /
        //fPrimaryKineticEnergy = ke;
        fPrimaryDef = pd;
        G4ThreeVector dir = myTrack->GetMomentumDirection();


        if (fVerbose > 1) G4cout << "### Primary "   << name << " KineticE(eV) = " << ke
                                 << "; Mass(MeV) = " << pd->GetPDGMass() / MeV
                                 << "; Pos(mm) = "   << myTrack->GetPosition() / mm
                                 << "; Dir = "       << myTrack->GetMomentumDirection()
                                 << G4endl;
      } else {
    // For Secondary tracks.
        if (fVerbose > 1) G4cout << "=== Secondary " << name << " KineticE(eV) = " << ke / eV
                                 << "; Mass(MeV) = " << pd->GetPDGMass() / MeV
                                 << "; Pos(mm) = "   << myTrack->GetPosition() / mm
                                 << "; Dir = "       << myTrack->GetMomentumDirection()
                                 << G4endl;
        //    ke = std::log10(ke);

        if (pd == G4Gamma::GammaDefinition()){
          fNgam++;
          //const G4VProcess* procG = myTrack->GetCreatorProcess();         //  check
        }else if (pd == G4Electron::ElectronDefinition()){
          fNelec++;
        }else if (pd == G4Positron::PositronDefinition()) {
          fNposit++;
        } else if (pd == G4Proton::ProtonDefinition()){
          fNproton++;
        } else if (pd == G4Neutron::NeutronDefinition()){//&& TKin < fTcut){  // <----- CHECK
          fNneutron++;
          fNeutronStack++;
          // fEventAction->AddNeutronStack();
          //G4cout << ke << G4endl;
          / *
          fAnalysisManager->FillNtupleDColumn(15, 0, ke);   //log10(ke));
          fAnalysisManager->AddNtupleRow(15);
          * /
        } else if (pd == G4AntiProton::AntiProtonDefinition()){
          fNaproton++;
        } else if ( pd == G4PionPlus::PionPlusDefinition() ) {
          fNpions++;
        } else if ( pd == G4PionMinus::PionMinusDefinition()) {
          fNpions++;
        } else if ( pd == G4PionZero::PionZeroDefinition()) {
          fNpi0++;
        } else if ( pd == G4KaonPlus::KaonPlusDefinition() ||  pd == G4KaonMinus::KaonMinusDefinition()) {
          fNkaons++;
        } else if ( pd == G4KaonZeroShort::KaonZeroShortDefinition() ||  pd == G4KaonZeroLong::KaonZeroLongDefinition()) {
          fNkaons++;
        } else if (pd == G4Deuteron::DeuteronDefinition() || pd == G4Triton::TritonDefinition()){
          fNdeut++;
        } else if (pd == G4He3::He3() || pd == G4Alpha::Alpha()) {
          fNalpha++;
        } else if (pd->GetParticleType() == "nucleus"){
          fNions++;
        } else if (pd == G4MuonPlus::MuonPlusDefinition() || pd == G4MuonMinus::MuonMinusDefinition()){
          fNmuons++;
        }
      }

    }

  const G4ParticleDefinition* part = myTrack->GetDefinition();

  if (fHistoManager->GetVerbose() > 1) {
    G4cout << " Track #"                      << myTrack->GetTrackID()
           << " of "                          << part->GetParticleName()
           << " E (MeV)= "                    << myTrack->GetKineticEnergy() / MeV
           << " produced by Track ID= "       << myTrack->GetParentID()
           << G4endl;
  }
  if (myTrack->GetTrackID() > 1) {
    if (fKillSecondary || part == fParticle)
      status = fKill;
  }
  */
  G4ClassificationOfNewTrack status = fWaiting;     //fUrgent;
  return status;
}

void G4TARCStackingAction::SetKillStatus( G4bool value ) {
  fKillSecondary = value;
}

void G4TARCStackingAction::SetKill( const G4String& name ) {
  fParticle = G4ParticleTable::GetParticleTable()->FindParticle(name);
}

