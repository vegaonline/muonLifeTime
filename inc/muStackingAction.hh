//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muStackingAction.hh                 *
//* ****************************************************
//  $Id: muStackingAction.hh Feb 28, 2019 21:45:08Z vega $

#ifndef muStackingAction_H
#define muStackingAction_H 1

#include "G4UserStackingAction.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "globals.hh"

class G4Track;

class muStackingAction : public G4UserStackingAction{
public:
  muStackingAction();
  ~muStackingAction();

public:
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
};

#endif
