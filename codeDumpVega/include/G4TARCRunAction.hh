/*************************************************
* @file      G4TARCRunAction.hh
* @author    Abhijit Bhattacharyya
* @brief     This is for run Action i.e. to
*                 calculate energy deposition
************************************************/
#ifndef G4TARC_RUNACTION_HH
#define G4TARC_RUNACTION_HH 1

#include "G4UserRunAction.hh"
#include "G4NuclearLevelData.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"
#include "G4UserRunAction.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "globals.hh"

#include "G4TARCRun.hh"
#include "G4TARCAnalysis.hh"
#include "G4TARCPrimaryGeneratorAction.hh"

#include "Randomize.hh"
#include <fstream>
#include <iomanip>

class G4Run;
class G4TARCRun;

class G4TARCRunAction : public G4UserRunAction {
public:
  G4TARCRunAction();
  virtual ~G4TARCRunAction();

public:
  virtual G4Run* GenerateRun();
  virtual void BeginOfRunAction( const G4Run* );
  virtual void EndOfRunAction( const G4Run* );

  void DefineShellBlocks();
  //   void ReadExperimentalDataFromFile(G4String&);
  void InitVecs();
  void BookHistogram();
  //void CreateTuples();
  void FillRadialExperimentalData(const G4TARCRun*);

  void NeutronFluxHistogram(G4int, const G4TARCRun*);
  void RadialFluxHistogram(G4int, const G4TARCRun*);
  void ResultSummary(const G4int, const G4TARCRun*);
  template <typename T>  void Check10s(T, T&, G4String&);

private:
  G4int           fEventNum;
  G4String        fAnalysisFileName = "G4TARC_output";
  G4bool          fHistoBooked = false;

  std::vector<G4double>   fRadiusReference {200.0 * cm, 190.0 * cm, 185.0 * cm, 175.0 * cm, 165.0 * cm, 150.0 * cm,
    140.0 * cm, 130.0 * cm, 120.0 * cm, 110.0 * cm, 100.0 * cm, 90.0 * cm, 80.0 * cm, 70.0 * cm, 60.0 * cm, 50.0 * cm, 45.7 * cm,
    40.0 * cm, 30.0 * cm, 25.0 * cm, 20.0 * cm, 15.0 * cm, 10.0 * cm, 8.0 * cm, 5.0 * cm, 3.0 * cm};

  std::vector<G4double>   fOuterRadiusofShell;
  std::vector<G4double>   fInnerRadiusofShell;
  std::vector<G4double> fLocal_Energy_Integral;

  G4int    fRefShellNumber;

  G4double fRefShellThickness;
  G4double fRadHole;
  G4double fLenCyl;
  G4double fRefShellInnerRad;
  G4double fRefShellOuterRad;
  G4double fTestSphereRadius;
  G4double fTestSphereSurfaceArea;
  G4double fTestSphereVolume;
  G4double fTestShellVol;
  G4double fTARC_helium;
  G4double fTARC_helium_E;
  G4double fTARC_Integral;
  G4double fTARC_Integral_E;
  G4double fTARC_lithium;
  G4double fTARC_lithium_E;
  G4double fAbsolute_TotalFlux;
  G4double fAbsFlux;
  G4double fAbsEFlux;
  G4double fAbsFluence;
  G4double fAbsFluenceShell;

};


#endif
