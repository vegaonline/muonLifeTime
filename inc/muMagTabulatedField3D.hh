//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muMagTabulatedField3D.hh                   *
//* ****************************************************
//  $Id: muMagTabulatedField3D.hh Feb 05, 2019 15:25:08Z vega $
#ifndef muMagTabulatedField3D_H
#define muMagTabulatedField3D_H 1

#include "globals.hh"
#include "G4MagneticField.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4AutoLock.hh"

#include <fstream>
#include <vector>
#include <cmath>

class muMagTabulatedField3D
#ifndef STANDALONE
  : public G4MagneticField
#endif
{
  std::vector< std::vector< std::vector< double > > > xField;
  std::vector< std::vector< std::vector< double > > > yField;
  std::vector< std::vector< std::vector< double > > > zField;

  int nx, ny, nz;                                                                            // specifying the dimension of the magnetic field table
  double fMaxX, fMinX, fMaxY, fMinY, fMaxZ, fMinZ;        // physical limit of the defined region
  double fDX, fDY, fDZ;
  bool fInvertX, fInvertY, fInvertZ;


public:
  muMagTabulatedField3D(const char* fileName);
  void GetFieldValue( const double Point[4], double *Bfield ) const;

};


#endif
