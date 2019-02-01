//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  \file muonDetectorConstruction.cc                                 *
//* ****************************************************
//  $Id: muDetectorConstruction.cc Feb 01, 2019 15:25:08Z vega $

#include "muDetectorConstruction.hh"

muDetectorConstruction::muDetectorConstruction()
: G4VUserDetectorConstruction(), fDetectorMaterial(0), fLogicDetector(0),
  fMagnetMaterial(0), fLogicMagnet(0), fDetMessenger(0), fPhysicalWorld(0) {
    fDetectorLength       =  500.0 * mm;
    fDetectorWidth        =  500.0 * mm;
    fDetectorThickness    =   60.0 * mm;
    fMagnetPlateLength    = 1000.0 * mm;
    fMagnetPlateWidth     = 1000.0 * mm;
    fMagnetPlateThickness =   56.0 * mm;
    fNumDetector          = 2;

    DefineMaterials();
    fDetMessenger = new muDetectorMessenger(this);
  }


muDetectorConstruction::~muDetectorConstruction() {
    delete fDetMessenger;
}


G4VPhysicalVolume* muDetectorConstruction::Construct() {
  return ConstructVolumes();
}

void muDetectorConstruction::DefineMaterials() {
  auto nistMan = G4NistManager::Instance();

  nistMan->FindOrBuildMaterial("G4_AIR");   // Building Air

  // polyvinyltoluene C9H10 scintillator
  nistMan->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  // stainless steel
  nistMan->FindOrBuildMaterial("G4_STAINLESS-STEEL");
}

G4VPhysicalVolume* muDetectorConstruction::ConstructVolumes(){
  // cleaning up old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // Material Construction phase
  DefineMaterials();
  auto air          = G4Material::GetMaterial("G4_AIR");
  auto scintillator = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  auto steel        = G4Material::GetMaterial("G4_STAINLESS-STEEL");

  // The World
  

}
