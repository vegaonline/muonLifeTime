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
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"

#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"
#include "G4SystemOfUnits.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include <vector>

class G4GenericMessenger;
class G4G4FieldManager;
class G4ChordFinder;
class G4Mag_UsualEqRhs;
class G4MagIntegratorStepper;

class muMagneticField : public G4MagneticField{
public:
  muMagneticField(); // A zero field
  virtual ~muMagneticField();

public:
  void SetStepperType(G4int i) { fStepperType = i; }
  void CreateSteppers();
  void SetMinStep(G4double sstep)  { fMinStep = sstep; }
  void SetFieldValue(G4ThreeVector);
  void SetFieldYValue(G4double);
  G4ThreeVector GetGlobalFieldValue() const { return GetConstantFieldValue(fMagneticField); }
  void UpdateField();
  G4FieldManager* GetLocalFieldManager() { return fLocalFieldManager; }

protected:
  G4FieldManager* GetGlobalFieldManager();
  G4ThreeVector   GetConstantFieldValue(G4MagneticField* ) const;
  G4FieldManager*         fFieldManager;
  G4FieldManager*         fLocalFieldManager;
  G4ChordFinder*          fChordFinder;
  G4ChordFinder*          fLocalChordFinder;
  G4Mag_UsualEqRhs*       fEquation;
  G4Mag_UsualEqRhs*       fLocalEquation;
  G4MagneticField*        fMagneticField;
  G4MagneticField*        fLocalMagneticField;
  G4MagIntegratorStepper* fStepper;
  G4MagIntegratorStepper* fLocalStepper;
  G4int                   fStepperType;
  G4double                fMinStep;
  //muFieldMessenger*      fFieldMessenger;
};
#endif
