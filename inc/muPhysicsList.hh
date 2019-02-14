//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muPhysicsList.hh                            *
//* ****************************************************
//  $Id: muPhysicsList.hh Feb 13, 2019 15:25:08Z vega $
#ifndef muPhysicsList_H
#define muPhysicsList_H 1

#include "muNuclearBuilder.hh"
#include "muPhysListEmStandard.hh"

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmParameters.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4SystemOfUnits.hh"

#include "globals.hh"

class G4VPhysicsConstructor;

class muPhysicsList : public G4VModularPhysicsList {
public:
  muPhysicsList();
  ~muPhysicsList();

  virtual void ConstructParticle();
  virtual void ConstructProcess();
  void AddPhysicsList(const G4String& name);

private:
  G4VPhysicsConstructor* fEmPhysicsList;
  G4G4VPhysicsConstructor* fMuNuclPhysicsList;
  G4String fEmName;


};

#endif
