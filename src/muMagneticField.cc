//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muMagneticField.cc                          *
//* ****************************************************
//  $Id: muMagneticField.cc Feb 02, 2019 10:47:08Z vega $

#include "muMagneticField.hh"

muMagneticField::muMagneticField()
  : fFieldManager(0), fLocalFieldManager(0), fChordFinder(0), fLocalChordFinder(0),
    fEquation(0), fLocalEquation(0), fMagneticField(0), fLocalMagneticField(0),
    fStepper(0), fLocalStepper(0) //, fFieldMessenger(0)
    {
      fMagneticField      = new G4UniformMagField(G4G4ThreeVector(0.0, 1.0*tesla, 0.0));
      fLocalMagneticField = new G4UniformMagField(G4G4ThreeVector(0.0, 1.0*tesla, 0.0));
      //fFieldMessenger = new muFieldMessenger(this);
      fEquation = new G4G4G4Mag_UsualEqRhs(fMagneticField);
      fLocalEquation = new G4G4G4G4Mag_UsualEqRhs(fLocalMagneticField);
      fMinStep = 0.01*mm; // minimal step of 1 mm is default
      fStepperType = 4;   // ClassicalRK4 is default stepper
      fFieldManager = GetGlobalFieldManager();
      fLocalFieldManager = new G4FieldManager();
      UpdateField();
}

muMagneticField::~muMagneticField(){
  delete fMagneticField;
  delete fChordFinder;
  delete fStepper;
  //delete fFieldMessenger;
}

void muMagneticField::UpdateField() {
  // 1. First clean up of previous state
  delete fChordFinder;
  fChordFinder = nullptr;
  delete fLocalChordFinder;
  fLocalChordFinder = nullpr;
  G4cout<<"muMagneticField::UpdateField> The minimal step is equal to " << fMinStep/mm << " mm" << G4endl;
  G4cout<<"                              Stepper Type chosen = "        << fStepperType         << G4endl;

  // 2. This also deletes previous ones
  CreateSteppers();

  // 3. Create the chord finder(s)
  fChordFinder      = new G4ChordFinder(fMagneticField, fMinStep, fStepper);
  fLocalChordFinder = new G4ChordFinder(fLocalMagneticField, fMinStep, fLocalStepper);
  fFieldManager->SetChordFinder(fChordFinder);
  fLocalFieldManager->SetChordFinder(fLocalChordFinder);

  // 4. Ensure that the field is updated (in field manager & equation)
  fFieldManager->SetDetectorField(fMagneticField);
  fLocalFieldManager->SetDetectorField(fLocalMagneticField);
}

void muMagneticField::CreateSteppers() {
  delete fStepper;
  fStepper = nullptr;
  delete fLocalStepper;
  fLocalStepper = nullptr;
  switch (fStepperType) {
    case 0:
      fStepper      = new G4ExplicitEuler(fEquation);
      fLocalStepper = new G4ExplicitEuler(fLocalEquation);
      G4cout << "G4ExplicitEuler is called " << G4endl;
      break;
    case 1:
      fStepper      = new G4ImplicitEuler(fEquation);
      fLocalStepper = new G4ImplicitEuler(fLocalEquation);
      G4cout << "G4ImplicitEuler is called " << G4endl;
      break;
    case 2:
      fStepper      = new G4SimpleRunge(fEquation);
      fLocalStepper = new G4SimpleRunge(fLocalEquation);
      G4cout << "G4SimpleRunge is called " << G4endl;
      break;
    case 3:
      fStepper      = new G4SimpleHeum(fEquation);
      fLocalStepper = new G4SimpleHeum(fLocalEquation);
      G4cout << "G4SimpleHeum is called " << G4endl;
      break;
    case 4:
      fStepper      = new G4ClassicalRK4(fEquation);
      fLocalStepper = new G4ClassicalRK4(fLocalEquation);
      G4cout << "G4ClassicalRK4 is called " << G4endl;
      break;
      case 5:
      fStepper = new G4HelixExplicitEuler( fEquation );
      fLocalStepper = new G4HelixExplicitEuler( fLocalEquation );
      G4cout << "G4HelixExplicitEuler is called" << G4endl;
      break;
    case 6:
      fStepper = new G4HelixImplicitEuler( fEquation );
      fLocalStepper = new G4HelixImplicitEuler( fLocalEquation );
      G4cout << "G4HelixImplicitEuler is called" << G4endl;
      break;
    case 7:
      fStepper = new G4HelixSimpleRunge( fEquation );
      fLocalStepper = new G4HelixSimpleRunge( fLocalEquation );
      G4cout << "G4HelixSimpleRunge is called" << G4endl;
      break;
    case 8:
      fStepper = new G4CashKarpRKF45( fEquation );
      fLocalStepper = new G4CashKarpRKF45( fLocalEquation );
      G4cout << "G4CashKarpRKF45 is called" << G4endl;
      break;
    case 9:
      fStepper = new G4RKG3_Stepper( fEquation );
      fLocalStepper = new G4RKG3_Stepper( fLocalEquation );
      G4cout << "G4RKG3_Stepper is called" << G4endl;
      break;
    default: fStepper = 0;
  }
}

void muMagneticField::SetFieldYValue(G4double fieldStrength){
  G4ThreeVector fieldSetVec(0.0, 0.0, fieldStrength);
  SetFieldValue( fieldSetVec );
}

void muMagneticField::SetFieldValue(G4ThreeVector fieldVector){
  if(fMagneticField) delete fMagneticField;
  if(fieldVector != G4ThreeVector(0.,0.,0.)) {
    fMagneticField = new  G4UniformMagField(fieldVector);
  } else {
    // If the new field's value is Zero, then
    // setting the pointer to zero ensures
    // that it is not used for propagation.
    fMagneticField = 0;
  }
  // Either
  //   - UpdateField() to reset all (ChordFinder, Equation);
  // UpdateField();
  //     or simply update the field manager & equation of motion  with pointer to new field
  GetGlobalFieldManager()->SetDetectorField(fMagneticField);
  fEquation->SetFieldObj( fMagneticField );
}

void muMagneticField::SetLocalFieldValue(G4ThreeVector fieldVector){
  if(fLocalMagneticField) delete fLocalMagneticField;

  if(fieldVector != G4ThreeVector(0.,0.,0.)) {
    fLocalMagneticField = new  G4UniformMagField(fieldVector);
  } else  {
    // If the new field's value is Zero, then
    // setting the pointer to zero ensures
    // that it is not used for propagation.
    fLocalMagneticField = 0;
  }
  // Either
  //   - UpdateField() to reset all (ChordFinder, Equation);
  // UpdateField();
  //     or simply update the field manager & equation of motion
  //     with pointer to new field
  GetLocalFieldManager()->SetDetectorField(fLocalMagneticField);
  fEquation->SetFieldObj( fLocalMagneticField );
}

G4FieldManager* muMagneticField::GetGlobalFieldManager(){
  return G4TransportationManager::GetTransportationManager()->GetFieldManager();
}

G4ThreeVector F03FieldSetup::GetConstantFieldValue(G4MagneticField* magneticField) const {
  if ( ! magneticField ) return G4ThreeVector();

  static G4double fieldValue[6],  position[4];
  position[0] = position[1] = position[2] = position[3] = 0.0;

  magneticField->GetFieldValue( position, fieldValue);
  G4ThreeVector fieldVec(fieldValue[0], fieldValue[1], fieldValue[2]);

  return fieldVec;
}
