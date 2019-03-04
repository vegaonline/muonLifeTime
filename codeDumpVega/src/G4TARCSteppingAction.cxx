#include "G4TARCSteppingAction.hh"

G4TARCSteppingAction::G4TARCSteppingAction(G4TARCEventAction* anEvent)
: G4UserSteppingAction(), fEventAction(anEvent)// , fHisto(0)
{

  //fEventAction = anEvent;
  //fHisto = G4TARCHistoManager::GetPointer();

  fRefShellThickness = 2.0 * mm;
  fRefShellOuterRad = 456.0 * mm;
  fRefShellInnerRad = fRefShellOuterRad - fRefShellThickness;
  //fMaxOuterRadiusofShell = 1500.0 * mm;
  //vi ../RoofMinInnerRadiusofShell = 10.0 * mm;
  fEnergy0 = 0.0;
  fNumGen = 0;
  fShellNumber = (G4int)((fMaxOuterRadiusofShell - fMinInnerRadiusofShell) / fShellThickness + 0.5);
  fRefShellNumber = fRadiusReference.size();

  for (int i = 0; i < fRefShellNumber; i++) {
    G4double tmp = fRadiusReference[i];
    fOuterRadiusofShell.push_back(tmp);
    tmp -=fRefShellThickness;
    fInnerRadiusofShell.push_back(tmp);
  }
}

void G4TARCSteppingAction::UserSteppingAction(const G4Step* myStep){
  //fHisto->ProcessStepping(myStep);
  ProcessStepping(myStep);
}


// User Stepping Action
void G4TARCSteppingAction::ProcessStepping(const G4Step* myStep){
  G4Track* myTrack = myStep->GetTrack();
  G4int StepNo = myTrack->GetCurrentStepNumber();
  G4double fParticleEnergy = myStep->GetPreStepPoint()->GetKineticEnergy();
  G4double fParticleTime   = myStep->GetPreStepPoint()->GetGlobalTime();
  G4double fParticleMomentum = myStep->GetPreStepPoint()->GetMomentum().mag();
  //G4double fZMomentum = myStep->GetPreStepPoint()->GetMomentum().z();
  G4double angle = myStep->GetPreStepPoint()->GetMomentum().angle(myStep->GetPreStepPoint()->GetPosition());
  G4double cosAngle = std::abs(cos(angle));
  G4int thisTrackID = myTrack->GetTrackID();
  G4int parentTrackID = myTrack->GetParentID();

  G4ParticleDefinition* fParticleType = myTrack->GetDefinition();
  G4String fParticleName = fParticleType->GetParticleName();
  G4double primEnergy = 0.0;

  if (StepNo == 1 && thisTrackID != 1)  fEventAction->analysePS(fParticleEnergy, fParticleName, fParticleMomentum);      //  , fParticleTime, fParticleMomentum, zMomentum);

  if (StepNo == 1){
    if (thisTrackID == 1){
      fParentEnergy.clear();
      fParentParticle.clear();
      fParentParticleID.clear();
      fNumGen = 0;
    }
    if ( fParticleName == "neutron"){
      fEnergy0 = fParticleEnergy;
      fTime0 = fParticleTime;
      fEventAction->analysePS(fEnergy0, fParticleName, fParticleMomentum);      //  , fParticleTime, fParticleMomentum, zMomentum);
      flag = true;
    }
  }

  fParentEnergy[thisTrackID] = fParticleEnergy;
  fParentParticle[thisTrackID] = fParticleName;
  fParentParticleID[thisTrackID] = parentTrackID;

  G4bool reduced_tally = false;

  if (thisTrackID == 1 && StepNo == 1) primEnergy = fParticleEnergy;
  if (thisTrackID != 1 && StepNo == 1) {
    G4int tempID = thisTrackID;
    fNumGen = 1;
    while(fParentParticleID[tempID] != 1){
      tempID = fParentParticleID[tempID];
      ++fNumGen;
    }
    if (fParentParticle[parentTrackID] == "neutron" && fParticleName == "neutron"){
      reduced_tally = true;
      fParentParticle.erase(parentTrackID);
    }

    fEventAction->analyseSecondaries (fParticleEnergy, fParticleName, fParticleTime, fParticleMomentum, parentTrackID, primEnergy,
      fParentEnergy[parentTrackID], fParentParticle[parentTrackID], reduced_tally, fNumGen);
  }

  if (fParticleName == "neutron"){
    fEventAction->NeutronEnergyTime(fParticleEnergy, fParticleTime, fEnergy0);
  } else {
    if (fParticleName == "Pb207" || fParticleName == "Pb208")  fEventAction->otherEnergyTime(fParticleEnergy, fParticleTime, fEnergy0);
  }

  if (thisTrackID > 1){
    if (myTrack->GetDynamicParticle()->GetParticleDefinition()->GetParticleName() == "neutron"){
      if ( (myTrack->GetLogicalVolumeAtVertex()->GetName() != "sampleSphere_log") || (myTrack->GetLogicalVolumeAtVertex()->GetName() != "sampleSphere_log")) {
          fEventAction->ExtraNeutronProcs("init");
      }
    }
  }

  G4double radiusPre = myStep->GetPreStepPoint()->GetPosition().mag();
  G4double radiusPost = myStep->GetPostStepPoint()->GetPosition().mag();
  //G4double zPos = myStep->GetPreStepPoint()->GetPosition().z();
  G4double StepLength = myStep->GetStepLength();
  G4String vol = myStep->GetTrack()->GetVolume()->GetName();

  G4TouchableHistory* thePreTouchable  = (G4TouchableHistory*) (myStep->GetPreStepPoint()->GetTouchable());
  G4TouchableHistory* thePostTouchable = (G4TouchableHistory*) (myStep->GetPostStepPoint()->GetTouchable());
  G4String PreVol="";
  G4String PostVol="";

  if (fParticleName == "neutron"){
    G4Material* matTest = 0;
    const G4Element* eleTest = 0;
    G4VPhysicalVolume* pvol = 0;
    G4LogicalVolume* lvol = 0;
    G4String lvName = "";
    const G4VTouchable* touchabletest = myTrack->GetTouchable();
    if (touchabletest) pvol = touchabletest->GetVolume();
    if (pvol) lvol = pvol->GetLogicalVolume();
    if (lvol) lvName = lvol->GetName();    // sampleSphere_log  sampleSphere_log2
    if (lvName == "sampleSphere_log" || lvName == "sampleSphere_log2") matTest = lvol->GetMaterial();
    if (matTest){
      //G4cout << matTest->GetNumberOfElements() << G4endl;
      eleTest = matTest->GetElement(0);
      //G4cout << lvName << "    " << eleTest->GetName() << G4endl;
    }
    //DumpCS(fParticleType, eleTest, matTest);
  }

  
  if (fParticleName == "neutron"){
    PreVol = thePreTouchable->GetVolume()->GetName();
    //if (PreVol != "world_log_PV")  PostVol = thePostTouchable->GetVolume()->GetName();   // this statement is equivalent to following statement but following is better
    if (thePostTouchable->GetVolume())  PostVol = thePostTouchable->GetVolume()->GetName();

    if (myStep->GetTrack()->GetNextVolume()){
      if (PreVol == "lab_phys" && PostVol == "world_log_PV"){
        fEventAction->exitingTally(true, fParticleEnergy);
      } else if (PostVol == "world_log_PV"){
        fEventAction->exitingTallyCheck(true);
      }
    }

    G4bool pre_inside = false;
    G4bool post_inside = false;

    if  ((radiusPre <= (fRefShellOuterRad + fMyTol)) && (radiusPre >= (fRefShellInnerRad - fMyTol)) ) pre_inside = true;
    if  ((radiusPost <= (fRefShellOuterRad + fMyTol)) && (radiusPost >= (fRefShellInnerRad - fMyTol))) post_inside = true;
    if (pre_inside && post_inside) fEventAction->analyseNeutronShellFluence(fParticleEnergy, StepLength);

    for (G4int ishell = 0; ishell < fRefShellNumber; ishell++){
      G4bool pre_inside_radial = false;
      G4bool post_inside_radial = false;
      G4double radOut = fOuterRadiusofShell[ishell];
      G4double radIn  = fInnerRadiusofShell[ishell];
      if ((radiusPre <= (radOut + fMyRadTol)) && (radiusPre >= (radIn - fMyRadTol))) pre_inside_radial = true;
      if ((radiusPost <= (radOut + fMyRadTol)) && (radiusPost >= (radIn - fMyRadTol)) ) post_inside_radial = true;
      if (pre_inside_radial && post_inside_radial) fEventAction->analyseNeutronRadialFluence(fParticleEnergy, StepLength, ishell);  // fParticleTime, StepLength, ishell);
    }

    if (vol == "sample_phys" || vol == "sampleTube_phys" || vol == "sample_phys2"){
      //G4double radValue = fRefShellOuterRad;
      fEventAction->analyseNeutronFluence(fParticleEnergy, StepLength);  //fParticleTime,  thisTrackID, radValue, StepLength,  parentTrackID, primEnergy,  fParticleName);
    }


    //for (std::size_t ii = 0; ii < fFluxRadTables.size(); ++ii){
    for (G4int ii = 0; ii < fEventAction->fMaxRadCount; ++ii){
      G4double radValue = fEventAction->fExptRadiiTables[8][ii];   // fFluxRadTables[ii] / 10.0;
      if ( (radiusPre < radValue && radiusPost > radValue) ||(radiusPre > radValue && radiusPost < radValue)){
        //G4cout << ii << " FluxRad  " << radValue << "  RadPre " << radiusPre <<  " radPost " << radiusPost << G4endl;
        fEventAction->analyseNeutronFlux(fParticleEnergy, thisTrackID, radValue, cosAngle, fParticleName);
      }
    }
  }
}



void G4TARCSteppingAction::DumpCS(const G4ParticleDefinition* fParticleType, const G4Element* fElement, const G4Material* matTest){
  std::ofstream csFile;
  csFile.open("csTc99.dat", std::ios::app);
  G4ProcessManager* procManager = fParticleType->GetProcessManager();
  G4ProcessVector* procLists = procManager->GetProcessList();
  for (G4int iProc = 0; iProc < procLists->size(); iProc++){
    //  G4cout << iProc << "    " << (*procLists)[iProc]->GetProcessType() << "    " << (*procLists)[iProc]->GetProcessName() << G4endl;
    if ((*procLists)[iProc]->GetProcessType() == 4) {  // IonInelastic:4  RadioActiveDecay : 6    msc : 2   coupledtransportation : 1
      G4double fParticleMass = fParticleType->GetPDGMass();
      G4double fParticleTestProcEnergy = 0.0;
      G4HadronicProcess* hadProc = static_cast<G4HadronicProcess*>((*procLists)[iProc]);
      for (G4double mom = 10.0 * keV, fParticleTestProcEnergy = std::sqrt(sqr(fParticleMass) + sqr(mom));
            fParticleTestProcEnergy < 1.0 * GeV;
            mom *= 10.0, fParticleTestProcEnergy = std::sqrt(sqr(fParticleMass) + sqr(mom))) {
              const G4DynamicParticle* fDynPart = new G4DynamicParticle(fParticleType, G4ThreeVector(1, 0, 0), fParticleTestProcEnergy - fParticleMass);

              //G4double cross_section = hadProc->GetMicroscopicCrossSection(fDynPart, fElement, matTest);   //, 293.0);
              //G4double cs_lhep = (G4HadronCrossSections::Instance())->GetInelasticCrossSection(fDynPart, fElement, 293.0);
              //csFile << mom / MeV << "      " << cross_section * 1000.0 / barn << G4endl;   // "    " << cs_lhep * 10000.0 / barn << G4endl;

              delete fDynPart;
      }
    }
  }
  csFile.close();
}
