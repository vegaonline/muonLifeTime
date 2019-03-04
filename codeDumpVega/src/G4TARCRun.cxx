/*************************************************
* @file      G4TARCRun.cxx
* @author    Abhijit Bhattacharyya
* @brief     This is for run i.e. to
*                 calculate energy deposition
************************************************/

#include "G4TARCRun.hh"

G4TARCRun::G4TARCRun() : G4Run() {
  ReadExperimentalDataFromFile(fExptlDataFileName);
  initVectors();

  fCollID.resize(fScorerNumber, 0);
  G4SDManager* SDMan = G4SDManager::GetSDMpointer();
  SDMan->SetVerboseLevel(1);
  G4String fullName;
  G4int k = 0;
  for (G4int i = 0; i < fParaNumber; i++) {
    G4int jstart = i * (kLim + 1) ;
    for (G4int j = jstart; j < jstart + kLim + 1; j++) {
      fullName = fParaName[i] + "/" + fScorerName[j];
      fCollName.push_back(fullName);
      fCollID[k] = SDMan->GetCollectionID(fullName);
      ++k;
    }
  }
  fNColl = fCollID.size();
  //fRunMap.resize(fNColl);

  StartProcessing();
}

G4TARCRun::~G4TARCRun(){
  for (G4int i = 0; i < fNColl; i++){
    //if (fRunMap[i])
    fRunMap[i].clear();
  }
  fCollID.clear();
  fCollName.clear();

}


void G4TARCRun::RecordEvent(const G4Event* thisEvent) {
  ++fNevt;
  G4HCofThisEvent* HCE = thisEvent->GetHCofThisEvent();
  if (!HCE) return;

  for (G4int i = 0; i < fNColl; i++) {
    G4THitsMap<G4double>* evtMap = 0;
    evtMap = (G4THitsMap<G4double>*)(HCE->GetHC(fCollID[i]));
    fRunMap[i] += *evtMap;
  }

}

G4double G4TARCRun::GetTotal(const G4THitsMap<G4double>& map) const {
  G4double tot = 0;
  if (map.GetSize() == 0) return tot;
  for (std::map<G4int, G4double*>::iterator itr=map.GetMap()->begin(); itr !=map.GetMap()->end(); itr++){
    tot += *(itr->second);
  }
  return tot;
}

G4double G4TARCRun::GetCollectionValue(G4int i) const{
  G4double val = GetTotal(fRunMap[i]);
  return val;
}

G4THitsMap<G4double> G4TARCRun::GetHitsMap(const G4String& detName, const G4String& colName){
  G4String fThisfullName = detName+"/"+colName;
  return GetHitsMap(fThisfullName);
}

G4THitsMap<G4double> G4TARCRun::GetHitsMap(const G4String& fThisfullName){
  // G4int NCol = fCollName.size();
  G4THitsMap<G4double>* temp = 0;
  for (G4int i = 0; i < fNColl; i++) { //    i < NCol; i++) {
    if (fCollName[i] == fThisfullName) return fRunMap[i];
  }
  return *temp;
}

void G4TARCRun::Merge(const G4Run* thisRun) {
  const G4TARCRun* localRun = static_cast<const G4TARCRun *> (thisRun);

  for (G4int i = 0; i < fNColl; i++){
    fRunMap[i] += localRun->fRunMap[i];
  }

  fNevt                     =  std::max(fNevt, localRun->GetNumberOfEvents()); //     localRun->fNevt;
  fIncidentBeamEnergy       = localRun->fIncidentBeamEnergy;
  fExiting_Flux             += localRun->GetExitingFlux();    // fExiting_Flux;
  fExiting_Energy           += localRun->fExiting_Energy;
  fExiting_check_Flux       += localRun->fExiting_check_Flux;
  fGamma_flux               += localRun->fGamma_flux;
  fNeutron_flux             += localRun->fNeutron_flux;
  fElectron_flux            += localRun-> fElectron_flux;
  fPiMinus_flux             += localRun->fPiMinus_flux;
  fPiPlus_flux              += localRun->fPiPlus_flux;
  fPiZero_flux              += localRun->fPiZero_flux;
  fPositron_flux            += localRun->fPositron_flux;
  fProton_flux              += localRun->fProton_flux;
  fAntiProton_flux      += localRun->fAntiProton_flux;
  fMuon_flux                += localRun->fMuon_flux;
  fTriton_flux              += localRun->fTriton_flux;
  fDeuteron_flux            += localRun->fDeuteron_flux;
  fAlpha_flux               += localRun->fAlpha_flux;
  fHe3_flux                 += localRun->fHe3_flux;
  fOther_flux               += localRun->fOther_flux;
  fNeutronInit              += localRun->fNeutronInit;
  fNeutron_check            += localRun->fNeutron_check;
  fNeutron_fluence          += localRun->fNeutron_fluence;
  fIntegral_flux_46cm       += localRun->fIntegral_flux_46cm;
  fTARC_Integral_Eflux_46cm += localRun->fTARC_Integral_Eflux_46cm;
  fTotalFlux                += localRun->fTotalFlux;

  for (G4int ii = 0; ii < (fMaxTestFluxData); ii++) {
    fFlux_Energy[ii]        += localRun->fFlux_Energy[ii];
    fFlux_Data[ii]          += localRun->fFlux_Data[ii];
    fFlux[ii]               += localRun->fFlux[ii];
    fCos_Flux[ii]           += localRun->fCos_Flux[ii];
    fFluence_Step[ii]       += localRun->fFluence_Step[ii];
    fFluence_Step_Cyl[ii]   += localRun->fFluence_Step_Cyl[ii];
    fFluence_Step_Shell[ii] += localRun->fFluence_Step_Shell[ii];
    fEFlux[ii]              += localRun->fEFlux[ii];
  }

  for (G4int ii = 0; ii < fMaxFluenceData; ii++) {
    fFlux_Low[ii]               += localRun->fFlux_Low[ii];
    fFlux_Low_Energy[ii]        += localRun->fFlux_Low_Energy[ii];
    fFlux_Low_Data[ii]          += localRun->fFlux_Low_Data[ii];
    fCos_Low_Flux[ii]           += localRun->fCos_Low_Flux[ii];
    fLow_Fluence_Step[ii]       += localRun->fLow_Fluence_Step[ii];
    fLow_Fluence_Cyl[ii]        += localRun->fLow_Fluence_Cyl[ii];
    fLow_Fluence_Step_Shell[ii] += localRun->fLow_Fluence_Step_Shell[ii];

    fLithium_Flux[ii]           += localRun->fLithium_Flux[ii];
    fFlux_Lithium_Energy[ii]    += localRun->fFlux_Lithium_Energy[ii];
    fFlux_Lithium_Data[ii]      += localRun->fFlux_Lithium_Data[ii];
    fCos_Lithium_Flux[ii]       += localRun->fCos_Lithium_Flux[ii];
    fLithium_Fluence_Step[ii]   += localRun->fLithium_Fluence_Step[ii];


    for (G4int i = 0; i < fMaxRadCount; i++){
      for (G4int j = 0; j < fRefShellNumber; j++) {
        fRadialFluenceStep[j][i]+= localRun->fRadialFluenceStep[j][i];
      }
    }
  }
  G4Run::Merge(thisRun);
}
void G4TARCRun::initVectors() {
  fEnergy0 = 0.0;
  fNColl = 0;
  fNevt = 0;
  fIncidentBeamEnergy = 0.0*MeV;
  fTARC_Integral = 0.0; fTARC_Integral_E = 0.0; fTARC_lithium = 0.0;
  fTARC_lithium_E = 0.0; fTARC_helium = 0.0; fTARC_helium_E = 0.0;
  fIntegral_EFlux = 0.0;
  fIntegral_flux_5cm = 0;
  fIntegral_flux_10cm = 0;
  fIntegral_flux_46cm = 0;
  fIntegral_flux_70cm = 0;
  fIntegral_flux_100cm = 0;
  fIntegral_flux_120cm = 0;
  fTARC_Integral_Eflux_46cm = 0;
  fExiting_Flux = 0;
  fExiting_check_Flux = 0;
  fExiting_Energy = 0.0;

  fTotalFlux = 0;
  fExiting_Flux = 0;
  fExiting_check_Flux = 0;
  fIntegral_flux_5cm = 0;
  fIntegral_flux_10cm = 0;
  fIntegral_flux_46cm = 0;
  fIntegral_flux_70cm = 0;
  fIntegral_flux_100cm = 0;
  fIntegral_flux_120cm = 0;
  fGamma_flux = 0;
  fNeutron_flux = 0;
  fNeutron_check = 0;
  fNeutronInit = 0;        // k_eff
  fNeutronBreed = 0;   // k_eff
  fElectron_flux = 0;
  fPiPlus_flux = 0;
  fPiMinus_flux = 0;
  fPiZero_flux = 0;
  fPositron_flux = 0;
  fProton_flux = 0;
  fAntiProton_flux = 0;
  fDeuteron_flux = 0;
  fTriton_flux = 0;
  fAlpha_flux = 0;
  fHe3_flux = 0;
  fNucleii_flux = 0;
  fMuon_flux = 0;
  fOther_flux = 0;
  fDuplicate_neutrons = 0;

  flag = false;
  fNumGen = 0;
  fFracBinWidth = 0.2;
  fMaxEBin = 500;
  fMyTol = 1.0e-9 * mm;
  fMyRadTol = 1.0e-6 * mm;

  fLocal_Energy_Integral.resize(4, 0.0);
  fENflux.resize(4, 0.0);
  fNeutflux.resize(4, 0.0);

  fNeutronInit = 0;
  fNeutronBreed = 0;

  fRadialFluenceStep.resize(fRefShellNumber, std::vector<G4double>(fMaxRadCount, 0.0));

  fFluence.resize(fMaxFluenceTable, std::vector<G4double>(fMaxEBin, 0.0));
  fFluence_Step.resize(fMaxTestFluxData, 0.0);
  fFluence_Step_Shell.resize(fMaxTestFluxData, 0.0);
  fFluence_Step_Cyl.resize(fMaxTestFluxData, 0.0);
  fFluence_Cyl.resize(fMaxTestFluxData, 0.0);

  fLow_Fluence_Step.resize(fMaxFluenceData, 0.0);
  fLow_Fluence_Step_Shell.resize(fMaxFluenceData, 0.0);
  fLow_Fluence_Cyl.resize(fMaxFluenceData, 0.0);

  fFlux_He3.resize((fMaxTestFluxData+fMaxFluxData), 0.0);
  fFlux_Low.resize((fMaxTestFluxData+fMaxFluxData), 0.0);
  fFlux_Lithium.resize((fMaxTestFluxData+fMaxFluxData), 0.0);
  fFlux.resize((fMaxTestFluxData+fMaxFluxData), 0.0);
  fFlux_Radius.resize(fMaxRadCount, std::vector<G4double> (fMaxRadCount, 0.0));
  fEFlux.resize((fMaxTestFluxData+fMaxFluxData), 0.0);

  fCos_Lithium_Flux.resize(fMaxFluxData, 0.0);
  fCos_Low_Flux.resize(fMaxFluxData, 0.0);
  fCos_Flux.resize(fMaxFluxData, 0.0);
  fCos_He3_Flux.resize(fMaxFluxData, 0.0);

  fLithium_Flux.resize(fMaxFluxData, 0.0);
  fLithium_Radial_Mean.resize(fMaxRadCount, 0.0);
  fLithium_Radial_True_Mean.resize(fMaxRadCount, 0.0);
  fLithium_Radial_Energy_Lower.resize(fMaxRadCount, 0.0);
  fLithium_Radial_Energy_Upper.resize(fMaxRadCount, 0.0);
  fLithium_Fluence_Step.resize(fMaxFluenceData, 0.0);
  fLithium_Fluence_Step_Shell.resize(fMaxFluenceData, 0.0);

}

void G4TARCRun::StartProcessing(){
  G4double fMeanEnergy = 0.0;
  for (G4int i = 0; i < fMaxTestFluxData; i++){
    fMeanEnergy = 0.5 * (fFlux_Energy[i] + fFlux_Energy[i + 1]);
    fEflux_Data.push_back(fMeanEnergy * fFlux_Data_in[i]);
    fIntegral_EFlux += (fMeanEnergy * fFlux_Data_in[i]) / 1.0e6;
    fFine_Energy.push_back(fFlux_Energy_in[i]);
    //G4cout << i << " Energy: " << fMeanEnergy << G4endl;
  }

  G4double fScale = 100.0;
  fEnergy0 = 0.01;
  G4double fBinWidth = (std::log(1.0e5) - std::log(fEnergy0)) / fScale; // as per definition in NIM paper
  G4int fRadialIndex = 0;

  fFlux_Lithium_Energy[0] = fEnergy0;
  fFlux_Low_Energy[0] = fEnergy0;

  G4int mIndex = 0;
  for (std::size_t i = 0; i < fFlux_Low_Energy.size(); i++)  fFlux_Low_Energy[i + 1] = std::exp(fBinWidth + std::log(fFlux_Low_Energy[i]));

  for(std::size_t i = 0; i <= fFlux_Lithium_Energy.size(); i++){  // this = is imp as it generates the last item that satisfies the condition
    fFlux_Lithium_Energy[i + 1] = std::exp(fBinWidth + std::log(fFlux_Lithium_Energy[i]));
    fMeanEnergy = std::exp(0.5 * (std::log(fFlux_Low_Energy[i + 1]) + std::log(fFlux_Low_Energy[i])));
    G4double fLithiumMeanEnergy = std::exp(0.5 * (std::log(fFlux_Lithium_Energy[i + 1]) + std::log(fFlux_Lithium_Energy[i])));

    if (fFlux_Lithium_Energy[i] < fExptEnergyBin[fRadialIndex]  && fFlux_Lithium_Energy[i + 1] > fExptEnergyBin[fRadialIndex]){
      fLithium_Radial_Energy_Lower[fRadialIndex] = fFlux_Lithium_Energy[i];
      fLithium_Radial_Energy_Upper[fRadialIndex] = fFlux_Lithium_Energy[i+1];;
      fLithium_Radial_Mean[fRadialIndex] = fExptEnergyBin[fRadialIndex];
      fLithium_Radial_True_Mean[fRadialIndex] = fLithiumMeanEnergy;
      //G4cout << fRadialIndex << "      " << fLithium_Radial_Mean[fRadialIndex] << G4endl;
      ++fRadialIndex;
      //fRadialIndex = (fRadialIndex > fMaxRadCount - 1) ? (fMaxRadCount - 1) : fRadialIndex;
    }
    if (std::abs(fLithiumMeanEnergy / fFlux_Lithium_Energy_in[mIndex] - 1.0) < 0.05) {
      fFlux_Lithium_Data[i]       = fFlux_Lithium_Data_in[mIndex];
      fFlux_Lithium_Syst_Err[i] = fFlux_Lithium_Data_in[mIndex];
      fTARC_lithium_IntegralData += fFlux_Lithium_Data_in[mIndex];
      fTARC_lithium_E                  += fFlux_Lithium_Data_in[mIndex] * fLithiumMeanEnergy / 1.0e6;   // check 10^6
      ++mIndex;
    }
  }
}

void G4TARCRun::ReadExperimentalDataFromFile(G4String& exptFileName){
  //fReadData = false;
  std::ifstream exptIN(exptFileName, std::ios::in);
  G4String lineIN;
  unsigned NCount = 0, restCount = 0, file0 = 0, iTableNum = 0, fIFluxCountRef=0;
  G4bool readPara = false;
  G4double v1, v2, v3, v4, v5, v6, v7, v8, v9, v10;
  G4bool isFlux = false;
  G4int is40 = 0;
  fMaxFluenceTable = 0;
  std::vector<G4double> tmpV1;
  std::vector<G4double> tmpV2;
  std::vector<G4double> tmpV3;
  fMaxFluxData = -999999.99;
  fMaxFluenceData = -999999.99;
  fMaxTestFluxData = -999999.99;
  //G4int fileCount = 0;
  while (getline(exptIN, lineIN)){
    lineIN = std::regex_replace(lineIN, std::regex("^ +| +$|( ) +"), "$1");
    if (lineIN.size() > 1 && lineIN.find("#", 0, 1) != std::string::npos){  // if the line starts with # sign
      std::size_t found1 = lineIN.find("Table");
      readPara = false;
      G4String tableNum = (lineIN.substr(found1 + 5, 3));
      tableNum = std::regex_replace(tableNum, std::regex("^ +| +$|( ) +"), "$1"); // stripping extra spaces
      iTableNum = std::atoi(tableNum);
      isFlux = ((std::find( fFluxTableList.begin(), fFluxTableList.end(), iTableNum) != fFluxTableList.end()) || (iTableNum == fMeanEnergyTable)) ;
      if (iTableNum == fMeanEnergyTable) ++is40;
      if (found1 != std::string::npos){
        file0 = (iTableNum == 0) ? 1 : 0;
      }
      lineIN="";
    } else if (lineIN.size() > 1 && lineIN.find("#", 0, 1) == std::string::npos){   // the line does not start with # symbol
      if (lineIN.find(";", 0, 1) != std::string::npos){
        NCount = atoi(lineIN.substr(1, lineIN.size()).c_str());
        fIFluxCountRef = (iTableNum == 40) ? NCount : 0;
        restCount = NCount;
        readPara = true;
        if (!isFlux) fMaxFluenceData = (std::max(fMaxFluenceData, (signed)NCount));
        if (isFlux && !fIFluxCountRef) fMaxFluxData = (std::max(fMaxFluxData, (signed)NCount));
        fMaxTestFluxData = fIFluxCountRef;
        //std::cout << "Table->" << iTableNum << " Data-> " << NCount
        //<< " Flux: " << isFlux << " Fluence: " << !(isFlux) << std::endl;
        continue;
      }
      if ( file0 && readPara){
        std::stringstream ss (lineIN);
        ss >> v1 >> v2 >> v3 >> v4 >> v5 >> v6 >> v7 >> v8 >> v9 >> v10;
        fExptEnergyBin.push_back(v1); fExptEnergyBin.push_back(v2); fExptEnergyBin.push_back(v3); fExptEnergyBin.push_back(v4);
        fExptEnergyBin.push_back(v5); fExptEnergyBin.push_back(v6); fExptEnergyBin.push_back(v7); fExptEnergyBin.push_back(v8);
        fExptEnergyBin.push_back(v9); fExptEnergyBin.push_back(v10);
        //G4cout << "filecount->" << fileCount << "   " << fExptEnergyBin.size() << G4endl;
        //for (unsigned ijk = 0 ; ijk < fExptEnergyBin.size(); ijk++) std::cout << fExptEnergyBin[ijk] << "  ";
        //std::cout << std::endl;
        file0 = 0;
        readPara = false;
      }
      if (!file0 && readPara){
        std::istringstream sdummy;
        std::stringstream ss;
        sdummy.str(lineIN); // sdummy is used for number of components which deletes sdumy. So ss is required
        ss.str(lineIN);
        v1 = v2 = v3 = v4 = v5 = v6 = v7 = v8 = v9 = 0.0;
        G4int wcount = std::distance(std::istream_iterator<std::string>(sdummy), std::istream_iterator<std::string>());
        if (wcount == 3) {
          ss >> v1 >> v2  >> v3;
          if (isFlux && is40 == 2){
            fMeanEnergyT40List.push_back(v1);  fMeanEnergyT40List.push_back(v2);  fMeanEnergyT40List.push_back(v3);
          } else {
            tmpV1.push_back(v1);  tmpV2.push_back(v2);  tmpV3.push_back(v3);
          }
          --restCount;
        } else if (wcount == 6) {
          ss >> v1 >> v2 >> v3 >> v4 >> v5 >> v6;
          if (isFlux && is40 == 2){
            fMeanEnergyT40List.push_back(v1);  fMeanEnergyT40List.push_back(v2);  fMeanEnergyT40List.push_back(v3);
            fMeanEnergyT40List.push_back(v4);  fMeanEnergyT40List.push_back(v5);  fMeanEnergyT40List.push_back(v6);
          } else {
            tmpV1.push_back(v1);  tmpV2.push_back(v2);  tmpV3.push_back(v3);
            tmpV1.push_back(v4);  tmpV2.push_back(v5);  tmpV3.push_back(v6);
          }
          restCount -= 2;
        }else if (wcount == 9){
          ss >> v1 >> v2 >> v3 >> v4 >> v5 >> v6 >> v7 >> v8 >> v9;
          if (isFlux && is40 == 2){
            fMeanEnergyT40List.push_back(v1);  fMeanEnergyT40List.push_back(v2);  fMeanEnergyT40List.push_back(v3);
            fMeanEnergyT40List.push_back(v4);  fMeanEnergyT40List.push_back(v5);  fMeanEnergyT40List.push_back(v6);
            fMeanEnergyT40List.push_back(v7);  fMeanEnergyT40List.push_back(v8);  fMeanEnergyT40List.push_back(v8);
            fMeanEnergyT40List.push_back(v9);
          } else {
            tmpV1.push_back(v1);  tmpV2.push_back(v2);  tmpV3.push_back(v3);
            tmpV1.push_back(v4);  tmpV2.push_back(v5);  tmpV3.push_back(v6);
            tmpV1.push_back(v7);  tmpV2.push_back(v8);  tmpV3.push_back(v9);
          }
          restCount -= 3;
        }
      }
      if (!isFlux && (tmpV1.size()) == NCount){
        if (iTableNum!=0) ++fMaxFluenceTable;
        fExptRadiiTables.push_back(tmpV1);
        fExptFluenceTables.push_back(tmpV2);
        fExptErrTables.push_back(tmpV3);
        std::vector<G4double>().swap(tmpV1);
        std::vector<G4double>().swap(tmpV2);
        std::vector<G4double>().swap(tmpV3);
      } else if (isFlux && (tmpV1.size()) == NCount  && is40 !=2) {
        fExptEnergyTables.push_back(tmpV1);
        fExptFluxTables.push_back(tmpV2);
        fExptFluxErrTables.push_back(tmpV3);
        std::vector<G4double>().swap(tmpV1);
        std::vector<G4double>().swap(tmpV2);
        std::vector<G4double>().swap(tmpV3);
      }
    }
    lineIN="";
  }
  exptIN.close();
  std::vector<G4double>().swap(tmpV1);
  std::vector<G4double>().swap(tmpV2);
  std::vector<G4double>().swap(tmpV3);

  // Now sorting the vector fMeanEnergyT40List
  std::sort(fMeanEnergyT40List.begin(), fMeanEnergyT40List.end(),  [] (G4double const& a, G4double const& b) { return a < b; });

  fMaxRadCount = fExptRadiiTables[8].size();
  //  G4cout << " Rad count = " << fMaxRadCount << G4endl;
  //  fMaxRadCount = 10  fMaxTestFluxData = 21;
  //  fMaxFluxData  = 95 fMaxFluenceData  = 102

  for (std::size_t i = 0; i < fExptRadiiTables.size(); i++){
    for (std::size_t j = 0; j < fExptRadiiTables[i].size(); j++) {
      fExptRadiiTables[i][j] *= 10.0;  // in mm now
    }
  }

  for (std::size_t ijk2 = 0; ijk2 < fExptRadiiTables[3].size(); ijk2++){
    G4double rVal = fExptRadiiTables[3][ijk2];
    if (floatDummy != rVal)   fRadList.push_back(rVal);
    floatDummy = rVal;
  }

  for (std::size_t ij1 = 0; ij1 < fExptEnergyTables[0].size(); ij1++){    // Table 36
    fFlux_Low_Energy.push_back(fExptEnergyTables[0][ij1]);
    fFlux_Low_Data.push_back(fExptFluxTables[0][ij1]);
    fFlux_Low_Syst_Err.push_back(fExptFluxErrTables[0][ij1]);
  }

  for (std::size_t ij1 = 0; ij1 < fExptEnergyTables[1].size(); ij1++){   // Table 38
    fFlux_Lithium_Energy.push_back(fExptEnergyTables[1][ij1]);
    fFlux_Lithium_Data.push_back(fExptFluxTables[1][ij1]);
    fFlux_Lithium_Syst_Err.push_back(fExptFluxErrTables[1][ij1]);
  }

  for (std::size_t ij1 = 0; ij1 < fExptEnergyTables[2].size(); ij1++){   // Table 40
    fFlux_Energy.push_back(fExptEnergyTables[2][ij1]);
    fFlux_Data.push_back(fExptFluxTables[2][ij1]);
    fFlux_Syst_Err.push_back(fExptFluxErrTables[2][ij1]);
  }

  fFlux_Energy_in = fFlux_Energy;
  fFlux_Data_in = fFlux_Data;
  fFlux_Syst_Err_in = fFlux_Syst_Err;

  fFlux_Low_Energy_in = fFlux_Low_Energy;
  fFlux_Low_Data_in = fFlux_Low_Data;
  fFlux_Low_Syst_Err_in = fFlux_Low_Syst_Err;

  fFlux_Lithium_Energy_in = fFlux_Lithium_Energy;
  fFlux_Lithium_Data_in = fFlux_Lithium_Data;
  fFlux_Lithium_Syst_Err_in = fFlux_Lithium_Syst_Err;

  // This is a test to shrink use of memory
  //std::vector<std::vector<G4double> > ().swap(fExptEnergyTables);
  //std::vector<std::vector<G4double> > ().swap(fExptFluxTables);
  //std::vector<std::vector<G4double> > ().swap(fExptFluxErrTables);
  // This is end of test block
  //fReadData = true;
}


void G4TARCRun::AddFlux(const G4String& particleName) {
  if(particleName == "gamma")               fGamma_flux++;
  if(particleName == "neutron")               fNeutron_flux++;
  if(particleName == "e-")                        fElectron_flux++;
  if(particleName == "pi-")                       fPiMinus_flux++;
  if(particleName == "pi+")                      fPiPlus_flux++;
  if(particleName == "pi0")                      fPiZero_flux++;
  if(particleName == "e+")                       fPositron_flux++;
  if(particleName == "proton")                 fProton_flux++;
  if(particleName == "anti_proton")         fAntiProton_flux++;
  if(particleName == "alpha")                   fAlpha_flux++;
  if(particleName == "He3")                     fHe3_flux++;
  if(particleName == "deuteron")              fDeuteron_flux++;
  if(particleName == "triton")                   fTriton_flux++;
  if(particleName == "mu-")                     fMuon_flux++;
  if(particleName == "mu+")                    fMuon_flux++;
  if(particleName == "other")                   fOther_flux++;
  if(particleName == "neutron_check")    fNeutron_check++;
  if(particleName == "neutron_fluence")  fNeutron_fluence++;
}

void G4TARCRun::analyseNeutronRadialFluence(G4double fParticleEnergyL, //G4double fParticleTimeL,
  G4double StepLengthL, G4int ishellL){
    if (ishellL < 0 || ishellL > fRefShellNumber) G4cout << "WARNING! radial index is wrong !!!!!!!" << G4endl;
    G4double tempEnergy = fParticleEnergyL / eV;
    if (tempEnergy <= fLithium_Radial_Energy_Upper[9] && tempEnergy >= fLithium_Radial_Energy_Lower[0]){
      for (G4int i = 0 ; i <= fMaxRadCount; ++i) {
        if (tempEnergy >= fLithium_Radial_Energy_Lower[i] && tempEnergy <= fLithium_Radial_Energy_Upper[i]){
          fRadialFluenceStep[ishellL][i] += StepLengthL;  //  (StepLengthL / mm);  steplength is in mm
        }
      }
    }
}


void G4TARCRun::analyseNeutronFluence(G4double energyL, G4double thisStepL) {
  //   , G4double timeL, G4int thisTrackIDL,
  //G4double radiusL, G4double thisStepL,  G4int ParentIDL, G4double parentEnergyL, G4String& parentParticleL){
  G4double fTempE = energyL / eV;

  //if (fTempE >= fFlux_Energy[fFlux_Energy.size()-1]){
  if (fTempE >= fFlux_Energy[0]){
    for (G4int ii = 0; ii < fMaxTestFluxData; ii++){
      if (fTempE > fFlux_Energy[ii] && fTempE < fFlux_Energy[ii + 1]) fFluence_Step[ii] += thisStepL;
    }
  }
}




void G4TARCRun::analyseNeutronShellFluence(G4double energyL, G4double StepLengthL){
  G4double tempE = energyL / eV;

  if (tempE < fFlux_Lithium_Energy[fMaxFluxData - 1]){
    for (G4int ii1 = 0; ii1 < fMaxFluxData; ii1++){
      if (tempE > fFlux_Lithium_Energy[ii1] && tempE < fFlux_Lithium_Energy[ii1 + 1]) fLithium_Fluence_Step_Shell[ii1] += StepLengthL;
    }
  }
  std::size_t lastTagLowEFlux = fFlux_Low_Energy.size();
  if (tempE < fFlux_Low_Energy[lastTagLowEFlux - 1]){
    for (std::size_t ii1 = 0; ii1 < lastTagLowEFlux; ii1++){
      if (tempE > fFlux_Low_Energy[ii1] && tempE < fFlux_Low_Energy[ii1 + 1]) fLow_Fluence_Step_Shell[ii1] += StepLengthL;
    }
  }
  std::size_t lastTagFluxE = fFlux_Energy.size();
  if (tempE > fFlux_Energy[0]){
    for (std::size_t ii1 = 0; ii1 < lastTagFluxE; ii1++){
      if (tempE > fFlux_Energy[ii1] && tempE < fFlux_Energy[ii1 + 1]) fFluence_Step_Shell[ii1] += StepLengthL;
    }
  }
}



void G4TARCRun::analyseNeutronFlux(G4double n_EnergyL, G4int thisTrackIDL, G4double radiusL,
  G4double cosAngleL, G4String& fParticleNameL)
  //G4double zPosL,G4double cosAngleL, G4String fParticleNameL)
  {
    //G4cout << "n_EnergyL: " << n_EnergyL << "   " << n_EnergyL / eV << "  rad: " << radiusL << "    " << radiusL / mm << G4endl;
    G4double OnebyCosAngle = 1.0 / std::abs(cosAngleL);
    if (fParticleNameL == "neutron"){
      if (thisTrackIDL == fOldTrackID && std::abs(radiusL - 456.0) <= 0.01){
        ++fDuplicate_neutrons;
      } else {
        fDuplicate_neutrons = 0;
      }
      fOldTrackID = thisTrackIDL;
    }
    G4double tempEnergy = n_EnergyL / eV;

    if (fParticleNameL == "neutron"){
      for (G4int ii1 = 0; ii1 < fMaxRadCount; ii1++){
        if (std::abs(radiusL - fRadList[ii1]) <= 0.01){
          for (G4int ii2 = 0; ii2 < fMaxRadCount; ii2++){
            // if (std::abs(n_EnergyL - fExptEnergyBin[ii2]) < fractional_fBinWidth * fExptEnergyBin[ii2])
            if (std::abs(tempEnergy - fExptEnergyBin[ii2]) < (fFracBinWidth * fExptEnergyBin[ii2]))
              fFlux_Radius[ii1][ii2] += OnebyCosAngle;
          }
        }
      }

      if (std::abs(radiusL - 456.0) <= 0.01){
        // if (n_EnergyL > 0.345*eV && n_EnergyL < 1.0e5*eV){
        if (tempEnergy > 0.345 && tempEnergy < 1.0e5){
          fTARC_Integral   += OnebyCosAngle;
          fTARC_Integral_E += tempEnergy * OnebyCosAngle;
        }
        //G4int nVal = (G4int)((2.0 + std::log10(n_EnergyL / eV)) / 0.09);
        G4int nVal = (G4int)((2.0 + std::log10(tempEnergy)) / 0.09);
        if (nVal < 0) nVal = 0;
        if (nVal < fMaxFluxData) {
          //fFluence_Spectrum[nVal] += 1.0 /std::abs(cosAngleL);
          if (nVal > fNmax) fNmax = nVal;
        }
        if (tempEnergy > 0.0194 && tempEnergy < 1.0e5){
          fTARC_lithium   += OnebyCosAngle;
          fTARC_lithium_E += tempEnergy * OnebyCosAngle; // / std::abs(cosAngleL);
        }
        if (tempEnergy > 59500.0 && tempEnergy < 1825092.0){
          fTARC_helium   +=OnebyCosAngle;
          fTARC_helium_E += tempEnergy * OnebyCosAngle; // / std::abs(cosAngleL);
        }
      }

      std::size_t LithiumMax = fFlux_Lithium_Energy.size();
/*
      for (std::size_t iii = 0; iii < LithiumMax; iii++){
        if (tempEnergy < fFlux_Lithium_Energy[iii])  G4cout << tempEnergy << "     " << fFlux_Lithium_Energy[iii] << G4endl;
      }
*/

      if (std::abs(radiusL - 50.0) <= 0.01) ++fIntegral_flux_5cm;
      if (std::abs(radiusL - 100.0) <= 0.01) ++fIntegral_flux_10cm;
      if (std::abs(radiusL - 700.0) <= 0.01) ++fIntegral_flux_70cm;
      if (std::abs(radiusL - 1000.0) <= 0.01) ++fIntegral_flux_100cm;
      if (std::abs(radiusL - 1200.0) <= 0.01) ++fIntegral_flux_120cm;

      if (std::abs(radiusL - 456.0) <= 0.01){
        ++fIntegral_flux_46cm;
        fTARC_Integral_Eflux_46cm += tempEnergy;
        fTotalFlux++;
        if (tempEnergy < fFlux_Energy[0]) fLocal_Energy_Integral[0] += tempEnergy;
        if (tempEnergy < fFlux_Lithium_Energy[LithiumMax-1]){
          for (std::size_t ijk1 = 0; ijk1 < LithiumMax; ijk1++){
            if(tempEnergy > fFlux_Lithium_Energy[ijk1] && tempEnergy < fFlux_Lithium_Energy[ijk1 + 1]){
              fLithium_Flux[ijk1] += 1.0;
              fCos_Lithium_Flux[ijk1] = OnebyCosAngle;
            }
          }
        }
        if (tempEnergy < fFlux_Low_Energy[fMaxFluxData]){
          for (G4int ijk1 = 0; ijk1 < fMaxFluxData; ijk1++){
            if (tempEnergy >fFlux_Low_Energy[ijk1] && tempEnergy < fFlux_Low_Energy[ijk1 + 1]){
              fFlux_Low[ijk1] += 1.0;
              fCos_Low_Flux[ijk1] += OnebyCosAngle;
            }
          }
        } else if ( tempEnergy > fFlux_Energy[0]) {
            for(G4int ijk1 = 0; ijk1 < fMaxTestFluxData; ijk1++){
              if (tempEnergy > fFlux_Energy[ijk1] && tempEnergy < fFlux_Energy[ijk1 + 1]){
                fLocal_Energy_Integral[1] += tempEnergy;
                fFlux[ijk1]++;
                if (cosAngleL != 0.0) fCos_Flux[ijk1] += OnebyCosAngle;
                fCos_Low_Flux[ijk1] = fCos_Flux[ijk1];
                fEFlux[ijk1] += tempEnergy;
              }
            }
        }
      }
    }
}

void G4TARCRun::NeutronExtraCalc(const G4String& procID){
  if (procID == "init") ++fNeutronInit;
}
