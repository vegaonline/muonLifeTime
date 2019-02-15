//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muMagneticField.cc                          *
//* ****************************************************
//  $Id: muMagneticField.cc Feb 02, 2019 10:47:08Z vega $

#include "muMagneticField.hh"

muMagneticField::muMagneticField : G4MagneticField(), fMessenger(0), fBX(0.0*tesla), fBY(1.0*tesla), fBZ(0.0*tesla){
  SetField(fBX, fBY, fBZ);
  DefineCommands();
}

muMagneticField::~muMagneticField(){
  delete fMessenger;
}


muMagneticField::SetField(G4double valX, G4double valY, G4double valZ){
  fMagField[0] = valX;
  fMagField[1] = valY;
  fMagField[2] = valZ;
}


void muMagneticField::DefineCommands() {
  fMessenger = new G4GenericMessenger(this, "/muondir/field/", "Field control"); // define command directory
  auto & fValcmd = fMessenger->DeclareMethodWithUnit("value", "tesla", &muMagneticField::SetField, "Set Field strength");
  fValcmd.SetParameterName("field", true);
  fValcmd.SetDefaultValue("0.0, 1.0, 0.0");

}
