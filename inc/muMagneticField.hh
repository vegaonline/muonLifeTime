//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muMagneticField.hh                          *
//* ****************************************************
//  $Id: muMagneticField.hh Feb 15, 2019 10:05:08Z vega $
#ifndef muMagneticField_H
#define muMagneticField_H 1

#include "globals.hh"
#include "G4MagneticField.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"

#include <vector>

class G4GenericMessenger;

class muMagneticField : public G4MagneticField{
public:
  muMagneticField();
  virtual ~muMagneticField();

public:
  void SetField(G4double, G4double, G4double);
  std::vector<G4double> GetField() const { return  fMagField; }
  G4double GetFieldX()             const { return fBX;}
  G4double GetFieldY()             const { return fBY;}
  G4double GetFieldZ()             const { return fBZ;}

private:
  void DefineCommands();
  std::vector<G4double> fMagField;
  G4GenericMessenger* fMessenger;
  G4double fBX, fBY, fBZ;
};
#endif
