//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muNuclearBuilder.hh                         *
//* ****************************************************
//  $Id: muNuclearBuilder.hh Feb 14, 2019 09:25:08Z vega $
#ifndef muNuclearBuilder_H
#define muNuclearBuilder_H 1

#include "G4VPhysicsConstructor.hh"
#include "G4ParticleDefinition.hh"
#include "G4MuonPlus.hh"
#include "G4MUonMinus.hh"
#include "G4ProcessManager.hh"
#include "G4MuonNuclearProcess.hh"
#include "G4MuonVDNuclearModel.hh"
#include "globals.hh"

class muNuclearBuilder : public G4VPhysicsConstructor {
public:
  muNuclearBuilder(const G4String& name = "muNucl");
  ~muNuclearBuilder();

public:
  virtual void ConstructParticle() {}; // dummy
  virtual void ConstructProcess();

};

#endif
