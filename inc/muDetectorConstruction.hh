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
#include "G4GeometryManager.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4FieldManager.hh"
#include "G4UniformMagField.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4NistManager.hh"
#include "G4RunManager.hh"
#include "G4AutoDelete.hh"
#include "G4Cache.hh"
#include "G4MagneticField.hh"
#include "globals.hh"
#include <vector>

#include "muDetectorMessenger.hh"
#include "muMagneticField.hh"
#include "muMagTabulatedField3D.hh"

class G4LogicalVolume;
class G4AssemblyVolume;
class G4Material;
class muDetectorMessenger;
class muMagneticField;
class G4UniformMagField;
class muMagTabulatedField3D;
class G4GlobalMagFieldMessenger;
class G4GenericMessenger;
class G4VisAttributes;

#define MAG 1                     // Magnetic Field Grid
#define MEASUREVOL 1  // Volume for measurement

class muDetectorConstruction : public G4VUserDetectorConstruction{
public:
  muDetectorConstruction();
  ~muDetectorConstruction();

public:
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

  inline const G4VPhysicalVolume* GetD0PV() const {return D0PV;}
  inline const G4VPhysicalVolume* GetD1PV() const {return D1PV;}

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
  G4double GetTopPosition()                     const { return fSetupTopHt; }
  G4double GetBotPosition()                     const { return fSetupBotHt; }
  G4Material* GetDetectorMaterial()                   { return fDetectorMaterial; }
  G4int GetLogicDetectorNumber()                const { return fLogicDetector.size(); }
  G4LogicalVolume* GetLogicDetector(G4int iTag)       { return fLogicDetector[iTag]; }
  G4double GetMagnetLength()                    const { return fMagnetPlateLength; }
  G4double GetMagnetWidth()                     const { return fMagnetPlateWidth; }
  G4double GetMagnetThickness()                 const { return fMagnetPlateThickness; }
  G4Material* GetMagnetPlateMaterial()                { return fMagnetPlateMaterial; }
  G4LogicalVolume* GetLogicMagnet()                   { return fMagPlateL; }

private:
  void DefineMaterials();
  G4VPhysicalVolume* ConstructVolumes();

private:
  //static G4ThreadLocal muMagneticField* fMagneticField;
  //static G4ThreadLocal G4FieldManager* fFieldMgr;
  static G4ThreadLocal G4GlobalMagFieldMessenger* fMagFieldMessenger;
  //G4Cache<muMagneticField*> fMagneticField;
  G4Cache<G4MagneticField*> fMagneticField;

  G4String fMagFileName = "";

  G4int fNumDetector;  // Total number of detectors of same size used
  std::vector<G4int>    fDetPlaced;    // position of i-th detector : 0 if above magnet and 1 if below magnet
  std::vector<G4double> fDistDetMagnet; // Distance of i-th detector from center of magnet

  G4double fDetectorLength; // Length of each detector
  G4double fDetectorWidth;  // Width of each detector
  G4double fDetectorThickness; // Thickness of each detector
  G4Material* fDetectorMaterial;
  G4LogicalVolume* fDetLogicL;
  G4VPhysicalVolume* D0PV;
  G4VPhysicalVolume* D1PV;
  std::vector<G4LogicalVolume*> fLogicDetector;
  std::vector<G4VisAttributes*> fVisAttributes;
  G4double fSetupTopHt, fSetupBotHt;

  G4double fMagnetPlateLength;  // Length of Magnet
  G4double fMagnetPlateWidth;   // Width of Magnet
  G4double fMagnetPlateThickness;   // Thickness of Magnet
  G4double fMagnetPlateGap;     // Gap of Magnet
  G4double fMagnetPlateSlotLen, fMagnetPlateSlotWidth, fMagnetPlateSlotThk;
  G4double fMagnetPlateStandLen, fMagnetPlateStandThk, fMagnetPlateStandWid;
  G4double fAirGap;
  G4Material* fMagnetPlateMaterial;
  G4LogicalVolume* fMagPlateL; //LogicMagnet;
  G4LogicalVolume* fMagPlateTilesL;
  G4LogicalVolume* fMagPlateTilesIL;
  G4LogicalVolume* fMagPlateSlottedTilesNXML;
  G4LogicalVolume* fMagPlateSlottedTilesNXPL;
  G4LogicalVolume* fMagPlateSlottedTilesNXMIL;   // inverted
  G4LogicalVolume* fMagPlateSlottedTilesNXPIL;   // inverted
  G4LogicalVolume* fMagPlateSlottedTilesSXML;
  G4LogicalVolume* fMagPlateSlottedTilesSXPL;
  G4LogicalVolume* fMagPlateSlottedTilesSXMIL;  // inverted
  G4LogicalVolume* fMagPlateSlottedTilesSXPIL;  // inverted
  G4LogicalVolume* fMagAirBlockL;
  G4LogicalVolume* fMagnetCoilL;
  G4LogicalVolume* fMagnetCoilAirL;

  G4double fCoilLength;
  G4double fCoilThickness;
  G4double fCoilHeight;
  G4double fCoilWidth;
  G4Material* fCoilMaterial;


  G4double fWorldLength;
  G4double fWorldWidth;
  G4double fWorldThickness;
  G4Material* fWorldMaterial;
  G4VPhysicalVolume* fPhysicalWorld;

  G4Material* air;
  G4Material* scintillator;
  G4Material* steel;
  G4Material* copper;

  G4ThreeVector TM;
  G4RotationMatrix* RM;

};

#endif
