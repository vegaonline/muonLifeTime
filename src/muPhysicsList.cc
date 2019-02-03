//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muPhysicsList.cc                                 *
//* ****************************************************
//  $Id: muPhysicsList.cc Feb 02, 2019 22:47:08Z vega $

#include "muPhysicsList.hh"

muPhysicsList::muPhysicsList() : G4VModularPhysicsList(), fEmPhysicsList(0), fMuNuclPhysicsList(0){

  fEmName = G4String("emstandard_opt0");         // EM Physics
  fEmPhysicsList = new G4EmStandardPhysics();

  
}
