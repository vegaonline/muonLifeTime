//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muonDetectorConstruction.hh                                 *
//* ****************************************************
//  $Id: muDetectorConstruction.hh Feb 01, 2019 15:25:08Z vega $
#ifndef muDetectorConstruction_H
#define muDetectorConstruction_H 1

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4FieldManager.hh"
#include "G4NistManager.hh"
#include "globals.hh"
#include <vector>

#include "muDetectorMessenger.hh"

class G4LogicalVolume;
class G4Material;
class muDetectorMessenger;

class muDetectorConstruction : public G4G4VUserDetectorConstruction{
public:
  muDetectorConstruction();
  ~muDetectorConstruction();

public:
  virtual G4VPhysicalVolume* Construct();

  void SetDetectorLength(G4double);
  void SetDetectorWidth(G4double);
  void SetDetectorThickness(G4double);
  void SetDetectorMaterial(G4String);

  void SetMagnetPlateLength(G4double);
  void SetMagnetPlateWidth(G4double);
  void SetMagnetPlateThickness(G4double);
  void SetMagnetPlateGap(G4double);
  void SetDetectorMaterial(G4String);
  void SetMagnetPlateGapMaterial(G4String);

  muDetectorMessenger* fDetMessenger;

  void UpdateGeometry();
  void PrintParameters();

  G4int GetNumberOfDetectors()        const { return fNumDetector; }
  G4double GetDetectorLength()        const { return fDetectorLength; }
  G4double GetDetectorWidth()         const { return fDetectorWidth; }
  G4double GetDetectorThickness()     const { return fDetectorThickness; }
  G4Material* GetDetectorMaterial()         { return fDetectorMaterial; }
  G4LogicalVolume* GetLogicDetector()       { return fLogicDetector; }
  G4double GetMagnetLength()          const { return fMagnetPlateLength; }
  G4double GetMagnetWidth()           const { return fMagnetPlateWidth; }
  G4double GetMagnetThickness()       const { return fMagnetPlateThickness; }
  G4Material* GetMagnetMaterial()           { return fMagnetMaterial; }
  G4LogicalVolume* GetLogicMagnet()         { return fLogicMagnet; }

private:
  void DefineMaterials();
  G4VPhysicalVolume* ConstructVolumes();

private:
  G4int fNumDetector;  // Total number of detectors of same size used
  G4int *fDetPlaced;    // position of i-th detector : 0 if above magnet and 1 if below magnet
  G4double *fDistDetMagnet; // Distance of i-th detector from center of magnet

  G4double fDetectorLength; // Length of each detector
  G4double fDetectorWidth;  // Width of each detector
  G4double fDetectorThickness; // Thickness of each detector
  G4Material* fDetectorMaterial;
  G4LogicalVolume* fLogicDetector;

  G4double fMagnetPlateLength;  // Length of Magnet
  G4double fMagnetPlateWidth;   // Width of Magnet
  G4double fMagnetPlateThickness;   // Thickness of Magnet
  G4double fMagnetPlateGap;     // Gap of Magnet
  G4Material* fMagnetMaterial;
  G4LogicalVolume* fLogicMagnet;

  static G4ThreadLocal G4FieldManager* fFieldMgr;
  static G4ThreadLocal muMagneticField* fMagneticField;

  G4double fWorldLength;
  G4double fWorldWidth;
  G4double fWorldThickness;
  G4Box*  sWorld;
  G4Material* fWorldMaterial;
  G4LogicalVolume* lWorld;
  G4VPhysicalVolume* fPhysicalWorld;

};

#endif
