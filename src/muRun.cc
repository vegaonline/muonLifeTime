//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muRun.cc                              *
//* ****************************************************
//  $Id: muRun.cc Feb 02, 2019 11:47:08Z vega $

#include "muRun.hh"

muRun::muRun() : G4Run() {

}

muRun::~muRun() {

}


void muRun::RecordEvent(const G4Event* ) {
  ++fNEvt;
}
