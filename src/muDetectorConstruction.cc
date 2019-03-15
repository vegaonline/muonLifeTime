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
  fCoilHeight           = 2.0 * (fMagnetPlateThickness + fMagnetPlateStandThk + fAirGap +1.5 *  fCoilThickness); //150.0 * mm;
  fCoilWidth            = 2.0 * (fMagnetPlateWidth + fCoilThickness); //470.0 * mm;
  fMagnetPlateSlotLen   =   0.5 * fCoilLength + 2.0 * fAirGap;  // slot length       // ------ X ------
  fMagnetPlateSlotWidth =   fCoilThickness + 2.0 * fAirGap;  // slot width        // ------ Z ------
  fMagnetPlateSlotThk   =   fMagnetPlateThickness + fMagnetPlateStandThk;  // ------ Y ------
  fNumDetector          =    2;
  fDetPlaced.push_back(1);
  fTmpGap = 50.0 * mm + fMagnetPlateSlotThk;  // fMagnetPlateThickness + 0.5 * fMagnetPlateGap;  // D_detMag + H_MagPlate + 0.5 * MagPlatesGap
  fTmpGap += 0.5 * fDetectorThickness;     // this shows distance of centre of det from center of magnet
  fDistDetMagnet.push_back(fTmpGap);  // Det#1 above magnet
  fDetPlaced.push_back(-1);
  fTmpGap = 150.0 * mm + fMagnetPlateSlotThk;  // fMagnetPlateThickness + 0.5 * fMagnetPlateGap;
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
  auto fMagTileSlot     = new G4Box("MagTileSlotTopS", 0.5 * fMagnetPlateSlotLen, 0.5 * fMagnetPlateSlotThk,  0.5 * fMagnetPlateSlotWidth);
  auto fMagTileAir     = new G4Box("MagTileAirS", 0.5 * fMagnetPlateSlotLen, 0.5 * fMagnetPlateSlotThk,  0.5 * fMagnetPlateSlotWidth);
  auto fMagTileStand    = new G4Box("MagTileStandS", 0.5 * fMagnetPlateStandLen, 0.5 * fMagnetPlateStandThk, 0.5 * fMagnetPlateStandWid);

  auto fMagnetCoilO     = new G4Box("MagnetCoilOut", 0.5 * fCoilLength, 0.5 * (fCoilHeight + 2.0 * fAirGap), 0.5 * fCoilWidth);
  auto fMagnetCoilI     = new G4Box("MagnetCoilIn", 0.5 * fCoilLength, 0.5 * (fCoilHeight - 2.0 * fCoilThickness), 0.5 * (fCoilWidth - 2.0 * fCoilThickness));
  G4VSolid* fMagCoilS   = new G4SubtractionSolid("CuCoilS", fMagnetCoilO, fMagnetCoilI, 0, G4ThreeVector(0, 0, 0));
  fMagnetCoilL          = new G4LogicalVolume(fMagCoilS, fCoilMaterial, "MagnetCoil");
  fMagnetCoilAirL       = new G4LogicalVolume(fMagnetCoilI, fWorldMaterial, "MagnetCoil");

  G4double dx = 0.0, dy = 0.0, dz = 0.0;
  RM->set(0.0, 0.0, 0.0);
  // Plate top without slot but with four stands
  dx = -0.5 * fMagnetPlateLength     + 0.5 * fMagnetPlateStandLen;
  dy = -0.5 * fMagnetPlateThickness  - 0.5 * fMagnetPlateStandThk;
  dz = -0.5 * fMagnetPlateWidth      + 0.5 * fMagnetPlateStandWid;

  TM.setX(dx);  TM.setY(dy); TM.setZ(dz);
  G4VSolid* fMagTopTileS1 = new G4UnionSolid("TileTopStand1", fMagTilePlateTop, fMagTileStand, 0, TM); // Tile top with left most top stand 1

  TM.setX(dx);  TM.setY(-1.0 * dy);  TM.setZ(dz);
  G4VSolid* fMagTopTileSI1 = new G4UnionSolid("TileTopStandI1", fMagTilePlateTop, fMagTileStand, 0, TM); // Tile top with left most top stand 1

  TM.setX(dx);  TM.setY(dy); TM.setZ(-1.0 * dz);
  G4VSolid* fMagTopTileS12 = new G4UnionSolid("TileTopStand12", fMagTopTileS1, fMagTileStand, 0, TM); // Tile top with left most bot stand 1, 2

  TM.setX(dx); TM.setY(-1.0 * dy);  TM.setZ(-1.0 * dz);
  G4VSolid* fMagTopTileSI12 = new G4UnionSolid("TileTopStandI12", fMagTopTileSI1, fMagTileStand, 0, TM); // Tile top with left most bot stand 1, 2

  TM.setX(-1.0 * dx);  TM.setY(dy); TM.setZ(-1.0 * dz);
  G4VSolid* fMagTopTileS123 = new G4UnionSolid("TileTopStand123", fMagTopTileS12, fMagTileStand, 0, TM); // Tile top with right most bot stand 1, 2, 3

  TM.setX(-1.0 * dx);  TM.setY(-1.0 * dy);  TM.setZ(-1.0 * dz);
  G4VSolid* fMagTopTileSI123 = new G4UnionSolid("TileTopStandI123", fMagTopTileSI12, fMagTileStand, 0, TM); // Tile top with right most bot stand 1, 2, 3

  TM.setX(-1.0 * dx);  TM.setY(dy); TM.setZ(dz);
  G4VSolid* fMagTopTileS = new G4UnionSolid("TileTop", fMagTopTileS123, fMagTileStand, 0, TM); // Tile top with right most stand 1, 2, 3, 4

  TM.setX(-1.0 * dx);  TM.setY(-1.0 * dy);  TM.setZ(dz);
  G4VSolid* fMagTopTileIS = new G4UnionSolid("TileTopI", fMagTopTileSI123, fMagTileStand, 0, TM); // Tile top with right most stand 1, 2, 3, 4

  fMagPlateTilesL = new G4LogicalVolume(fMagTopTileS, fMagnetPlateMaterial, "MagTile");
  fMagPlateTilesIL = new G4LogicalVolume(fMagTopTileIS, fMagnetPlateMaterial, "MagTileI");

  // Plate top with slot and four stands TYPE: A with slot at North   #2 and #6
  dx = 0.5 * fMagnetPlateSlotLen;
  dy = 0.5 * fMagnetPlateThickness - 0.5 * fMagnetPlateSlotThk;
  dz = -0.5 * fMagnetPlateWidth + 0.5 * fMagnetPlateSlotWidth;
  TM.setX(dx-fAirGap);  TM.setY(dy); TM.setZ(dz);
  G4VSolid* fMagTopTileSlottedNS = new G4SubtractionSolid("TileTopSlotted", fMagTopTileS123, fMagTileSlot, 0, TM);
  G4VSolid* fMagTopTileSlottedNIS = new G4SubtractionSolid("TileTopSlottedInv", fMagTopTileSI123, fMagTileSlot, 0, TM);
  fMagPlateSlottedTilesNXML = new G4LogicalVolume(fMagTopTileSlottedNS, fMagnetPlateMaterial, "MagTileSlottedXMN");
  fMagPlateSlottedTilesNXMIL = new G4LogicalVolume(fMagTopTileSlottedNIS, fMagnetPlateMaterial, "MagTileSlottedXMN");
  TM.setX(-1.0 * dx + fAirGap);
  fMagTopTileSlottedNS = new G4SubtractionSolid("TileTopSlotted", fMagTopTileS123, fMagTileSlot, 0, TM);
  fMagTopTileSlottedNIS = new G4SubtractionSolid("TileTopSlottedInv", fMagTopTileSI123, fMagTileSlot, 0, TM);
  fMagPlateSlottedTilesNXPL = new G4LogicalVolume(fMagTopTileSlottedNS, fMagnetPlateMaterial, "MagTileSlottedXPN");
  fMagPlateSlottedTilesNXPIL = new G4LogicalVolume(fMagTopTileSlottedNIS, fMagnetPlateMaterial, "MagTileSlottedXPN");

  // Plate top with slot and four stands TYPE: B with slot at South  #3 and $7
  dz = 0.5 * fMagnetPlateWidth - 0.5 * fMagnetPlateSlotWidth;
  TM.setX(dx - fAirGap);  TM.setY(dy); TM.setZ(dz);
  G4VSolid* fMagTopTileSlottedSS = new G4SubtractionSolid("TileTopSlotted", fMagTopTileS123, fMagTileSlot, 0, TM);
  G4VSolid* fMagTopTileSlottedSIS = new G4SubtractionSolid("TileTopSlotted", fMagTopTileSI123, fMagTileSlot, 0, TM);
  fMagPlateSlottedTilesSXML = new G4LogicalVolume(fMagTopTileSlottedSS, fMagnetPlateMaterial, "MagTileSlottedS");
  fMagPlateSlottedTilesSXMIL = new G4LogicalVolume(fMagTopTileSlottedSIS, fMagnetPlateMaterial, "MagTileSlottedS");
  TM.setX(-1.0 * dx + fAirGap);
  fMagTopTileSlottedSS = new G4SubtractionSolid("TileTopSlotted", fMagTopTileS123, fMagTileSlot, 0, TM);
  fMagTopTileSlottedSIS = new G4SubtractionSolid("TileTopSlotted", fMagTopTileSI123, fMagTileSlot, 0, TM);
  fMagPlateSlottedTilesSXPL = new G4LogicalVolume(fMagTopTileSlottedSS, fMagnetPlateMaterial, "MagTileSlottedS");
  fMagPlateSlottedTilesSXPIL = new G4LogicalVolume(fMagTopTileSlottedSIS, fMagnetPlateMaterial, "MagTileSlottedS");

  // Assembling the magnet with plates
  auto fMagnetAssembly = new G4AssemblyVolume();

  //------------- TOP ------------------
  // Side -X:1 TOP
  RM->set(0.0, 0.0, 0.0);
  dx = 0.5 * fMagnetPlateLength;
  dy = 0.5 * fMagnetPlateSlotThk;
  dz = 1.5 * fMagnetPlateWidth;

  TM.setX(-1.0 * dx); TM.setY(dy); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesL, TM, 0);     // #1

  // Side -X:9 BOTTOM
  TM.setX(-1.0 * dx); TM.setY(dy - fMagnetPlateThickness - 2.0 * fMagnetPlateGap); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesIL, TM, 0);     // #9

  // Side -X:2 TOP
  dz -= fMagnetPlateWidth;
  TM.setX(-1.0 * dx); TM.setY(dy); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesNXML, TM, 0);     // #2

  // Side -X:10 BOTTOM
  TM.setX(-1.0 * dx); TM.setY(dy - fMagnetPlateThickness - 2.0 * fMagnetPlateGap); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesNXMIL, TM, 0);     // #10


  // Side -X:3 BOTTOM
  dz -= fMagnetPlateWidth;
  TM.setX(-1.0 * dx); TM.setY(dy); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesSXML, TM, 0);     // #3

  // Side -X:11 BOTTOM
  TM.setX(-1.0 * dx); TM.setY(dy - fMagnetPlateThickness - 2.0 * fMagnetPlateGap); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesSXMIL, TM, 0);     // #11

  // Side -X:4 BOTTOM
  dz -= fMagnetPlateWidth;
  TM.setX(-1.0 * dx); TM.setY(dy); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesL, TM, 0);     // #4

  // Side -X:12 BOTTOM
  TM.setX(-1.0 * dx); TM.setY(dy - fMagnetPlateThickness - 2.0 * fMagnetPlateGap); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesIL, TM, 0);     // #12

  // Side +X:5 Top
  dz = dz;
  TM.setX(dx); TM.setY(dy); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesL, TM, 0);     // #5

  // Side +X:13 BOTTOM
  TM.setX(dx); TM.setY(dy - fMagnetPlateThickness - 2.0 * fMagnetPlateGap); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesIL, TM, 0);     // #13

  // Side +X:6 Top
  dz += fMagnetPlateWidth;
  TM.setX(dx); TM.setY(dy); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesSXPL, TM, 0);     // #6

  // Side +X:14 BOTTOM
  TM.setX(dx); TM.setY(dy - fMagnetPlateThickness - 2.0 * fMagnetPlateGap); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesSXPIL, TM, 0);     // #14

  // Side +X:7 Top
  dz += fMagnetPlateWidth;
  TM.setX(dx); TM.setY(dy); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesNXPL, TM, 0);     // #7

  // Side +X:15 BOTTOM
  TM.setX(dx); TM.setY(dy - fMagnetPlateThickness - 2.0 * fMagnetPlateGap); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateSlottedTilesNXPIL, TM, 0);     // #15

  // Side +X:8 Top
  dz += fMagnetPlateWidth;
  TM.setX(dx); TM.setY(dy); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesL, TM, 0);     // #8

  // Side +X:16 BOTTOM
  TM.setX(dx); TM.setY(dy - fMagnetPlateThickness - 2.0 * fMagnetPlateGap); TM.setZ(-1.0 * dz);
  fMagnetAssembly->AddPlacedVolume(fMagPlateTilesIL, TM, 0);     // #16

  // Place the Coil
  TM.setX(0.0); TM.setY(-fAirGap); TM.setZ(0.0);
  fMagnetAssembly->AddPlacedVolume(fMagnetCoilL, TM, 0);
  fMagnetAssembly->AddPlacedVolume(fMagnetCoilAirL, TM, 0);

  TM.setX(0.0); TM.setY(0.0); TM.setZ(0.0);
  fMagnetAssembly->MakeImprint(lWorld, TM, 0); // placing at the origin of the world volume

  auto visAttrib = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
  visAttrib->SetVisibility(false);
  lWorld->SetVisAttributes(visAttrib);
  fVisAttributes.push_back(visAttrib);

  visAttrib = new G4VisAttributes(G4Colour(0.8, 0.6, 0.7));  // for magnet
  fMagPlateTilesL->SetVisAttributes(visAttrib);
  fMagPlateTilesIL->SetVisAttributes(visAttrib);
  visAttrib = new G4VisAttributes(G4Colour(0.8, 0.6, 0.7));  // for magnet
  fMagPlateSlottedTilesNXML->SetVisAttributes(visAttrib);
  fMagPlateSlottedTilesNXPL->SetVisAttributes(visAttrib);
  fMagPlateSlottedTilesNXMIL->SetVisAttributes(visAttrib);
  fMagPlateSlottedTilesNXPIL->SetVisAttributes(visAttrib);
  fMagPlateSlottedTilesSXML->SetVisAttributes(visAttrib);
  fMagPlateSlottedTilesSXPL->SetVisAttributes(visAttrib);
  fMagPlateSlottedTilesSXMIL->SetVisAttributes(visAttrib);
  fMagPlateSlottedTilesSXPIL->SetVisAttributes(visAttrib);
  visAttrib = new G4VisAttributes(G4Colour(0.8, 0.2, 0.3));  // for magnet
  fMagnetCoilL->SetVisAttributes(visAttrib);
  visAttrib = new G4VisAttributes(G4Colour(0.0, 0.0, 0.0));  // for magnet
  fMagnetCoilAirL->SetVisAttributes(visAttrib);
  fVisAttributes.push_back(visAttrib);

  visAttrib = new G4VisAttributes(G4Color(0.5, 0.4, 0.7));  // G4Colour(0.9, 0.4, 0.6));
  for (auto ij=0; ij < fNumDetector; ij++) {
    fLogicDetector[ij]->SetVisAttributes(visAttrib);
  }  
  fVisAttributes.push_back(visAttrib);

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
