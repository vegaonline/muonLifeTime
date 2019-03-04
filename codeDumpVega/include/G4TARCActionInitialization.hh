/**************************************************************
 * @file           G4TARCActionInitialization.hh
 * @author         Abhijit Bhattacharyya
 * @brief          Action definition
 *************************************************************/

#ifndef G4TARC_ActionInitialization_H
#define G4TARC_ActionInitialization_H

#include "G4VUserActionInitialization.hh"
#include "G4TARCDetectorConstruction.hh"
#include "G4TARCPrimaryGeneratorAction.hh"
#include "G4TARCRunAction.hh"
#include "G4TARCEventAction.hh"
#include "G4TARCSteppingAction.hh"
#include "G4TARCStackingAction.hh"

class G4TARCRunAction;
class G4TARCEventAction;
class G4TARCDetectorConstruction;

class G4TARCDetectorConstruction;
//class G4VSteppingVerbose;

class G4TARCActionInitialization : public G4VUserActionInitialization {
public:
  //G4TARCActionInitialization(G4TARCDetectorConstruction*);
  G4TARCActionInitialization();
  virtual ~G4TARCActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;

  //virtual G4VSteppingVerbose* InitializeSteppingVerbose() const;

private:
  G4TARCDetectorConstruction* fGeomConstruct;
  G4TARCRunAction* const fMasterRunAction;
};

#endif
