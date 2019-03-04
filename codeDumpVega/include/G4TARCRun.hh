/*************************************************
* @file      G4TARCRun.hh
* @author    Abhijit Bhattacharyya
* @brief     This is for run i.e. to
*                 calculate energy deposition
************************************************/
#ifndef G4TARC_RUN_HH
#define G4TARC_RUN_HH 1

#include "G4Run.hh"
#include "G4Event.hh"

#include "G4SDManager.hh"
#include "G4THitsMap.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4SystemOfUnits.hh"

#include <vector>
#include <regex>

class G4SDManager;
class G4Run;
class G4Event;

class G4TARCRun : public G4Run {

public:
  G4TARCRun();
  virtual ~G4TARCRun();

public:
  void ReadExperimentalDataFromFile(G4String& );

  inline void CalcExitingFlux(G4double exitingE)    {++fExiting_Flux; fExiting_Energy += exitingE;}
  inline void ExitingTallyCheck()                              {++fExiting_check_Flux;}

  G4int GetExitingFlux() const                {return fExiting_Flux;}
  G4double GetExitingEnergy() const     {return fExiting_Energy;}
  G4int GetExitingCheckFlux() const      {return fExiting_check_Flux;}
  G4int GetIntegralFlux_46cm() const     {return fIntegral_flux_46cm;}
  G4int GetIntegralEFlux_46cm() const   {return fTARC_Integral_Eflux_46cm;}
  G4int GetRefShellNumber() const        {return fRefShellNumber;}
  G4int GetNumberOfEvents() const       {return fNevt;}

  void SetNumberOfEvents(const G4int val)     {fNevt = val; } // std::max(fNevt, val);}
  void SetBeamEnergy(G4double val)    {fIncidentBeamEnergy = val; }
  void SetBeamParticleName(const G4String name) { fIncidentBeamParticleName = name;}

  void initVectors();
  void StartProcessing();
  void NeutronExtraCalc(const G4String&);

  virtual void RecordEvent(const G4Event*);
  virtual void Merge(const G4Run*);
  virtual void FillPerEvent(G4double ) { ;}

  G4int GetNumberOfHitsMap()  const                                            {return fRunMap->GetSize();}
  G4THitsMap<G4double> GetHitsMap(G4int idx)                      {return fRunMap[idx];}
  G4THitsMap<G4double> GetHitsMap(const G4String& detName, const G4String& colName);
  G4THitsMap<G4double> GetHitsMap(const G4String& fullName);
  void DumpAllScorer(){};

  void AddFlux(const G4String&);
  void analyseNeutronFlux(G4double, G4int, G4double, G4double,  G4String&);
  void analyseNeutronShellFluence(G4double, G4double);
  void analyseNeutronRadialFluence(G4double, G4double, G4int); //G4double, G4int);
  void analyseNeutronFluence(G4double, G4double );

  G4double GetCollectionValue(G4int) const;

private:
  G4double GetTotal(const G4THitsMap<G4double>& ) const;

public:
  G4bool flag;
  std::vector<G4double>                  fRadiusReference {200.0 * cm, 190.0 * cm, 185.0 * cm, 175.0 * cm, 165.0 * cm, 150.0 * cm,
    140.0 * cm, 130.0 * cm, 120.0 * cm, 110.0 * cm, 100.0 * cm, 90.0 * cm, 80.0 * cm, 70.0 * cm, 60.0 * cm, 50.0 * cm, 45.7 * cm,
    40.0 * cm, 30.0 * cm, 25.0 * cm, 20.0 * cm, 15.0 * cm, 10.0 * cm, 8.0 * cm, 5.0 * cm, 3.0 * cm};


  std::vector<G4double>                          fExptEnergyBin;
  std::vector<G4double>                          fRadList;

  std::vector<G4double>                          fFlux_Low;
  std::vector<G4double>                          fFlux_Low_Energy;
  std::vector<G4double>                          fFlux_Low_Data;
  std::vector<G4double>                          fFlux_Low_Syst_Err;
  std::vector<G4double>                          fFlux_Low_Energy_in;
  std::vector<G4double>                          fFlux_Low_Data_in;
  std::vector<G4double>                          fFlux_Low_Syst_Err_in;

  std::vector<G4double>                          fFlux_Lithium;
  std::vector<G4double>                          fFlux_Lithium_Energy;
  std::vector<G4double>                          fFlux_Lithium_Data;
  std::vector<G4double>                          fFlux_Lithium_Syst_Err;
  std::vector<G4double>                          fFlux_Lithium_Energy_in;
  std::vector<G4double>                          fFlux_Lithium_Data_in;
  std::vector<G4double>                          fFlux_Lithium_Syst_Err_in;

  std::vector<G4double>                          fFlux_Energy;
  std::vector<G4double>                          fFlux_Energy_in;
  std::vector<G4double>                          fFlux_Data_in;
  std::vector<G4double>                          fFlux_Data;
  std::vector<G4double>                          fFlux_Syst_Err_in;
  std::vector<G4double>                          fFlux_Syst_Err;

  std::vector<G4double>                          fFlux_He3;
  std::vector<G4double>                          fFlux_He3_Energy;
  std::vector<G4double>                          fFlux_He3_Energy_in;
  std::vector<G4double>                          fFlux_He3_Data;
  std::vector<G4double>                          fFlux_He3_Syst_Err;

  std::vector<G4double>                          fEFlux;
  std::vector<G4double>                          fEflux_Data;
  std::vector<G4double>                          fFine_Energy;
  std::vector<G4double>                          fLocal_Energy_Integral;
  std::vector<G4double>                          fFlux;
  std::vector<G4double>                          fENflux;
  std::vector<G4double>                          fNeutflux;
  std::vector<G4double>                          fFluence_Cyl;
  std::vector<G4double>                          fFluence_Step;
  std::vector<G4double>                          fFluence_Step_Shell;
  std::vector<G4double>                          fFluence_Step_Cyl;
  std::vector<G4double>                          fLow_Fluence_Step;
  std::vector<G4double>                          fLow_Fluence_Step_Shell;
  std::vector<G4double>                          fLow_Fluence_Cyl;
  std::vector<G4double>                          fCos_Low_Flux;
  std::vector<G4double>                          fCos_Flux;
  std::vector<G4double>                          fCos_He3_Flux;
  std::vector<G4double>                          fCos_Lithium_Flux;
  std::vector<G4double>                          fLithium_Flux;
  std::vector<G4double>                          fLithium_Radial_Mean;
  std::vector<G4double>                          fLithium_Radial_True_Mean;
  std::vector<G4double>                          fLithium_Radial_Energy_Upper;
  std::vector<G4double>                          fLithium_Radial_Energy_Lower;
  std::vector<G4double>                          fLithium_Fluence_Step;
  std::vector<G4double>                          fLithium_Fluence_Step_Shell;

  std::vector< std::vector<G4double> >   fExptRadiiTables;
  std::vector< std::vector<G4double> >   fExptFluenceTables;
  std::vector< std::vector<G4double> >   fExptErrTables;
  std::vector< std::vector<G4double> >   fExptEnergyTables;
  std::vector< std::vector<G4double> >   fExptFluxTables;
  std::vector< std::vector<G4double> >   fExptFluxErrTables;
  std::vector< std::vector<G4double> >   fFlux_Radius;
  std::vector< std::vector<G4double> >   fRadialFluenceStep;
  std::vector< std::vector<G4double> >   fFluence;


  G4int fNumGen;
  G4int fNevt;
  G4int fRefShellNumber = fRadiusReference.size();
  G4int fMaxRadCount;
  G4int fMaxTestFluxData;
  G4int fMaxFluxData;
  G4int fMaxFluenceData;
  G4int fMaxFluenceTable;
  G4int fTotalFlux;
  G4int fExiting_Flux;
  G4int fExiting_check_Flux;
  G4int fIntegral_flux_5cm;
  G4int fIntegral_flux_10cm;
  G4int fIntegral_flux_46cm;
  G4int fIntegral_flux_70cm;
  G4int fIntegral_flux_100cm;
  G4int fIntegral_flux_120cm;
  G4int fGamma_flux;
  G4int fNeutron_flux;
  G4int fNeutron_check;
  G4int fNeutronInit;        // k_eff
  G4int fNeutronBreed;   // k_eff
  G4int fElectron_flux;
  G4int fPiPlus_flux;
  G4int fPiMinus_flux;
  G4int fPiZero_flux;
  G4int fPositron_flux;
  G4int fProton_flux;
  G4int fAntiProton_flux;
  G4int fDeuteron_flux;
  G4int fTriton_flux;
  G4int fAlpha_flux;
  G4int fHe3_flux;
  G4int fMuon_flux;
  G4int fNucleii_flux;
  G4int fOther_flux;
  G4int fDuplicate_neutrons;
  G4int fOldTrackID;
  G4int fMaxEBin;
  G4int fNmax;
  G4int fNColl;

  G4int intDummy = 0;
  G4double floatDummy = 0.0;

  G4double fMyTol;
  G4double fMyRadTol;
  G4double fNeutron_fluence;
  G4double fTARC_Integral;
  G4double fTARC_Integral_Eflux_46cm;
  G4double fTARC_Integral_E;
  G4double fTARC_lithium;
  G4double fTARC_lithium_E;
  G4double fTARC_lithium_IntegralData;
  G4double fTARC_helium;
  G4double fTARC_helium_E;
  G4double fIntegral_EFlux;
  G4double fExiting_Energy;
  G4double fFracBinWidth;
  G4double fEnergy0;

  G4double fIncidentBeamEnergy;
  G4String  fIncidentBeamParticleName;

  G4String fParaName[2]    = {"TARCNeut", "TARCNeutSRC"};
  G4String fScorerName[18]  = {"Collisions0", "CollWeight0", "Population0", "Track_Enter0", "SL0", "SLW0", "SLWE0", "SLW_V0", "SLWE_V0","Collisions1", "CollWeight1", "Population1", "Track_Enter1", "SL1", "SLW1", "SLWE1", "SLW_V1", "SLWE_V1"};
  static const G4int fParaNumber = 2;      // (static)fParaName.size();
  static const G4int fScorerNumber = 18;    //fScorerName.size();

private:
  G4String                         fExptlDataFileName = "Data/TARC_EXPT_DATA/TARC_EXPTL_DATA.txt";
  std::vector<G4int>         fFluxTableList {36, 38};
  unsigned                         fMeanEnergyTable = 40;
  std::vector<G4double>  fMeanEnergyT40List;


    static const G4int kLim = fScorerNumber / 2 - 1;

    std::vector<G4String> fCollName;
    //  std::vector<std::vector<G4int> > fCollID;
    std::vector<G4int> fCollID;

    // G4THitsMap<G4double> fRunMapTARC[fParaNumber][fScorerNumber]; // map for accumulation [i][j]: i for det j for scorer

    //std::vector<std::vector<G4THitsMap<G4double>* > > fRunMapTARC;

    //std::vector<G4THitsMap<G4double>* > fRunMap;
    G4THitsMap<G4double> fRunMap[fScorerNumber];

  /*
  std::vector<std::vector<G4THitsMap<G4double> > > fRunMapSum; // [2][16]
  std::vector<std::vector<G4int> > fColIDSum;                  // [fDetname.size][fPrimNameSum.size]
  std::vector<std::vector<G4THitsMap<G4double> > > fRunMapPara;// [2][16]
  std::vector<std::vector<G4int> > fColIDPara;                 // [fParaName.size][fPrimeNameSum.size]
  */

};
#endif
