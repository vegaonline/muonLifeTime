//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muDetectorConstruction.hh                   *
//* ****************************************************
//  $Id: muDetectorConstruction.hh Feb 01, 2019 15:25:08Z vega $
#ifndef muDetectorConstruction_H
#define muDetectorConstruction_H 1

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4AssemblyVolume.hh"
#include "G4AssemblyStore.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4FieldManager.hh"
#include "G4NistManager.hh"
#include "G4RunManager.hh"
#include "globals.hh"
#include <vector>

#include "muDetectorMessenger.hh"
#include "muMagneticField.hh"

class G4LogicalVolume;
class G4Material;
class muDetectorMessenger;
class muMagneticField;
class G4GenericMessenger;

class muDetectorConstruction : public G4VUserDetectorConstruction{
public:
  muDetectorConstruction();
  ~muDetectorConstruction();

public:
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

  void SetDetectorLength(G4double);
  void SetDetectorWidth(G4double);
  void SetDetectorThickness(G4double);
  void SetDetectorMaterial(G4String&);

  void SetMagnetPlateLength(G4double);
  void SetMagnetPlateWidth(G4double);
  void SetMagnetPlateThickness(G4double);
  void SetMagnetPlateGap(G4double);
  void SetMagnetPlateMaterial(G4String&);

  muDetectorMessenger* fDetMessenger;

  void InitMeasurement();
  void UpdateGeometry();
  void PrintParameters();

  G4int GetNumberOfDetectors()                  const { return fNumDetector; }
  G4double GetDetectorLength()                  const { return fDetectorLength; }
  G4double GetDetectorWidth()                   const { return fDetectorWidth; }
  G4double GetDetectorThickness()               const { return fDetectorThickness; }
  G4Material* GetDetectorMaterial()                   { return fDetectorMaterial; }
  G4int GetLogicDetectorNumber()                const { return fLogicDetector.size(); }
  G4LogicalVolume* GetLogicDetector(G4int iTag)       { return fLogicDetector[iTag]; }
  G4double GetMagnetLength()                    const { return fMagnetPlateLength; }
  G4double GetMagnetWidth()                     const { return fMagnetPlateWidth; }
  G4double GetMagnetThickness()                 const { return fMagnetPlateThickness; }
  G4Material* GetMagnetPlateMaterial()                { return fMagnetPlateMaterial; }
  G4LogicalVolume* GetLogicMagnet()                   { return fLogicMagnet; }

private:
  void DefineMaterials();
  G4VPhysicalVolume* ConstructVolumes();

private:
  static G4ThreadLocal muMagneticField* fMagneticField;
  static G4ThreadLocal G4FieldManager* fFieldMgr;

  G4int fNumDetector;  // Total number of detectors of same size used
  std::vector<G4int>    fDetPlaced;    // position of i-th detector : 0 if above magnet and 1 if below magnet
  std::vector<G4double> fDistDetMagnet; // Distance of i-th detector from center of magnet

  G4double fDetectorLength; // Length of each detector
  G4double fDetectorWidth;  // Width of each detector
  G4double fDetectorThickness; // Thickness of each detector
  G4Material* fDetectorMaterial;
  std::vector<G4LogicalVolume*> fLogicDetector;
  G4double fSetupTopHt;

  G4double fMagnetPlateLength;  // Length of Magnet
  G4double fMagnetPlateWidth;   // Width of Magnet
  G4double fMagnetPlateThickness;   // Thickness of Magnet
  G4double fMagnetPlateGap;     // Gap of Magnet
  G4Material* fMagnetPlateMaterial;
  G4LogicalVolume* fLogicMagnet;
  
  G4ThreeVector Ta, Tm;
  G4RotationMatrix Ra, Rm;

  G4double fWorldLength;
  G4double fWorldWidth;
  G4double fWorldThickness;
  G4Box*  sWorld;
  G4Material* fWorldMaterial;
  G4LogicalVolume* lWorld;
  G4VPhysicalVolume* fPhysicalWorld;

  G4Material* air;
  G4Material* scintillator;
  G4Material* steel;

};

#endif
