//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muDetectorConstruction.cc                   *
//* ****************************************************
//  $Id: muDetectorConstruction.cc Feb 01, 2019 15:25:08Z vega $

#include "muDetectorConstruction.hh"

//G4ThreadLocal muMagneticField* muDetectorConstruction::fMagneticField = 0;
//G4ThreadLocal G4FieldManager* muDetectorConstruction::fFieldMgr = 0;
G4ThreadLocal G4GlobalMagFieldMessenger* muDetectorConstruction::fMagFieldMessenger = 0;

muDetectorConstruction::muDetectorConstruction()
: G4VUserDetectorConstruction(), fDetectorMaterial(nullptr), fLogicDetector(), fDetLogicL(nullptr), fMagneticField(),
  fMagnetPlateMaterial(nullptr), fMagPlateL(nullptr), fDetMessenger(nullptr), fPhysicalWorld(nullptr),
  D0PV(nullptr), D1PV(nullptr), fVisAttributes() {
    fMagneticField.Put(0);
    InitMeasurement();
    DefineMaterials();
    fMagFileName = "magField3D.tab";
    fDetMessenger = new muDetectorMessenger(this);
  }

muDetectorConstruction::~muDetectorConstruction() {
    delete fDetMessenger;
    for (auto visAttrib: fVisAttributes) delete visAttrib;
}

void muDetectorConstruction::InitMeasurement(){
  // X is length, Y is thickness and Z is width
  G4double fTmpGap      =    0.0;  // dynamic calculation of height of detector i from center of the magnet
  fAirGap               =    1.0 * mm;   // Air Gap
  fMagnetPlateLength    = 500.0 * mm;   // 1000.0 * mm;  because two tiles
  fMagnetPlateWidth     = 250.0 * mm;    //1000.0 * mm;  because four tiles
  fMagnetPlateThickness =   55.0 * mm;   // 56.0 * mm;
  fMagnetPlateStandLen  =  100.0 * mm;   // X
  fMagnetPlateStandThk  =   20.0 * mm;   // Y
  fMagnetPlateStandWid  =   40.0 * mm;   // Z
  fMagnetPlateGap       =   fMagnetPlateStandThk;
  fDetectorLength       =  500.0 * mm;
  fDetectorWidth        =  500.0 * mm;
  fDetectorThickness    =   60.0 * mm;
  fCoilLength           = 500.0 * mm;
  fCoilThickness        =  15.0 * mm;
  fCoilHeight           = 2.0 * (fMagnetPlateThickness + fMagnetPlateStandThk); //150.0 * mm;
  fCoilWidth            = 2.0 * (fMagnetPlateWidth - fCoilThickness); //470.0 * mm;
  fMagnetPlateSlotLen   =   fCoilLength + 2.0 * fAirGap;  // slot length
  fMagnetPlateSlotWidth =   fCoilWidth + 2.0 * fAirGap;  // slot width
  fMagnetPlateSlotThk   =   fMagnetPlateThickness + fMagnetPlateStandThk;
  fNumDetector          =    2;
  fDetPlaced.push_back(1);
  fTmpGap = 50.0 * mm + fMagnetPlateThickness + 0.5 * fMagnetPlateGap;  // D_detMag + H_MagPlate + 0.5 * MagPlatesGap
  fTmpGap += 0.5 * fDetectorThickness;     // this shows distance of centre of det from center of magnet
  fDistDetMagnet.push_back(fTmpGap);  // Det#1 above magnet
  fDetPlaced.push_back(-1);
  fTmpGap = 150.0 * mm + fMagnetPlateThickness + 0.5 * fMagnetPlateGap;
  fTmpGap += 0.5 * fDetectorThickness;    // this shows distance of centre of det from center of magnet
  fDistDetMagnet.push_back(fTmpGap);  // Det#2 below magnet
  G4double fMaxY = 0.0;
  for (G4int ii = 0; ii < fNumDetector; ii++) {
    fMaxY = std::max(fMaxY, fDistDetMagnet[ii] + 0.5 * fDetectorThickness);  // as fDistDetMagnet is at centre of det
  }
  fWorldLength = 1.4 * std::max(fDetectorLength, fMagnetPlateLength);
  fWorldWidth = 1.4 * std::max(fDetectorWidth, fMagnetPlateWidth);
  fWorldThickness = 1.4 * (2.0 * fMaxY);   // fMaxY is computed for any one side if it is maximum in +Y or -Y. So 2X for -Y to +Y
}

G4VPhysicalVolume* muDetectorConstruction::Construct() {
  return ConstructVolumes();
}

void muDetectorConstruction::ConstructSDandField() {
  //auto sdManager = G4SDManager::GetSDMpointer();
  /*   // working
  fMagneticField = new muMagneticField();
  G4ThreeVector fieldValue = G4ThreeVector();
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  G4AutoDelete::Register(fMagFieldMessenger);
  */

  fMagFileName = "GenerateMagFieldTable/magField3D.tab";

#if MAG
  if (fMagneticField.Get() == 0) {
    G4MagneticField* fMagF = new muMagTabulatedField3D(fMagFileName);
    fMagneticField.Put(fMagF);

    G4FieldManager* pFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();

    pFieldMgr->SetDetectorField(fMagneticField.Get());
    pFieldMgr->CreateChordFinder(fMagneticField.Get());
    G4AutoDelete::Register(fMagFieldMessenger);
  }
#endif
}

void muDetectorConstruction::DefineMaterials() {
  auto nistMan = G4NistManager::Instance();

  nistMan->FindOrBuildMaterial("G4_AIR");   // Building Air

  // polyvinyltoluene C9H10 scintillator
  nistMan->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  // stainless steel
  nistMan->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  // Copper
  nistMan->FindOrBuildMaterial("G4_Cu");

  air               = G4Material::GetMaterial("G4_AIR");
  scintillator      = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  steel             = G4Material::GetMaterial("G4_STAINLESS-STEEL");
  copper            = G4Material::GetMaterial("G4_Cu");

  fWorldMaterial    = air;
  fMagnetPlateMaterial   = steel;
  fDetectorMaterial = scintillator;
  fCoilMaterial = copper;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

G4VPhysicalVolume* muDetectorConstruction::ConstructVolumes(){
  // cleaning up old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  RM = new G4RotationMatrix();

  InitMeasurement(); // initialize measurements.
  // Material Construction phase
  DefineMaterials();

  fWorldMaterial = air;
  fMagnetPlateMaterial = steel;
  fDetectorMaterial = scintillator;
  fCoilMaterial = copper;

  // The World --------------------------------------------
  auto sWorld = new G4Box("WorldLabS", 0.5 * fWorldLength, 0.5 * fWorldThickness, 0.5 * fWorldWidth);
  auto lWorld  = new G4LogicalVolume(sWorld, fWorldMaterial, "WorldLabL");
  fPhysicalWorld = new G4PVPlacement(0, G4ThreeVector(), lWorld, "WorldLabP", 0, false, 0);

  // Detectors --------------------------------------------
  fSetupTopHt = -99999.0;
  fSetupBotHt = 99999.0;

  for (G4int ii = 0; ii < fNumDetector; ii++) {  // Define each detector with respect to the magnet
    auto detname = "Detector" + std::to_string(ii);  // if there is error use std::ostringstream ss; ss << ii; detname = ss.str()
    auto detectorBox = new G4Box(detname, 0.5 * fDetectorLength, 0.5 * fDetectorThickness, 0.5 * fDetectorWidth);
    fDetLogicL = new G4LogicalVolume(detectorBox, fDetectorMaterial, detname);
    fLogicDetector.push_back(fDetLogicL);
    G4double yCoord = fDetPlaced[ii] * fDistDetMagnet[ii] ;
    if (fDetPlaced[ii] > 0) fSetupTopHt = std::max(fSetupTopHt, std::abs(yCoord) + 0.5 * fDetPlaced[ii] * fDetectorThickness);
    if (fDetPlaced[ii] < 0) fSetupBotHt = std::min(fSetupBotHt, -1.0 *(std::abs(yCoord) + 0.5 * fDetPlaced[ii] * fDetectorThickness));
    if (ii == 0)
      D0PV = new G4PVPlacement(0, G4ThreeVector(0.0, yCoord, 0.0), fDetLogicL, detname, lWorld, false, 0);
    if (ii == 1)
      D1PV = new G4PVPlacement(0, G4ThreeVector(0.0, yCoord, 0.0), fDetLogicL, detname, lWorld, false, 0);
    G4cout << " detector #" << (ii+1) << " at (0.0, " << yCoord << ", 0.0)" << G4endl;
  }

  // Magnet  and Coil --------------------------------------------------------------------------
  //  There are two type of tiles one with and the other without tiles.
  auto fMagTilePlateTop = new G4Box("MagTilePlateTopS", 0.5 * fMagnetPlateLength, 0.5 * fMagnetPlateThickness, 0.5 * fMagnetPlateWidth);
  auto fMagTileSlot     = new G4Box("MagTileSlotTopS", 0.5 * fMagnetPlateSlotLen, 0.5 * fMagnetPlateSlotThk, 0.5 * fMagnetPlateSlotWidth);
  auto fMagTileStand    = new G4Box("MagTileStandS", 0.5 * fMagnetPlateStandLen, 0.5 * fMagnetPlateStandThk, 0.5 * fMagnetPlateStandWid);

  auto fMagnetCoilO     = new G4Box("MagnetCoilOut", 0.5 * fCoilLength, 0.5 * fCoilWidth, 0.5 * fCoilHeight);
  auto fMagnetCoilI     = new G4Box("MagnetCoilIn", 0.5 * fCoilLength, 0.5 * (fCoilWidth - fCoilThickness), 0.5 * (fCoilHeight - fCoilThickness));
  G4VSolid* fMagCoilS   = new G4SubtractionSolid("CuCoilS", fMagnetCoilO, fMagnetCoilI, 0, G4ThreeVector(0,0,0));
  fMagnetCoilL = new G4LogicalVolume(fMagCoilS, fCoilMaterial, "MagnetCoil");
  G4cout << fCoilLength << "   " << fCoilWidth << "   " << fCoilThickness << "   " << fCoilHeight << G4endl; //exit(0);

  G4double dx = 0.0, dy = 0.0, dz = 0.0;
  RM->set(0.0, 0.0, 0.0);
  // Plate top without slot but with four stands
  dx = -0.5 * fMagnetPlateLength + 0.5 * fMagnetPlateStandLen;
  dy = -0.5 * fMagnetPlateThickness - 0.5 * fMagnetPlateStandThk;
  dz = -0.5 * fMagnetPlateWidth + 0.5 * fMagnetPlateStandWid;
  TM.setX(dx);  TM.setY(dy); TM.setZ(dz);
  G4VSolid* fMagTopTileS1 = new G4UnionSolid("TileTopStand1", fMagTilePlateTop, fMagTileStand, 0, TM); // Tile top with left most top stand 1
  dy = -1.0 * dy; TM.setY(dy);
  G4VSolid* fMagTopTileSI1 = new G4UnionSolid("TileTopStand1", fMagTilePlateTop, fMagTileStand, 0, TM); // Tile top with left most top stand 1
  dx = -0.5 * fMagnetPlateLength + 0.5 * fMagnetPlateStandLen;
  dy = -0.5 * fMagnetPlateThickness - 0.5 * fMagnetPlateStandThk;
  dz = 0.5 * fMagnetPlateWidth - 0.5 * fMagnetPlateStandWid;
  TM.setX(dx);  TM.setY(dy); TM.setZ(dz);
  G4VSolid* fMagTopTileS12 = new G4UnionSolid("TileTopStand12", fMagTopTileS1, fMagTileStand, 0, TM); // Tile top with left most bot stand 1, 2
  dy = -1.0 * dy; TM.setY(dy);
  G4VSolid* fMagTopTileSI12 = new G4UnionSolid("TileTopStand12", fMagTopTileSI1, fMagTileStand, 0, TM); // Tile top with left most bot stand 1, 2
  dx = 0.5 * fMagnetPlateLength -0.5 * fMagnetPlateStandLen;
  dy = -0.5 *fMagnetPlateThickness - 0.5 * fMagnetPlateStandThk;
  dz = 0.5 * fMagnetPlateWidth - 0.5 * fMagnetPlateStandWid;
  TM.setX(dx);  TM.setY(dy); TM.setZ(dz);
  G4VSolid* fMagTopTileS123 = new G4UnionSolid("TileTopStand123", fMagTopTileS12, fMagTileStand, 0, TM); // Tile top with right most bot stand 1, 2, 3
  dy = -1.0 * dy; TM.setY(dy);
  G4VSolid* fMagTopTileSI123 = new G4UnionSolid("TileTopStand123", fMagTopTileSI12, fMagTileStand, 0, TM); // Tile top with right most bot stand 1, 2, 3
  dx = 0.5 * fMagnetPlateLength - 0.5 * fMagnetPlateStandLen;
  dy = -0.5 * fMagnetPlateThickness - 0.5 * fMagnetPlateStandThk;
  dz = -0.5 * fMagnetPlateWidth + 0.5 * fMagnetPlateStandWid;
  TM.setX(dx);  TM.setY(dy); TM.setZ(dz);
  G4VSolid* fMagTopTileS = new G4UnionSolid("TileTop", fMagTopTileS123, fMagTileStand, 0, TM); // Tile top with right most stand 1, 2, 3, 4
  dy = -1.0 * dy; TM.setY(dy);
  G4VSolid* fMagTopTileIS = new G4UnionSolid("TileTop", fMagTopTileSI123, fMagTileStand, 0, TM); // Tile top with right most stand 1, 2, 3, 4
  fMagPlateTilesL = new G4LogicalVolume(fMagTopTileS, fMagnetPlateMaterial, "MagTile");
  fMagPlateTilesIL = new G4LogicalVolume(fMagTopTileIS, fMagnetPlateMaterial, "MagTileI");

  // Plate top with slot and four stands TYPE: A with slot at North   #2 and #6
  dx = 0.5 * fMagnetPlateSlotLen;
  dy = 0.5 * fMagnetPlateThickness - 0.5 * fMagnetPlateSlotThk;
  dz = -0.5 * fMagnetPlateWidth + 0.5 * fMagnetPlateSlotWidth;
  //G4cout << "dx: " << dx << " dy: " << dy << " dz: " << dz << G4endl; exit(0);
  TM.setX(dx);  TM.setY(dy); TM.setZ(dz);
  G4VSolid* fMagTopTileSlottedNS = new G4SubtractionSolid("TileTopSlotted", fMagTopTileS, fMagTileSlot, 0, TM);
  G4VSolid* fMagTopTileSlottedNIS = new G4SubtractionSolid("TileTopSlottedInv", fMagTopTileIS, fMagTileSlot, 0, TM);
  fMagPlateSlottedTilesNL = new G4LogicalVolume(fMagTopTileSlottedNS, fMagnetPlateMaterial, "MagTileSlottedN");
  fMagPlateSlottedTilesNIL = new G4LogicalVolume(fMagTopTileSlottedNIS, fMagnetPlateMaterial, "MagTileSlottedN");

  // Plate top with slot and four stands TYPE: B with slot at South  #3 and $7
  //dx = 0.5 * fMagnetPlateSlotLen;
  //dy =  -0.5 * fMagnetPlateStandThk; // -0.5 * fMagnetPlateSlotThk + 0.5 * fMagnetPlateThickness;
  dz = 0.5 * fMagnetPlateWidth - 0.5 * fMagnetPlateSlotWidth;
  TM.setX(dx);  TM.setY(dy); TM.setZ(dz);
  G4VSolid* fMagTopTileSlottedSS = new G4SubtractionSolid("TileTopSlotted", fMagTopTileS, fMagTileSlot, 0, TM);
  G4VSolid* fMagTopTileSlottedSIS = new G4SubtractionSolid("TileTopSlotted", fMagTopTileIS, fMagTileSlot, 0, TM);
  fMagPlateSlottedTilesSL = new G4LogicalVolume(fMagTopTileSlottedSS, fMagnetPlateMaterial, "MagTileSlottedS");
  fMagPlateSlottedTilesSIL = new G4LogicalVolume(fMagTopTileSlottedSIS, fMagnetPlateMaterial, "MagTileSlottedS");

  // Assembling the magnet with plates
  auto fMagnetAssembly = new G4AssemblyVolume();

  //------------- TOP ------------------
  // Side -X:1 TOP
  RM->set(0.0, 0.0, 0.0);
  dx = 0.5 * fMagnetPlateLength;
  dy = 0.5 * fMagnetPlateSlotThk;
  dz = 1.5 * fMagnetPlateWidth;
  G4cout << "dx: " << dx << " dy: " << dy << " dz: " << dz << G4endl; //exit(0);
  TM.setX(-1.0 * dx); TM.setY(dy); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesL, TM, RM);     // #1

  // Side -X:9 BOTTOM
  TM.setX(-1.0 * dx); TM.setY(dy - fMagnetPlateThickness - 2.0 * fMagnetPlateGap); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesIL, TM, RM);     // #9

  // Side -X:2 TOP
  dz -= fMagnetPlateWidth;
  TM.setX(-1.0 * dx); TM.setY(dy); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesNL, TM, RM);     // #2

  // Side -X:10 BOTTOM
  TM.setX(-1.0 * dx); TM.setY(dy - fMagnetPlateThickness - 2.0 * fMagnetPlateGap); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesNIL, TM, RM);     // #10

  // Side -X:3 BOTTOM
  dz -= fMagnetPlateWidth;
  TM.setX(-1.0 * dx); TM.setY(dy); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesSL, TM, RM);     // #3

  // Side -X:11 BOTTOM
  TM.setX(-1.0 * dx); TM.setY(dy - fMagnetPlateThickness - 2.0 * fMagnetPlateGap); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesSIL, TM, RM);     // #11

  // Side -X:4 BOTTOM
  dz -= fMagnetPlateWidth;
  TM.setX(-1.0 * dx); TM.setY(dy); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesL, TM, RM);     // #4

  // Side -X:12 BOTTOM
  TM.setX(-1.0 * dx); TM.setY(dy - fMagnetPlateThickness - 2.0 * fMagnetPlateGap); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesIL, TM, RM);     // #12

  // Side +X:5 Top
  dz = dz;
  TM.setX(dx); TM.setY(dy); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesL, TM, RM);     // #5

  // Side +X:13 BOTTOM
  TM.setX(dx); TM.setY(dy - fMagnetPlateThickness - 2.0 * fMagnetPlateGap); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesIL, TM, RM);     // #13

  // Side +X:6 Top
  dz += fMagnetPlateWidth;
  TM.setX(dx); TM.setY(dy); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesSL, TM, RM);     // #6

  // Side +X:14 BOTTOM
  TM.setX(dx); TM.setY(dy - fMagnetPlateThickness - 2.0 * fMagnetPlateGap); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesSIL, TM, RM);     // #14

  // Side +X:7 Top
  dz += fMagnetPlateWidth;
  TM.setX(dx); TM.setY(dy); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesNL, TM, RM);     // #7

  // Side +X:15 BOTTOM
  TM.setX(dx); TM.setY(dy - fMagnetPlateThickness - 2.0 * fMagnetPlateGap); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesNIL, TM, RM);     // #15

  // Side +X:8 Top
  dz += fMagnetPlateWidth;
  TM.setX(dx); TM.setY(dy); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesL, TM, RM);     // #8

  // Side +X:16 BOTTOM
  TM.setX(dx); TM.setY(dy - fMagnetPlateThickness - 2.0 * fMagnetPlateGap); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesIL, TM, RM);     // #16

  // Place the Coil
  TM.setX(0.0); TM.setY(0.0); TM.setZ(0.0);
  fMagnetAssembly->AddPlacedVolume(fMagnetCoilL, TM, RM);

/*
  dx = 0.5 * fMagnetPlateLength;
  dy = 0.5 * fMagnetPlateSlotThk;
  dz = 0.5 * fMagnetPlateWidth;
  TM.setX(dx); TM.setY(dy); TM.setZ(dz);
  RM->set(0.0, 0.0, 0.0);      RM->rotateX(180.0 * deg);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesSL, TM, RM);     // #6
  RM->set(0.0, 0.0, 0.0);
  dx = 0.5 * fMagnetPlateLength;
  dy = 0.5 * fMagnetPlateSlotThk;
  dz = -0.5 * fMagnetPlateWidth;
  TM.setX(dx); TM.setY(dy); TM.setZ(dz);
  RM->rotateZ(180.0*deg);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesNL, TM, RM);     // #7
  dx = 0.5 * fMagnetPlateLength;
  dy = 0.5 * fMagnetPlateSlotThk;
  dz = -0.5 * fMagnetPlateWidth - fMagnetPlateWidth;
  TM.setX(dx); TM.setY(dy); TM.setZ(dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesL, TM, RM);     // #8

  //---------------- BOTTOM --------------------------------

  // Side -X
  TM.setX(-0.5 * fMagnetPlateLength); TM.setY(fMagnetPlateSlotThk); TM.setZ(-(0.5 * fMagnetPlateWidth + fMagnetPlateWidth)); RM->rotateX(180.0*deg);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesL, TM, RM);     // #10
  TM.setX(-0.5 * fMagnetPlateLength); TM.setY(fMagnetPlateSlotThk); TM.setZ(-0.5 * fMagnetPlateWidth); RM->rotateX(180.0*deg);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesL, TM, RM);     // #11
  TM.setX(-0.5 * fMagnetPlateLength); TM.setY(fMagnetPlateSlotThk); TM.setZ(0.5 * fMagnetPlateWidth); RM->rotateY(180.0*deg);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesL, TM, RM);     // #12
  TM.setX(-0.5 * fMagnetPlateLength); TM.setY(fMagnetPlateSlotThk); TM.setZ(0.5 * fMagnetPlateWidth + fMagnetPlateWidth);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesL, TM, RM);     // #13
  // Sid +X
  TM.setX(0.5 * fMagnetPlateLength); TM.setY(fMagnetPlateSlotThk); TM.setZ(-(0.5 * fMagnetPlateWidth + fMagnetPlateWidth));
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesL, TM, RM);     // #14
  TM.setX(0.5 * fMagnetPlateLength); TM.setY(fMagnetPlateSlotThk); TM.setZ(-0.5 * fMagnetPlateWidth);  RM->rotateY(180.0*deg);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesL, TM, RM);     // #15
  TM.setX(0.5 * fMagnetPlateLength); TM.setY(fMagnetPlateSlotThk); TM.setZ(0.5 * fMagnetPlateWidth); RM->rotateY(-180.0*deg);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesL, TM, RM);     // #16
  TM.setX(0.5 * fMagnetPlateLength); TM.setY(fMagnetPlateSlotThk); TM.setZ(0.5 * fMagnetPlateWidth + fMagnetPlateWidth);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesL, TM, RM);     // #9
*/

  TM.setX(0.0); TM.setY(0.0); TM.setZ(0.0);
  fMagnetAssembly->MakeImprint(lWorld, TM, RM); // placing at the origin of the world volume

  auto visAttrib = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
  visAttrib->SetVisibility(false);
  lWorld->SetVisAttributes(visAttrib);
  fVisAttributes.push_back(visAttrib);

  visAttrib = new G4VisAttributes(G4Colour(0.1, 0.4, 0.7));  // for magnet
  fMagPlateTilesL->SetVisAttributes(visAttrib);
  fMagPlateTilesIL->SetVisAttributes(visAttrib);
  visAttrib = new G4VisAttributes(G4Colour(0.4, 0.1, 0.3));  // for magnet
  fMagPlateSlottedTilesNL->SetVisAttributes(visAttrib);
  fMagPlateSlottedTilesNIL->SetVisAttributes(visAttrib);
  visAttrib = new G4VisAttributes(G4Colour(0.1, 0.3, 0.4));  // for magnet
  fMagPlateSlottedTilesSL->SetVisAttributes(visAttrib);
  fMagPlateSlottedTilesSIL->SetVisAttributes(visAttrib);
  fVisAttributes.push_back(visAttrib);

  visAttrib = new G4VisAttributes(G4Color(0.5, 0.4, 0.7));  // G4Colour(0.9, 0.4, 0.6));
  for (auto ij=0; ij < fNumDetector; ij++) {
    fLogicDetector[ij]->SetVisAttributes(visAttrib);
  }
  visAttrib->SetVisibility(false); //---------------------------------------> REMOVE if ALL IS OK in geometry
  fVisAttributes.push_back(visAttrib);

  /*
  auto magBox = new G4Box("MagnetPlate", 0.5 * fMagnetPlateLength, 0.5 * fMagnetPlateThickness, 0.5 * fMagnetPlateWidth);
  auto airBox = new G4Box("AirBlock", 0.5 * fMagnetPlateLength, 0.5 * fMagnetPlateGap, 0.5 * fMagnetPlateWidth);
  fMagPlateL = new G4LogicalVolume(magBox, fMagnetPlateMaterial, "MagnetPlate");  // Plate 1 is at +Y && Plate 2 is at -Y
  fMagAirBlockL = new G4LogicalVolume(airBox, air, "AirGap");  // Plate 1 is at +Y && Plate 2 is at -Y

  auto fMagnetAssembly = new G4AssemblyVolume();
  G4RotationMatrix* Ra;
  G4ThreeVector Ta;
  Ta.setX(0.0); Ta.setY(0.5 * (fMagnetPlateGap + fMagnetPlateThickness)); Ta.setZ(0.0);   // upper Magnet plate
  fMagnetAssembly->AddPlacedVolume(fMagPlateL, Ta, Ra);
  //Ta.setX(0.0); Ta.setY(0.0); Ta.setZ(0.0);            // Air at centre
  //fMagnetAssembly->AddPlacedVolume(fMagAirBlockL, Ta, Ra);  // removed as world volume is filled with air
  Ta.setX(0.0); Ta.setY(-0.5 * (fMagnetPlateGap + fMagnetPlateThickness)); Ta.setZ(0.0); // Lower Magnet Plate
  fMagnetAssembly->AddPlacedVolume(fMagPlateL, Ta, Ra);
  Ta.setX(0.0); Ta.setY(0.0); Ta.setZ(0.0);
  fMagnetAssembly->MakeImprint(lWorld, Ta, Ra); // placing at the origin of the world volume

  auto visAttrib = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
  visAttrib->SetVisibility(false);
  lWorld->SetVisAttributes(visAttrib);
  fVisAttributes.push_back(visAttrib);

  visAttrib = new G4VisAttributes(G4Colour(0.9, 0.4, 0.6));  // for magnet
  fMagPlateL->SetVisAttributes(visAttrib);
  fVisAttributes.push_back(visAttrib);

  //visAttrib = new G4VisAttributes(G4Colour(0.3, 0.9, 0.1));  // for Air inside magnet
  //fMagAirBlockL->SetVisAttributes(visAttrib);
  //fVisAttributes.push_back(visAttrib);

  visAttrib = new G4VisAttributes(G4Colour(0.1, 0.4, 0.7));
  for (auto ij=0; ij < fNumDetector; ij++) {
    fLogicDetector[ij]->SetVisAttributes(visAttrib);
  }
  fVisAttributes.push_back(visAttrib);
*/
  PrintParameters();
  return fPhysicalWorld;
}

void muDetectorConstruction::PrintParameters() {
  G4cout << "\n Total number of detectors: " << fNumDetector << G4endl;
  for (G4int ii = 0; ii < fNumDetector; ii++) {
    G4cout << " Detector #"   << ii << " Length = "  << fDetectorLength/mm << " mm "
           << " Thickness = " << fDetectorThickness/mm << " mm "  << " Width = " << fDetectorWidth/mm << " mm "
           << " Distance from center of the magnet  "<<  (fDetPlaced[ii] * fDistDetMagnet[ii])/mm << " mm."
           << G4endl;
  }
  G4cout << " Magnet Plate"  << " Length = " << fMagnetPlateLength/mm << " mm "
         << " Thickness = " << fMagnetPlateThickness/mm << " mm "
         << " Width = " << fMagnetPlateWidth/mm << " mm." << G4endl;
}

void muDetectorConstruction::SetDetectorLength(G4double value) {
  fDetectorLength = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void muDetectorConstruction::SetDetectorWidth(G4double value){
  fDetectorWidth = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void muDetectorConstruction::SetDetectorThickness(G4double value){
  fDetectorThickness = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void muDetectorConstruction::SetDetectorMaterial(G4String& matChoice) {
  G4Material* pt2Material = G4Material::GetMaterial(matChoice);
  if (pt2Material) fDetectorMaterial = pt2Material;
}

void muDetectorConstruction::SetMagnetPlateLength(G4double value){
  fMagnetPlateLength = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void muDetectorConstruction::SetMagnetPlateWidth(G4double value){
  fMagnetPlateWidth = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void muDetectorConstruction::SetMagnetPlateThickness(G4double value){
  fMagnetPlateThickness = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void muDetectorConstruction::SetMagnetPlateMaterial(G4String& matChoice) {
  G4Material* pt2Material = G4Material::GetMaterial(matChoice);
  if (pt2Material) fMagnetPlateMaterial = pt2Material;
}

void muDetectorConstruction::SetMagnetPlateGap(G4double value){
  fMagnetPlateGap = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void muDetectorConstruction::UpdateGeometry(){
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructVolumes());
}
