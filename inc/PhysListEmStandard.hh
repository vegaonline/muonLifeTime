//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file PhysListEmStandard.hh                                 *
//* ****************************************************
//  $Id: PhysListEmStandard.hh Feb 13, 2019 15:25:08Z vega $
#ifndef PhysListEmStandard_H
#define PhysListEmStandard_H 1

#include "G4VPhysicsConstructor.hh"
#include "G4ParticleDefinition.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4Proton.hh"

#include "G4ProcessManager.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4EmParameters.hh"
#include "G4SystemOfUnits.hh"

#include "globals.hh"

class PhysListEmStandard : public G4VPhysicsConstructor {
public:
  PhysListEmStandard(const G4String& name = "standard");
  ~PhysListEmStandard();
public:
  virtual void ConstructParticle(); // dummy for Physics
  virtual void ConstructProcess();
  // invoked in Construct(). Each process will be registered to manager for each particle type
};

#endif
