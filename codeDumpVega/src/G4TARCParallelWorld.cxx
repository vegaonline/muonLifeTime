/**********************************************************
 * @file      G4TARCParallelWorld.cxx
 * @author    Abhijit Bhattacharyya
 * @brief     Parallelization processes
 *********************************************************/
#include "G4TARCParallelWorld.hh"

G4ThreadLocal G4bool G4TARCParallelWorld::fSDConstructed = false;


G4TARCParallelWorld::G4TARCParallelWorld( G4String& tarcParallelWorld)
  : G4VUserParallelWorld( tarcParallelWorld), fConstructed(false), fSerial(false) {
  DefineShellsBlocks();
}

G4TARCParallelWorld::~G4TARCParallelWorld() {
  std::vector<G4LogicalVolume*>().swap(fLVvector);
  std::vector<G4double>().swap(fInnerRadiusofShell);
  std::vector<G4double>().swap(fOuterRadiusofShell);
}

// Here defining concentric shells of finite thickness
void G4TARCParallelWorld::DefineShellsBlocks() {
  fHalfXBlockB           =     0.5 * 300 * mm;
  fHalfYBlockB           =     0.5 * 300 * mm;
  fHalfZBlockB           =     0.5 * 600 * mm;
  fHalfXVBox             =     0.5 * 150 * mm;
  fHalfYVBox             =     0.5 * 150 * mm;
  fHalfZVBox             =     0.5 * 300 * mm;
  fNewHalfZProt          =     0.5 * ((2.0 * fHalfZBlockB) / 3.0);
  fZposProt              = -fHalfZBlockB + fNewHalfZProt;
  fDiaMaxSphere          =  3300.0 * mm;
  fRadMaxSphere          =     0.5 * fDiaMaxSphere;
  fShellThickness        =     5.0 * cm;
  fRefShellThickness        =     2.0 * mm;
  fRefShellNumber = fRadiusReference.size();
  fRefShellOuterRad    = 457 * mm;
  fRefShellInnerRad   = fRefShellOuterRad - fRefShellThickness;
  fMinInnerRadiusofShell =    10.0 * mm;
  fMaxOuterRadiusofShell =  1500.0 * mm;
  fInnerRadProtonShell   =     0.0 * mm;   //
  fOuterRadProtonShell   =   300.0 * mm;   // These two were thought as a spherical 4Pi measurement for Proton
  // fShellNumber           = (G4int)((fMaxOuterRadiusofShell - fMinInnerRadiusofShell) / fShellThickness + 0.5);
  fRefShellNumber = fRadiusReference.size();

  for (G4int ii = 0; ii < fRefShellNumber; ii++) {
    G4double tmp = fRadiusReference[ii];
    fOuterRadiusofShell.push_back(tmp);
    tmp -=fRefShellThickness;
    fInnerRadiusofShell.push_back(tmp);
  }
  std::cout << "Define Blocks in ParallelWorld  initialised." << std::endl;
}


void G4TARCParallelWorld::Construct() {
  if (fConstructed) return;
  // A dummy material is used to fill the volulmes of the readout geometry.  Can we use Pb etc?
  G4Material* dummyMat = nullptr;
  G4String nameTmp;

  G4Colour col1 (1.0, 0.0, 0.0);  // red
  G4Colour col2 (1.0, 0.7, 0.0);
  G4Colour col3 (0.0, 1.0, 1.0);  //cyan
  G4VisAttributes* fAtt1 = new G4VisAttributes(col1);
  G4VisAttributes* fAtt2 = new G4VisAttributes(col2);
  G4VisAttributes* fAtt3 = new G4VisAttributes(col3);
  fAtt1->SetVisibility(true);
  fAtt2->SetVisibility(true);
  fAtt3->SetVisibility(true);
  fAtt3->SetForceSolid(true);

  // declaring the readout world
  // First the whole World
  ghostWorld = GetWorld();
  ghostWorldLog = ghostWorld->GetLogicalVolume();
  fLVvector.push_back(ghostWorldLog);
  fPVolumeStore.AddPVolume(G4GeometryCell(*ghostWorld, 0));

/*  This is being commented to run it smoothly without error and then it would be fixed.

  // Make a rectangular plate for proton hits and spallation point
  // blockB copy 50 equivalent : Z length is 300 mm for blockB. Here we consider 100 mm.
  G4Box* fTestBlockB50 = new G4Box("BoxPHit", fHalfXBlockB, fHalfYBlockB, fNewHalfZProt);
  fVBoxLogProton = new G4LogicalVolume(fTestBlockB50, dummyMat, "VB50Proton_log");
  fLVvector.push_back(fVBoxLogProton);
  nameTmp = GetCellName(0);
  // Here originally copy 50 was at 0,0,0 with zlength 300. Since we reduced to 100 and wish to
  // stick lowest Z bouondary to be at same position, origin of this block is shifted to -1000
  // so that
  fVBoxPVProton = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, fZposProt), nameTmp, fVBoxLogProton, ghostWorld, false, 0);
  fVBoxLogProton->SetVisAttributes(fAtt1);
  G4GeometryCell fProtonCell(*fVBoxPVProton, 0);
  fPVolumeStore.AddPVolume(fProtonCell);

*/

  // Make the thinner shell bunches
  for (G4int i = 0; i < fRefShellNumber; i++) {
    G4Sphere* radShellSphere = new G4Sphere("shellSphere", fInnerRadiusofShell[i], fOuterRadiusofShell[i],
                            0.0*deg, 360.0*deg, 0.0*deg, 180.0*deg);
    nameTmp = "radial_shell_log_" + std::to_string(i);
    fShellLog = new G4LogicalVolume(radShellSphere, dummyMat, nameTmp);
    fLVvector.push_back(fShellLog);
    nameTmp = GetCellName(i);
    fShellPhys = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), nameTmp, fShellLog, ghostWorld, false, i);
    fShellLog->SetVisAttributes(fAtt3);
    G4GeometryCell radCell(*fShellPhys, i);
    fPVolumeStore.AddPVolume(radCell);
  }
  fConstructed = true;
}


G4String G4TARCParallelWorld::GetCellName(G4int i) {
  G4String tmp = "";
  tmp = (i < 10) ? "0" + std::to_string(i) : std::to_string(i);
  return "cell_" + tmp;
}

G4GeometryCell G4TARCParallelWorld::GetGeometryCell(G4int i) {
  G4String name(GetCellName(i));
  const G4VPhysicalVolume* tmpPV =fPVolumeStore.GetPVolume(name);
  if (tmpPV) {
    return G4GeometryCell(*tmpPV, 0);
  } else {
    return G4GeometryCell(*ghostWorld, -2);
  }
}


void G4TARCParallelWorld::ConstructSD() {
  if (!fSDConstructed) {
    auto SDman = G4SDManager::GetSDMpointer();
    G4String fltName, fParticleName, psName, filterName;

    fNeutronFilter = new G4SDParticleFilter(filterName="neutronFilter", fParticleName = "neutron");
    /*
    fGammaFilter = new G4SDParticleFilter(filterName="gammaFilter", fParticleName = "gamma");
    fElectronFilter = new G4SDParticleFilter(filterName="ElectronFilter", fParticleName = "e-");
    fPositronFilter = new G4SDParticleFilter(filterName="PositronFilter", fParticleName = "e+");
    fEPFilter = new G4SDParticleFilter(filterName="EPFilter");
    fEPFilter->add("e-");
    fEPFilter->add("e+");
    fProtonFilter = new G4SDParticleFilter(filterName="ProtonFilter", fParticleName = "proton");
    fAntiProtonFilter = new G4SDParticleFilter(filterName="AntiProtonFilter", fParticleName = "anti_proton");
    fMuPlusFilter = new G4SDParticleFilter(filterName="MuPFilter", fParticleName = "mu+");
    fMuMinusFilter = new G4SDParticleFilter(filterName="MuMFilter", fParticleName = "mu-");
    fMuonFilter = new G4SDParticleFilter(filterName="MuonFilter");
    fMuonFilter->add("mu+");
    fMuonFilter->add("mu-");
    fPiPlusFilter = new G4SDParticleFilter(filterName="PiPFilter", fParticleName = "pi+");
    fPiMinusFilter = new G4SDParticleFilter(filterName="PiMFilter", fParticleName = "pi-");
    fPiZeroFilter = new G4SDParticleFilter(filterName="Pi0Filter", fParticleName = "pi0");
    fPionFilter = new G4SDParticleFilter(filterName="PionFilter");
    fPionFilter->add("pi+");
    fPionFilter->add("pi-");
    fPionFilter->add("pi0");
    fDeuteronFilter = new G4SDParticleFilter(filterName="DeuteronFilter", fParticleName = "deuteron");
    fTritonFilter = new G4SDParticleFilter(filterName="TritonFilter", fParticleName = "triton");
    fAlphaFilter = new G4SDParticleFilter(filterName="AlphaFilter", fParticleName = "alpha");
    fHe3Filter = new G4SDParticleFilter(filterName="He3Filter", fParticleName = "He3");
    */

    G4MultiFunctionalDetector* fTARCNeutSD = new G4MultiFunctionalDetector("TARCNeut");
    SDman->AddNewDetector(fTARCNeutSD);
    fTARCNeutSD->SetFilter(fNeutronFilter);
    G4MultiFunctionalDetector* fTARCNeutSRCSD = new G4MultiFunctionalDetector("TARCNeutSRC");
    SDman->AddNewDetector(fTARCNeutSRCSD);
    fTARCNeutSRCSD->SetFilter(fNeutronFilter);

    G4String sslog = "sampleSphere_log";
    G4String sslog2 = "sampleSphere_log2";

    //------------- Next we need to add concerned LV to these detectors ----------------------
    for (std::vector<G4LogicalVolume*>::iterator it = fLVvector.begin(); it != fLVvector.end(); ++it) {
      if (((*it)->GetName().find(sslog) !=std::string::npos) ||  ((*it)->GetName().find(sslog2)!=std::string::npos)) {
        SetSensitiveDetector( (*it)->GetName(), fTARCNeutSRCSD);
      } else {
        SetSensitiveDetector( (*it)->GetName(), fTARCNeutSD);
      }
    }

    G4PSNofCollision*   scorer00 = new G4PSNofCollision(psName="Collisions0");
    G4PSNofCollision*   scorer10 = new G4PSNofCollision(psName="CollWeight0");
    scorer10->Weighted(true);
    G4PSPopulation*   scorer20 = new G4PSPopulation(psName="Population0");
    G4PSTrackCounter* scorer30 = new G4PSTrackCounter(psName="Track_Enter0",fCurrent_In);
    G4PSTrackLength* scorer40 = new G4PSTrackLength(psName="SL0");   // Step_Length
    G4PSTrackLength* scorer50 = new G4PSTrackLength(psName="SLW0");  // Step_Length_Weighted
    scorer50->Weighted(true);
    G4PSTrackLength* scorer60 = new G4PSTrackLength(psName="SLWE0");   // Step_Length_Weighted.KE
    scorer60->Weighted(true);
    scorer60->MultiplyKineticEnergy(true);
    G4PSTrackLength* scorer70 = new G4PSTrackLength(psName="SLW_V0");       // Step_Length_Weighted_By_Velocity
    scorer70->Weighted(true);
    scorer70->DivideByVelocity(true);
    G4PSTrackLength* scorer80 = new G4PSTrackLength(psName="SLWE_V0");       // Track_Length_Weighted_KE_By_Velocity
    scorer80->Weighted(true);
    scorer80->MultiplyKineticEnergy(true);
    scorer80->DivideByVelocity(true);


    fTARCNeutSD->RegisterPrimitive(scorer00);
    fTARCNeutSD->RegisterPrimitive(scorer10);
    fTARCNeutSD->RegisterPrimitive(scorer20);
    fTARCNeutSD->RegisterPrimitive(scorer30);
    fTARCNeutSD->RegisterPrimitive(scorer40);
    fTARCNeutSD->RegisterPrimitive(scorer50);
    fTARCNeutSD->RegisterPrimitive(scorer60);
    fTARCNeutSD->RegisterPrimitive(scorer70);
    fTARCNeutSD->RegisterPrimitive(scorer80);


    G4PSNofCollision*   scorer01 = new G4PSNofCollision(psName="Collisions1");
    G4PSNofCollision*   scorer11 = new G4PSNofCollision(psName="CollWeight1");
    scorer11->Weighted(true);
    G4PSPopulation*   scorer21 = new G4PSPopulation(psName="Population1");
    G4PSTrackCounter* scorer31 = new G4PSTrackCounter(psName="Track_Enter1",fCurrent_In);
    G4PSTrackLength* scorer41 = new G4PSTrackLength(psName="SL1");   // Step_Length
    G4PSTrackLength* scorer51 = new G4PSTrackLength(psName="SLW1");  // Step_Length_Weighted
    scorer51->Weighted(true);
    G4PSTrackLength* scorer61 = new G4PSTrackLength(psName="SLWE1");   // Step_Length_Weighted.KE
    scorer61->Weighted(true);
    scorer61->MultiplyKineticEnergy(true);
    G4PSTrackLength* scorer71 = new G4PSTrackLength(psName="SLW_V1");       // Step_Length_Weighted_By_Velocity
    scorer71->Weighted(true);
    scorer71->DivideByVelocity(true);
    G4PSTrackLength* scorer81 = new G4PSTrackLength(psName="SLWE_V1");       // Track_Length_Weighted_KE_By_Velocity
    scorer81->Weighted(true);
    scorer81->MultiplyKineticEnergy(true);
    scorer81->DivideByVelocity(true);


    fTARCNeutSRCSD->RegisterPrimitive(scorer01);
    fTARCNeutSRCSD->RegisterPrimitive(scorer11);
    fTARCNeutSRCSD->RegisterPrimitive(scorer21);
    fTARCNeutSRCSD->RegisterPrimitive(scorer31);
    fTARCNeutSRCSD->RegisterPrimitive(scorer41);
    fTARCNeutSRCSD->RegisterPrimitive(scorer51);
    fTARCNeutSRCSD->RegisterPrimitive(scorer61);
    fTARCNeutSRCSD->RegisterPrimitive(scorer71);
    fTARCNeutSRCSD->RegisterPrimitive(scorer81);

    fSDConstructed = true;
  }
}


void G4TARCParallelWorld::SetSerialGeometry(G4bool ser) {
  if (fSerial == ser) return;
  fSerial = ser;
  if (!fConstructed) return;
}

G4VIStore* G4TARCParallelWorld::CreateImportanceStore(){
  G4VPhysicalVolume* thisGhostWorld = GetWorld();
  G4cout << " G4TARCParallelWorld:: Creating Importance Store " << G4endl;
  if (!fPVolumeStore.Size())  {
    G4Exception("G4TARCParallelWorld::CreateImportanceStore"
               ,"Testing...",RunMustBeAborted
               ,"no physical volumes created yet!");
  }
  // creating and filling the importance store
  //  G4IStore *istore = new G4IStore(*fWorldVolume);
  G4IStore *fMyIstore = G4IStore::GetInstance(thisGhostWorld->GetName());
  G4GeometryCell gWorldVolumeCell(GetWorldVolumeAddress(), 0);

  fMyIstore->AddImportanceGeometryCell(1, gWorldVolumeCell);
  // set importance values and create scorers
  //  G4int number_shells = analysis->GetNumberShells();
  // G4int cell(fShellNumber);
  for (G4int cell = 0; cell < fRefShellNumber; cell++) {
    G4GeometryCell gCell = GetGeometryCell(cell);
    fMyIstore->AddImportanceGeometryCell(1, gCell.GetPhysicalVolume(), cell);
  }
  return fMyIstore;
}
