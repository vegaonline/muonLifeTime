/********************************************************************
 * @file     G4TARCParallelWorld.hh
 * @author   Abhijit Bhattacharyya
 * @brief    Here use parallel world
 ********************************************************************/

#ifndef G4TARC_PARALLELWORLD_H
#define G4TARC_PARALLELWORLD_H

#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>

#include "G4TARCDetectorConstruction.hh"
#include "G4TARCPVolumeStore.hh"
#include "G4TARCVolumeSD.hh"

// primitive scorers related
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDParticleFilter.hh"
#include "G4PSNofCollision.hh"
#include "G4PSPopulation.hh"
#include "G4PSTrackCounter.hh"
#include "G4PSTrackLength.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSNofSecondary.hh"
#include "G4PSTrackLength.hh"
#include "G4PSNofStep.hh"
#include "G4PSMinKinEAtGeneration.hh"
#include "G4VSDFilter.hh"
#include "G4SDParticleFilter.hh"


//importance biasing related
#include "G4IStore.hh"

#include "G4GeometryCell.hh"

#include "G4VUserParallelWorld.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4PVReplica.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

#include "G4SolidStore.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VIStore;
class G4TARCVolumeSD;

class G4TARCParallelWorld : public G4VUserParallelWorld {
public:
  G4TARCParallelWorld (G4String&);
  virtual ~G4TARCParallelWorld();

public:
  virtual void Construct();
  virtual void ConstructSD();
  virtual void ConstructSDandField(){};

  void SetSerialGeometry(G4bool);
  G4bool IsSerial() const {return fSerial;}

  void DefineShellsBlocks();
  G4VPhysicalVolume& GetShellVolumeAddress() const { return *fShellPhys; }
  G4VPhysicalVolume& GetWorldVolumeAddress() const { return *ghostWorld; }
  G4VPhysicalVolume* GetWorldVolume() {return ghostWorld;}
  const G4VPhysicalVolume& GetPhysicalVolumeByName(const G4String& name) const { return *fPVolumeStore.GetPVolume(name);}
  G4GeometryCell GetGeometryCell(G4int);
  G4String GetCellName(G4int);
  G4VIStore* CreateImportanceStore();


private:
	G4bool                            fSerial;
    G4bool                            fConstructed;
	static G4ThreadLocal        G4bool fSDConstructed;

    G4TARCPVolumeStore                fPVolumeStore;

	//G4MultiFunctionalDetector*          fTARCSDP;
	//G4MultiFunctionalDetector*          fTARCSDSRCP;
  //G4MultiFunctionalDetector*        fTARCNeutronDet;
  //G4MultiFunctionalDetector*        fTARCProtonDet;

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



    std::vector<G4double>                  fRadiusReference {200.0 * cm, 190.0 * cm, 185.0 * cm, 175.0 * cm, 165.0 * cm, 150.0 * cm,
      140.0 * cm, 130.0 * cm, 120.0 * cm, 110.0 * cm, 100.0 * cm, 90.0 * cm, 80.0 * cm, 70.0 * cm, 60.0 * cm, 50.0 * cm, 45.7 * cm,
      40.0 * cm, 30.0 * cm, 25.0 * cm, 20.0 * cm, 15.0 * cm, 10.0 * cm, 8.0 * cm, 5.0 * cm, 3.0 * cm};

    G4int                                fRefShellNumber;
    G4int                                fRefShellOuterRad;
    G4int                                fRefShellInnerRad;
    G4int                                fShellNumber;
    G4double                          fShellThickness;
    G4double                          fRefShellThickness;
    G4double                          fMaxOuterRadiusofShell;
    G4double                          fMinInnerRadiusofShell;
    G4double                          fInnerRadProtonShell;
    G4double                          fOuterRadProtonShell;
    std::vector<G4double>             fOuterRadiusofShell;
    std::vector<G4double>             fInnerRadiusofShell;
    std::vector<G4LogicalVolume*>     fLVvector;
    G4double                          fHalfXBlockB;
    G4double                          fHalfYBlockB;
    G4double                          fHalfZBlockB;
    G4double                          fHalfXVBox;
    G4double                          fHalfYVBox;
    G4double                          fHalfZVBox;
    G4double                          fNewHalfZProt;
    G4double                          fZposProt;
    G4double                          fDiaMaxSphere;
    G4double                          fRadMaxSphere;
    G4LogicalVolume*                  fVBoxLogProton;
    G4VPhysicalVolume*                fVBoxPVProton;
    G4LogicalVolume*                  fShellLog;
    G4VPhysicalVolume*                fShellPhys;
    G4LogicalVolume*                  ghostWorldLog;
    G4VPhysicalVolume*                ghostWorld;


    G4LogicalVolumeStore*             fLVS;
    G4PhysicalVolumeStore*            fPVS;
    std::vector<G4VPhysicalVolume*>::const_iterator fPVciter;
    std::vector<G4LogicalVolume*>  ::const_iterator fLVciter;
};

#endif
