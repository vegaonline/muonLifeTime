/******************************************************************************
 * @file      G4TARCDetectorConstruction.hh
 * @author    Abhijit Bhattacharyya
 * @brief     This file loads the GDML based TARC geometry file
 *             The geometry file was originally taken from Alexander Howard
 *             Coding ideas have been taken from Vladimir GRICHINE

 *****************************************************************************/
#ifndef G4TARC_DETECTORCONSTRUCTION_H
#define G4TARC_DETECTORCONSTRUCTION_H

#include <algorithm>

#include "G4TARCDetectorMessenger.hh"
#include "G4TARCTargetSD.hh"
#include "G4TARCVolumeSD.hh"
#include "G4TARCtransmutSD.hh"
#include "G4TARCParallelWorld.hh"


#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"
#include "G4GDMLParser.hh"
#include "G4PVReplica.hh"
#include "G4PVPlacement.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSNofSecondary.hh"
#include "G4PSTrackLength.hh"
#include "G4PSNofStep.hh"
#include "G4PSMinKinEAtGeneration.hh"
#include "G4VSDFilter.hh"
#include "G4SDParticleFilter.hh"

#include "G4GeometryManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VSolid.hh"
#include "G4SolidStore.hh"
#include "G4PVPlacement.hh"
#include "G4Transform3D.hh"
#include "G4AffineTransform.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"

#include "globals.hh"
#include "G4PhysicalConstants.hh"

class G4GDMLParser;
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4VSensitiveDetector;
class G4VisAttributes;
class G4LogicalVolume;
class G4TARCDetectorMessenger; //---------------
class G4TARCTargetSD;
class G4TARCVolumeSD;
class G4TARCtransmutSD;
class G4VReplica;


// Detector constructor is loading the GDML geometry
class G4TARCDetectorConstruction : public G4VUserDetectorConstruction {
public:
  G4TARCDetectorConstruction();
  G4TARCDetectorConstruction( G4String ); //const G4GDMLParser& parser );
  virtual ~G4TARCDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();
  void SetReadFile( const G4String& );  // GDML file reader
  inline G4VPhysicalVolume* GetWorldVolume() { return fWorldPhysVol; }


private:
  G4TARCDetectorConstruction & operator=(const G4TARCDetectorConstruction&);
  G4TARCDetectorConstruction ( const G4TARCDetectorConstruction& );

  void addTransU();

private:
  static G4ThreadLocal G4bool fConstructedSDandField;
  G4bool                   fConstructed;
  G4bool                   fLogiTestLogPBeamTest = false;
  G4bool                   fFileLoaded = false;
  G4TARCDetectorMessenger* fDetectorMessenger = 0;
  G4String                 fGdmlFileNAME;
  G4GDMLParser             fParser;
  G4VPhysicalVolume*       fWorldPhysVol = 0;
  G4LogicalVolume*         fLAB = 0;

  G4LogicalVolume*         fTestLogPBeamTest = 0;
  G4LogicalVolumeStore*    fLVS;
  G4PhysicalVolumeStore*   fPVS;
  std::vector<G4LogicalVolume*>  ::const_iterator fLVciter;
  //std::vector<G4VPhysicalVolume*>::const_iterator fPVciter;
  std::vector<G4LogicalVolume*>     fLVvectorMini;  /// this is to store some LV for SD
  //G4ThreeVector            pbTargetMin;
  //G4ThreeVector            pbTargetMax;
  G4Material*              fTargetMat;
  G4Material*              fTc99;
/*
  G4MultiFunctionalDetector*          fTARCSD;
  G4MultiFunctionalDetector*          fTARCSDSRC;
	G4VPrimitiveScorer*               fPrimitive;
	G4SDParticleFilter*               fNeutronFilter;
	G4SDParticleFilter*               fGammaFilter;
	G4SDParticleFilter*               fEPFilter;
	G4SDParticleFilter*               fElectronFilter;
	G4SDParticleFilter*               fPositronFilter;
	G4SDParticleFilter*               fProtonFilter;
	G4SDParticleFilter*               fAntiProtonFilter;
	G4SDParticleFilter*               fMuPlusFilter;
	G4SDParticleFilter*               fMuMinusFilter;
	G4SDParticleFilter*               fMuonFilter;
	G4SDParticleFilter*               fPionFilter;
	G4SDParticleFilter*               fPiPlusFilter;
	G4SDParticleFilter*               fPiMinusFilter;
	G4SDParticleFilter*               fPiZeroFilter;
	G4SDParticleFilter*               fDeuteronFilter;
	G4SDParticleFilter*               fTritonFilter;
	G4SDParticleFilter*               fAlphaFilter;
	G4SDParticleFilter*               fHe3Filter;
*/
};

#endif
