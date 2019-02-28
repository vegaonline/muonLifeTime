//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muHistoManager.hh                              *
//* ****************************************************
//  $Id: muHistoManager.hh Feb 28, 2019 15:18:08Z vega $
#ifndef muHistoManager_H
#define muHistoManager_H 1

#include "globals.hh"
#include "g4root.hh"

class muHistoManager{
public:
  muHistoManager();
  ~muHistoManager();

public:
  void Book();
  void Save();
};

#endif
