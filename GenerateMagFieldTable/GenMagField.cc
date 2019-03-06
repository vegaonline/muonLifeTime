#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>

#define PI 2.0 * asin(1.0)

int main() {
  std::string fileName = "magField3D.tab";
  std::ofstream outFile(fileName);

  // declare parameters
  int nx, ny, nz;      // fix dimension of the table

  double epsa = 1.0e-2; // small offset for the thickness
  double mu0= 4.0 * PI * 1.0e-7; // Permeabilities in H/m
  double mur_Air = 1.0;
  double mur_Fe = 5000.0;
  double mur_Fe_annealed = 200000.0;
  double mur_CoFe = 18000.0;
  double muPlate = (mu0 * mur_Fe); // this is mu0 * mur of specific material
  double muGap = (mu0 * mur_Air); // this is mu0 * mur of specific material
  double fMU;

  double fLength = 1000.0;     // Plate length in mm
  double fWidth  = 1000.0;     // Plate width
  double fPlateThickness = 56; // Plate thickness
  double fPlateGap = 40.0;     // Gap between the two plates of magnet
  double fXMaxPlate, fXMinPlate;
  double fYMaxPlate1, fYMaxPlate2, fYMinPlate1, fYMinPlate2;
  double fZMaxPlate, fZMinPlate;
  double fXMaxGap, fXMinGap, fYMaxGap, fYMinGap, fZMaxGap, fZMinGap;

  double fDX, fDY, fDYP1, fDYP2, fDYG, fDZ;
  double x0, y0, z0;
  double xval = -9999.9, yval = -9999.9, zval = -9999.9;
  double b0 = 0.0, fBX = b0, fBY = b0, fBZ = b0;
  
  std::string buffer = "";


  // declare the values to the parameters
  nx = 10;
  ny = 10;
  nz = 10;

  fXMaxPlate = 0.5 * fLength;           // declares length 
  fXMinPlate = -1.0 * fXMaxPlate;  
  fXMaxGap = fXMaxPlate;
  fXMinGap = -1.0 * fXMaxGap;

  fZMaxPlate = 0.5 * fWidth;            // declares Width
  fZMinPlate = -1.0 * fZMaxPlate;
  fZMaxGap = fZMaxPlate;
  fZMinGap = fXMinGap;
  
  fYMaxGap = 0.5 * fPlateGap;          // max min height of gap
  fYMinGap = -1.0 * fYMaxGap;
  
  fYMaxPlate1 = fPlateThickness + fYMaxGap;  // max min height of top plate
  fYMinPlate1 = fYMaxGap;
  fYMaxPlate2 = fYMinGap;                     // max min height of top plate
  fYMinPlate2 = fYMaxPlate2 - fPlateThickness;

  fDX   = (fXMaxPlate - fXMinPlate) / (1.0 * nx - 1.0);   // DX same for all
  fDYP1 = fPlateThickness / (1.0 * ny - 1.0); // DY for Plate 1
  fDYG  = fPlateGap / (1.0 * ny - 1.0);       // DY for Gap
  fDYP2 = fDYP1;                        // Dy for plate 2
  fDZ   = (fZMaxPlate - fZMinPlate) / (1.0 * nz);   // DZ same for all

  std::cout << fXMinPlate  << "   " << fXMaxPlate  << "   "
            << fYMinPlate1 << "   " << fYMaxPlate1 << "   "
            << fYMinGap    << "   " << fYMaxGap    << "   "
            << fYMinPlate2 << "   " << fYMaxPlate2 << std::endl;
  
  
  b0 = 1.0;  // Tesla

  // Create values and dump the values to the data file table

  outFile << buffer << std::endl;  // the first line is blank

  buffer = " ";
  outFile << buffer << nx << "    " << (3 * ny) << "    " << nz << std::endl;
  buffer = "  ";

   // Insert Headers in rows
  outFile << " #  X(mm)"    << "    "
          << " Y(mm)"      << "    " 
          << " Z(mm)"      << "  "
          << " BX(tesla)"  << ""
          << " BY(tesla)"  << ""
          << " BZ(tesla)"  << ""
          << " MU(H/m) "
          << std::endl;

  // field value top to bottom and min to max
  x0 = fXMinPlate;
  y0 = fYMaxPlate1;
  z0 = fZMinPlate;
  yval = y0;

  outFile<< std::fixed;
  outFile << std::setprecision(3);

  // Top Plate
  y0 = fYMaxPlate1;
  fDY = fDYP1;
  fMU = muPlate;
  for (int iy = 0; iy < ny; iy++){
    yval = y0 - iy * fDY;
    x0 = fXMinPlate;
    for (int ix = 0; ix < nx; ix++) {
      xval = x0 + ix * fDX;
      z0 = fZMinPlate;
      for (int iz = 0; iz < nz; iz++) {
	zval = z0 + iz * nz;
	fBX = 0.0;
	fBY = b0;
	fBZ = 0.0;
     	outFile << buffer << xval
		<< buffer << yval
		<< buffer << zval
	        << buffer << buffer << fBX
	        << buffer << buffer << buffer << fBY
	        << buffer << buffer << fBZ
	        << buffer << buffer << fMU 
	        << std::endl;
      }
    }
  }

  // Air Gap
  y0 = fYMinPlate1 + epsa;
  fDY = fDYG;
  fMU = muGap;
  for (int iy = 0; iy < ny; iy++){
    yval = y0 - iy * fDY;
    x0 = fXMinPlate;
    for (int ix = 0; ix < nx; ix++) {
      xval = x0 + ix * fDX;
      z0 = fZMinPlate;
      for (int iz = 0; iz < nz; iz++) {
	zval = z0 + iz * nz;
	fBX = 0.0;
	fBY = b0;
	fBZ = 0.0;
     	outFile << buffer << xval
		<< buffer << yval
		<< buffer << zval
	        << buffer << buffer << fBX
	        << buffer << buffer << buffer << fBY
	        << buffer << buffer << fBZ
	        << buffer << buffer << fMU 
	        << std::endl;
      }
    }
  }

  // Bottom  Plate
  y0 = fYMaxPlate2;
  fDY = fDYP2;
  fMU = muPlate;
  for (int iy = 0; iy < ny; iy++){
    yval = y0 - iy * fDY;
    x0 = fXMinPlate;
    for (int ix = 0; ix < nx; ix++) {
      xval = x0 + ix * fDX;
      z0 = fZMinPlate;
      for (int iz = 0; iz < nz; iz++) {
	zval = z0 + iz * nz;
	fBX = 0.0;
	fBY = b0;
	fBZ = 0.0;
     	outFile << buffer << xval
		<< buffer << yval
		<< buffer << zval
	        << buffer << buffer << fBX
	        << buffer << buffer << buffer << fBY
	        << buffer << buffer << fBZ
	        << buffer << buffer << fMU 
	        << std::endl;
      }
    }
  }


  outFile.close();
  return 0;
}
  
