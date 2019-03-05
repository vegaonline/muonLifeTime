//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muRunAction.cc                              *
//* ****************************************************
//  $Id: muRunAction.cc Feb 02, 2019 11:47:08Z vega $

#include "muRunAction.hh"

// muRunAction::muRunAction(muDetectorConstruction* det, muPrimaryGeneratorAction* prim, muHistoManager* muHistM)
muRunAction::muRunAction()
 : G4UserRunAction(), fHistoBooked(false) {// ,fEventNum(0),  muHisto(muHistM) {
   G4AnalysisManager* fAnalysisMan = G4AnalysisManager::Instance();
   if (!fHistoBooked) BookHistogram();
   G4cout << fAnalysisMan->GetType() << " data is being stored." << G4endl;
}

muRunAction::~muRunAction(){
  delete G4AnalysisManager::Instance();
}

G4Run* muRunAction::GenerateRun() {
  return (new muRun);
}

void muRunAction::BeginOfRunAction(const G4Run* aRun) {
  G4cout << "### Run " << aRun->GetRunID() << " start. " << G4endl;

  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  auto fAnalysisMan = G4AnalysisManager::Instance();
  fAnalysisMan->OpenFile(fAnalysisFileName);
  //  CLHEP::HepRandom::showEngineStatus();  // working but not needed now
  //muHisto->Book();
}

void muRunAction::EndOfRunAction(const G4Run* aRun){
  //G4Material* material;
  //G4double density = 0.0, energy = 0.0, lengthY = 0.0;
  const muRun* thisRun = static_cast<const muRun*>(aRun);

  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;
/*
  material = fDetector->GetDetectorMaterial();
  lengthY = fDetector->GetDetectorThickness();
  density = material->GetDensity();
  G4String Particle = fPrimary->GetParticleGun()->GetParticleDefinition()->GetParticleName();
  energy = fPrimary->GetParticleGun()->GetParticleEnergy();
  G4cout << "\n The run consists of "  << NbOfEvents << " "<< Particle << " of "
         << energy / MeV   << " MeV "  << " through "
         << lengthY / mm   << " mm "   << " of "
         << material->GetName()           << " )" << G4endl;
  //muHisto->Save();
  */
  auto fAnalysisMan = G4AnalysisManager::Instance();
  if (fAnalysisMan->GetH1(1)) G4cout << "Edep_D0 " <<G4BestUnit(fAnalysisMan->GetH1(0)->mean(), "Energy");
  fAnalysisMan->Write();
  fAnalysisMan->CloseFile();
  G4cout <<"End of RunAction." << G4endl;
}

void muRunAction::BookHistogram(){
  delete G4AnalysisManager::Instance();
  G4AnalysisManager* fAnalysisMan = G4AnalysisManager::Instance();
  fAnalysisMan->SetNtupleMerging(true, 0, false);
  fAnalysisMan->SetVerboseLevel(1);
  fAnalysisMan->SetFileName(fAnalysisFileName);
  fHistoBooked = true;

  fAnalysisMan->CreateH1("E_D0", "Edep in Detector0", 100, 0.0, 5.0*GeV);
  fAnalysisMan->CreateH1("E_D1", "Edep in Detector1", 100, 0.0, 5.0*GeV);
  fAnalysisMan->CreateH1("L_D0", "Track in Detector0", 100, 0.0, 1.4*m);
  fAnalysisMan->CreateH1("L_D1", "Track in Detector1", 100, 0.0, 1.4*m);

  fAnalysisMan->CreateNtuple("Muons", "Edep and TrackL");
  fAnalysisMan->CreateNtupleDColumn("E_D0");
  fAnalysisMan->CreateNtupleDColumn("E_D1");
  fAnalysisMan->CreateNtupleDColumn("L_D0");
  fAnalysisMan->CreateNtupleDColumn("L_D1");
  fAnalysisMan->FinishNtuple();
}


template <typename T>
  void muRunAction::Check10s(T inVal, T &outVal, G4String &uniStr){
    outVal = 0;
    uniStr = "";
    G4int iresult = 0;

    if (inVal / 1000000000 >= 1){
      iresult = 9;  // unit = Giga
    } else {
      if (inVal / 1000000 >= 1){
        iresult = 6;   // Mega
      } else {
        if (inVal / 1000 >= 1) {
          iresult = 3;   // kilo
        } else {
          iresult = 1;
        }
      }
    }

    switch(iresult){
      case 9: outVal = inVal / 1.0e9; uniStr = " GeV"; break;
      case 6: outVal = inVal / 1.0e6; uniStr = " MeV"; break;
      case 3: outVal = inVal / 1.0e3; uniStr = " keV"; break;
     case 1: outVal = inVal; uniStr = " eV"; break;
    }
  }
