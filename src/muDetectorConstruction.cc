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

muDetectorConstruction::muDetectorConstruction()
: G4VUserDetectorConstruction(), fDetectorMaterial(0), fLogicDetector(0),
  fMagnetPlateMaterial(0), fLogicMagnet(0), fDetMessenger(0), fPhysicalWorld(0) {
    InitMeasurement();
    DefineMaterials();
    fDetMessenger = new muDetectorMessenger(this);
  }

muDetectorConstruction::~muDetectorConstruction() {
    delete fDetMessenger;
}

void muDetectorConstruction::InitMeasurement(){
  // X is length, Y is thickness and Z is width
  G4double fTmpGap      = 0.0;  // dynamic calculation of height of detector i from center of the magnet
  fDetectorLength       =  500.0 * mm;
  fDetectorWidth        =  500.0 * mm;
  fDetectorThickness    =   60.0 * mm;
  fMagnetPlateLength    = 1000.0 * mm;
  fMagnetPlateWidth     = 1000.0 * mm;
  fMagnetPlateThickness =   56.0 * mm;
  fMagnetPlateGap       = 40.0 * mm;
  fNumDetector          = 2;
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
  fMagneticField = new muMagneticField();


}

void muDetectorConstruction::DefineMaterials() {
  auto nistMan = G4NistManager::Instance();

  nistMan->FindOrBuildMaterial("G4_AIR");   // Building Air

  // polyvinyltoluene C9H10 scintillator
  nistMan->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  // stainless steel
  nistMan->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  air               = G4Material::GetMaterial("G4_AIR");
  scintillator      = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  steel             = G4Material::GetMaterial("G4_STAINLESS-STEEL");

  fWorldMaterial    = air;
  fMagnetPlateMaterial   = steel;
  fDetectorMaterial = scintillator;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

G4VPhysicalVolume* muDetectorConstruction::ConstructVolumes(){
  // cleaning up old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  InitMeasurement(); // initialize measurements.
  // Material Construction phase
  DefineMaterials();

  fWorldMaterial = air;
  fMagnetPlateMaterial = steel;
  fDetectorMaterial = scintillator;

  // The World
  auto sWorld = new G4Box("WorldLab", 0.5 * fWorldLength, 0.5 * fWorldThickness, 0.5 * fWorldWidth);
  auto lWorld  = new G4LogicalVolume(sWorld, fWorldMaterial, "World");
  fPhysicalWorld = new G4PVPlacement(0, G4ThreeVector(), lWorld, "World", 0, false, 0);

  // Detectors
  fSetupTopHt = 0.0;
  for (G4int ii = 0; ii < fNumDetector; ii++) {  // Define each detector with respect to the magnet
    auto detname = "Detector" + std::to_string(ii);  // if there is error use std::ostringstream ss; ss << ii; detname = ss.str()
    auto detectorBox = new G4Box(detname, 0.5 * fDetectorLength, 0.5 * fDetectorThickness, 0.5 * fDetectorWidth);
    auto detLogic = new G4LogicalVolume(detectorBox, fDetectorMaterial, detname);
    fLogicDetector.push_back(detLogic);
    G4double yCoord = fDetPlaced[ii] * fDistDetMagnet[ii] ;
    fSetupTopHt = std::max(fSetupTopHt, std::abs(yCoord) + 0.5 * fDetPlaced[ii] * fDetectorThickness);
    new G4PVPlacement(0, G4ThreeVector(0.0, yCoord, 0.0), detLogic, detname, lWorld, false, 0);
  }

  // Magnet
  auto magBox = new G4Box("MagnetPlate", 0.5 * fMagnetPlateLength, 0.5 * fMagnetPlateThickness, 0.5 * fMagnetPlateWidth);
  auto MagPlateL = new G4LogicalVolume(magBox, fMagnetPlateMaterial, "MagnetPlate");  // Plate 1 is at +Y && Plate 2 is at -Y
  auto fMagnetAssembly = new G4AssemblyVolume();
  G4ThreeVector Ta(0.0, 0.5 * fMagnetPlateGap, 0.0);
  G4RotationMatrix* Ra;
  fMagnetAssembly->AddPlacedVolume(MagPlateL, Ta, Ra);
  Ta.setX(0.0); Ta.setY(-0.5 * fMagnetPlateGap); Ta.setZ(0.0);
  fMagnetAssembly->AddPlacedVolume(MagPlateL, Ta, Ra);
  Ta.setX(0.0); Ta.setY(0.0); Ta.setZ(0.0);
  fMagnetAssembly->MakeImprint(lWorld, Ta, Ra); // placing at the origin of the world volume

  PrintParameters();
  return fPhysicalWorld;
}

void muDetectorConstruction::PrintParameters() {
  G4cout << "\n Total number of detectors: " << fNumDetector << G4endl;
  for (G4int ii = 0; ii < fNumDetector; ii++) {
    G4cout << " Detector #"   << ii << " Length = "  << fDetectorLength
           << " Thickness = " << fDetectorThickness  << " Width = " << fDetectorWidth
           << " Distance from center of the magnet  "<<  fDetPlaced[ii] * (fDistDetMagnet[ii] + 0.5 * fDetectorThickness) << G4endl;
  }
  G4cout << " Magnet Plate"  << " Length = " << fMagnetPlateLength
         << " Thickness = " << fMagnetPlateThickness << " Width = " << fMagnetPlateWidth << G4endl;
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
