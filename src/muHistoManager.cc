//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muHistoManager.cc                                 *
//* ****************************************************
//  $Id: muHistoManager.cc Mar 01, 2019 10:47:08Z vega $

#include "muHistoManager.hh"

muHistoManager::muHistoManager() {
  fileName[0] = "muonData";
  factoryOn = false;
  fNbHist = 0;

  for (G4int k = 0; k < kMaxHisto; k++){
    fHistId[k] = 0;
    fHistPt[k] = 0;
    fExist[k] = false;
    fUnit[k] = 1.0;
    fWidth[k] = 1.0;
    fAscii[k] = false;
  }
}

muHistoManager::~muHistoManager(){
  // delete fHistoMessenger;
}

void muHistoManager::Book(){
  if (fNbHist == 0) return; // if no histos, do nothing

  G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();
  analysisMan->SetVerboseLevel(0);
  G4String extension = analysisMan->GetFileType();
  fileName[1] = fileName[0] + "." + extension;
  G4bool fileOpen = analysisMan->OpenFile(fileName[0]);
  if (!fileOpen){
    G4cout << "\n---> HistoManager::book(): cannot open " << fileName[1] << G4endl;
    return;
  }

  analysisMan->SetFirstHistoId(1);
  for (G4int k = 0; k < kMaxHisto; k++){
    if (fExist[k]){
      fHistId[k] = analysisMan->CreateH1( fLabel[k], fTitle[k], fNbins[k], fVmin[k], fVmax[k]);
      fHistPt[k] = analysisMan->GetH1(fHistId[k]);
      factoryOn = true;
    }
  }
  if (factoryOn) G4cout << "\n----> Histogram file is opened in " << fileName[1] << G4endl;
}


void muHistoManager::Save(){
  if (factoryOn) {
    G4cout << "\n----> muHistoManager::save " << G4endl;
    G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();
    analysisMan->Write();
    analysisMan->CloseFile();
    SaveAscii();                    // Write fAscii file, if any
    G4cout << "\n----> Histograms are saved in " << fileName[1] << G4endl;
    //    delete G4AnalysisManager::Instance();
    factoryOn = false;
  }
}

void muHistoManager::FillHisto(G4int ih, G4double e, G4double weight){
  if (ih > kMaxHisto) {
    G4cout << "---> warning from muHistoManager::FillHisto() : histo " << ih
           << "does not fExist; e= " << e << " w= " << weight << G4endl;
    return;
  }

  if (fHistPt[ih]) fHistPt[ih]->fill(e/fUnit[ih], weight);
}

void muHistoManager::SetHisto(G4int ih, G4int nbins, G4double vmin, G4double vmax, const G4String& unit){
  if (ih > kMaxHisto) {
    G4cout << "---> warning from muHistoManager::SetHisto() : histo " << ih
           << "does not fExist" << G4endl;
    return;
  }

  const G4String id[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                 "10", "11", "12", "13", "14", "15"};
  const G4String title[] =
                { "dummy",                                           //0
                  "log10(Etransfert/Emu) muIonization",              //1
                  "log10(Etransfert/Emu) muPair",                    //2
                  "log10(Etransfert/Emu) muBrems",                   //3
                  "log10(Etransfert/Emu) muNuclear",                 //4
                  "log10(Etransfert/Emu) hIonization",               //5
                  "log10(Etransfert/Emu) hPair",                     //6
                  "log10(Etransfert/Emu) hBrems",                    //7
                  "dummy",                                           //8
                  "dummy",                                           //9
                  "dummy",                                           //10
                  "log10(Etransfert/Emu) muIonization",              //11
                  "log10(Etransfert/Emu) muPair",                    //12
                  "log10(Etransfert/Emu) muBrems",                   //13
                  "log10(Etransfert/Emu) muNuclear"                  //14
                 };
  G4String titl = title[ih];
  fUnit[ih] = 1.;

  if (unit != "none") {
    titl = title[ih] + " (" + unit + ")";
    fUnit[ih] = G4UnitDefinition::GetValueOf(unit);
  }

  fExist[ih] = true;
  fLabel[ih] = "h"+id[ih];
  fTitle[ih] = titl;
  fNbins[ih] = nbins;
  fVmin[ih]  = vmin;
  fVmax[ih]  = vmax;
  fWidth[ih] = fUnit[ih]*(vmax-vmin)/nbins;
  fNbHist++;

  G4cout << "----> SetHisto " << ih << ": " << titl << ";  "
         << nbins << " bins from "
         << vmin << " " << unit << " to " << vmax << " " << unit << G4endl;
}

void muHistoManager::Normalize(G4int ih, G4double fac){
  if (ih >= kMaxHisto) {
    G4cout << "---> warning from HistoManager::Normalize() : histo " << ih
           << "  fac= " << fac << G4endl;
    return;
  }
  if (fHistPt[ih]) fHistPt[ih]->scale(fac);
}

void muHistoManager::PrintHisto(G4int ih){
 if (ih < kMaxHisto) { fAscii[ih] = true; fAscii[0] = true; }
 else
    G4cout << "---> warning from muHistoManager::PrintHisto() : histo " << ih
           << "does not exist" << G4endl;
}

void muHistoManager::SaveAscii(){
 if (!fAscii[0]) return;

 G4String name = fileName[0] + ".ascii";
 std::ofstream File(name, std::ios::out);
 if (!File) {
   G4cout << "\n---> muHistoManager::saveAscii(): cannot open " << name << G4endl;
    return;
  }
  File.setf( std::ios::scientific, std::ios::floatfield );
  //write selected histograms
  for (G4int ih=0; ih<kMaxHisto; ih++) {
    if (fHistPt[ih] && fAscii[ih]) {
      File << "\n  1D histogram " << ih << ": " << fTitle[ih]
           << "\n \n \t     X \t\t     Y" << G4endl;

      for (G4int iBin=0; iBin<fNbins[ih]; iBin++) {
         File << "  " << iBin << "\t"
              << fHistPt[ih]->axis().bin_center(iBin) << "\t"
              << fHistPt[ih]->bin_height(iBin)
              << G4endl;
      }
    }
  }
}
