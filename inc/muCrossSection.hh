//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muCrossSection.hh                           *
//* ****************************************************
//  $Id: muCrossSection.hh Feb 14, 2019 16:52:08Z vega $
#ifndef muCrossSection_H
#define muCrossSection_H 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

class G4Material;
class G4Element;

class muCrossSection{
public:
  muCrossSection();
  ~muCrossSection();
public:
  G4double CR_Macroscopic (const G4String&, G4Material*, G4double, G4double);
  G4double CR_PerAtom (const G4String&, G4Material*, G4double, G4double);

private:
  G4double CRB_Mephi (G4double, G4double, G4double, G4double);
  G4double CRK_Mephi (G4double, G4double, G4double, G4double);
  G4double CRN_Mephi (G4double, G4double, G4double, G4double);
  G4double CRP_Mephi (G4double, G4double, G4double, G4double);
};

#endif
