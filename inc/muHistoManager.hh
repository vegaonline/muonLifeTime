//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muHistoManager.hh                              *
//* ****************************************************
//  $Id: muHistoManager.hh Mar 01, 2019 10:52:08Z vega $
#ifndef muHistoManager_H
#define muHistoManager_H 1

#include "globals.hh"
#include "g4root.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include <fstream>

const G4int kMaxHisto = 15;

class muHistoManager{
public:
  muHistoManager();
  ~muHistoManager();

public:
  void Book();
  void Save();
  void SetHisto(G4int, G4int, G4double, G4double, const G4String& unit="none");
  void FillHisto(G4int, G4double, G4double weight = 1.0);
  void Normalize(G4int, G4double);
  void PrintHisto(G4int);

  inline void      SetFileName (const G4String& name) { fileName[0] = name;};
  inline G4bool    HistoExist  (G4int id) {return fExist[id];}
  inline G4String  GetTitle    (G4int id) {return fTitle[id];}
  inline G4int     GetNbins    (G4int id) {return fNbins[id];}
  inline G4double  GetVmin     (G4int id) {return fVmin[id];}
  inline G4double  GetVmax     (G4int id) {return fVmax[id];}
  inline G4double  GetHistoUnit(G4int id) {return fUnit[id];}
  inline G4double  GetBinWidth (G4int id) {return fWidth[id];}
  inline G4int     GetHistoID  (G4int id) {return fHistId[id];}

private:
  void SaveAscii();

  G4String         fileName[2];
  G4bool           factoryOn;

  G4int            fNbHist;
  G4int            fHistId[kMaxHisto];
  G4AnaH1*         fHistPt[kMaxHisto];
  G4bool           fExist[kMaxHisto];
  G4String         fLabel[kMaxHisto];
  G4String         fTitle[kMaxHisto];
  G4int            fNbins[kMaxHisto];
  G4double         fVmin [kMaxHisto];
  G4double         fVmax [kMaxHisto];
  G4double         fUnit [kMaxHisto];
  G4double         fWidth[kMaxHisto];
  G4bool           fAscii[kMaxHisto];
  //muHistoMessenger*  fHistoMessenger;
};

#endif
