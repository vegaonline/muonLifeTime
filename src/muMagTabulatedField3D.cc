//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muMagTabulatedField3D.cc                   *
//* ****************************************************
//  $Id: muMagTabulatedField3D.cc Feb 05, 2019 15:25:08Z vega $


#include "muMagTabulatedField3D.hh"
namespace{
  G4Mutex  muMagTabulatedField3DLock = G4MUTEX_INITIALIZER;
}

muMagTabulatedField3D::muMagTabulatedField3D(const char* fileName) : fInvertX(false), fInvertY(false), fInvertZ(false) {
  double lenUnit = mm;
  double fieldUnit = tesla;
  G4cout << "\n ----------------------------------------------------------------------------------"
            << "\n                     Magnetic Field "
            << "\n ----------------------------------------------------------------------------------";
  G4cout << "\n ----> reading the field grid from " << fileName << " --- " << G4endl;

  G4AutoLock lock(&muMagTabulatedField3DLock);  // thread local to stop opening file simulataneously by all workers

  std::ifstream inFile(fileName); // open the fieldmap file for reading

  if (!inFile.is_open()) {
    G4ExceptionDescription ed;
    ed << " could not open the input file " << fileName << G4endl;
    G4Exception("muMagTabulatedField3D::muMagTabulatedField3D", " magField3D001", FatalException, ed); // adv//purgmag
  }

  // Ignore the first blank line
  char buffer[256];
  inFile.getline(buffer, 256);

  //Read Table dimensions
  inFile >> nx >> ny >> nz;

  // Setup storage space for the table
  xField.resize( nx );
  yField.resize( nx );
  zField.resize( nx );
  int ix, iy, iz;
  for (ix = 0; ix < nx; ix++) {
    xField[ix].resize(ny);
    yField[ix].resize(ny);
    zField[ix].resize(ny);
    for (iy = 0; iy < ny; iy++) {
      xField[ix][iy].resize(nz);
      yField[ix][iy].resize(nz);
      zField[ix][iy].resize(nz);
    }
  }

  // The line where the second character is "0" considered as the last line of the header
  do {
    inFile.getline(buffer, 256);
  } while (buffer[1]!='0');

  // Read data
  double fXval, fYval, fZval, fBX, fBY, fBZ;
  double permeability;   // not used for the muon study
  for (ix = 0; ix < nx; ix++) {
    for (iy = 0; iy < ny; iy++) {
      for (iz = 0; iz < nz; iz++){
        inFile >> fXval >> fYval >> fZval >> fBX >> fBY >> fBZ >> permeability;
        if (ix == 0 && iy == 0 && iz == 0) {
          fMinX = fXval * lenUnit;
          fMinY = fYval * lenUnit;
          fMinZ = fZval * lenUnit;
        }
        xField[ix][iy][iz] = fBX * fieldUnit;
        yField[ix][iy][iz] = fBY * fieldUnit;
        zField[ix][iy][iz] = fBZ * fieldUnit;
      }
    }
  }
  inFile.close();
  lock.unlock();

  fMaxX = fXval * lenUnit;
  fMaxY = fYval * lenUnit;
  fMaxZ = fZval * lenUnit;

  G4cout << "\n Reading complete. " << G4endl;

  if (fMaxX < fMinX) { std::swap(fMaxX, fMinX); fInvertX = true;}
  if (fMaxY < fMinY) { std::swap(fMaxY, fMinY); fInvertY = true;}
  if (fMaxZ < fMinZ) { std::swap(fMaxZ, fMinZ); fInvertZ = true;}

  fDX = fMaxX - fMinX;
  fDY = fMaxY - fMinY;
  fDZ = fMaxZ - fMinZ;
}

void muMagTabulatedField3D::GetFieldValue(const double point[4], double *Bfield) const {
  double x = point[0];
  double y = point[1];
  double z = point[2];

  // check that  the point is within theregion of the fieldmap
  if (
    x >= fMinX && x <= fMaxX &&
    y >= fMinY && y <= fMaxY &&
    z >= fMinZ && z <= fMaxZ
  ) {
    // poisition of the given point is within the region normalized to the range [0, 1]
    double xFraction = (x - fMinX) / fDX;
    double yFraction = (y - fMinY) / fDY;
    double zFraction = (z - fMinZ) / fDZ;

    if (fInvertX) { xFraction = 1.0 - xFraction; }
    if (fInvertY) { yFraction = 1.0 - yFraction; }
    if (fInvertZ) { zFraction = 1.0 - zFraction; }

    double xdindex, ydindex, zdindex;  // *** modf function uses second argument as OUTPUT argument

    // Position of the point within the cuboid defined by the nearest surrounding tabulated points
    double xlocal = (std::modf(xFraction * (nx - 1), &xdindex));
    double ylocal = (std::modf(yFraction * (ny - 1), &ydindex));
    double zlocal = (std::modf(zFraction * (nz - 1), &zdindex));

    // The indices of the nearest tabulated point whose coordinates are all less than those of the given points
    int xindex = static_cast<int>(xdindex);
    int yindex = static_cast<int>(ydindex);
    int zindex = static_cast<int>(zdindex);

#ifdef DEBUG_INTERPOLATING_FIELD
    G4cout << "Local x,y,z: " << xlocal << " " << ylocal << " " << zlocal << endl;
    G4cout << "Index x,y,z: " << xindex << " " << yindex << " " << zindex << endl;
    double valx0z0, mulx0z0, valx1z0, mulx1z0;
    double valx0z1, mulx0z1, valx1z1, mulx1z1;
    valx0z0= table[xindex  ][0][zindex];  mulx0z0=  (1-xlocal) * (1-zlocal);
    valx1z0= table[xindex+1][0][zindex];  mulx1z0=   xlocal    * (1-zlocal);
    valx0z1= table[xindex  ][0][zindex+1]; mulx0z1= (1-xlocal) * zlocal;
    valx1z1= table[xindex+1][0][zindex+1]; mulx1z1=  xlocal    * zlocal;
#endif

    // Full 3-dimensional version
    Bfield[0] =
      xField[xindex][yindex][zindex] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
      xField[xindex][yindex][zindex+1] * (1-xlocal) * (1-ylocal) * zlocal  +
      xField[xindex][yindex+1][zindex  ] * (1-xlocal) *  ylocal * (1-zlocal) +
      xField[xindex][yindex+1][zindex+1] * (1-xlocal) * ylocal * zlocal  +
      xField[xindex+1][yindex][zindex] * xlocal * (1-ylocal) * (1-zlocal) +
      xField[xindex+1][yindex][zindex+1] * xlocal * (1-ylocal) * zlocal  +
      xField[xindex+1][yindex+1][zindex] * xlocal * ylocal  * (1-zlocal) +
      xField[xindex+1][yindex+1][zindex+1] * xlocal * ylocal * zlocal ;
     Bfield[1] =
       yField[xindex][yindex][zindex] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
       yField[xindex][yindex][zindex+1] * (1-xlocal) * (1-ylocal) * zlocal  +
       yField[xindex][yindex+1][zindex] * (1-xlocal) * ylocal * (1-zlocal) +
       yField[xindex][yindex+1][zindex+1] * (1-xlocal) * ylocal  * zlocal  +
       yField[xindex+1][yindex][zindex] * xlocal * (1-ylocal) * (1-zlocal) +
       yField[xindex+1][yindex][zindex+1] * xlocal * (1-ylocal) * zlocal  +
       yField[xindex+1][yindex+1][zindex] * xlocal * ylocal * (1-zlocal) +
       yField[xindex+1][yindex+1][zindex+1] * xlocal * ylocal * zlocal ;
     Bfield[2] =
       zField[xindex][yindex][zindex] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
       zField[xindex][yindex][zindex+1] * (1-xlocal) * (1-ylocal) *    zlocal  +
       zField[xindex][yindex+1][zindex] * (1-xlocal) *    ylocal  * (1-zlocal) +
       zField[xindex][yindex+1][zindex+1] * (1-xlocal) *    ylocal  *    zlocal  +
       zField[xindex+1][yindex][zindex] *    xlocal  * (1-ylocal) * (1-zlocal) +
       zField[xindex+1][yindex][zindex+1] *    xlocal  * (1-ylocal) *    zlocal  +
       zField[xindex+1][yindex+1][zindex] *    xlocal  *    ylocal  * (1-zlocal) +
       zField[xindex+1][yindex+1][zindex+1] *    xlocal  *    ylocal  *    zlocal ;
   } else {
     Bfield[0] = 0.0;
     Bfield[1] = 0.0;
     Bfield[2] = 0.0;
   }
}
