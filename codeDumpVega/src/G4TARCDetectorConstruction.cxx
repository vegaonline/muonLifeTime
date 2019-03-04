/*************************************************************************
 * @file      G4TARCDetectorConstruction.cxx
 * @author    Abhijit Bhattacharyya
 * @brief     This file is for Detector Geometry setup
 *************************************************************************/

#include "G4TARCDetectorConstruction.hh"

G4ThreadLocal G4bool G4TARCDetectorConstruction::fConstructedSDandField = false;

G4TARCDetectorConstruction::G4TARCDetectorConstruction()
: G4VUserDetectorConstruction(),
  //fBeamBlock(0), fABlocks(0),   fBBlocks(0), fCBlocks(0), fSampleTubes(0), fSampleSpheres(0), fAllLead(0),
    fDetectorMessenger(0)   {
  fDetectorMessenger = new G4TARCDetectorMessenger(this);
}

G4TARCDetectorConstruction::G4TARCDetectorConstruction( G4String gdmlFileName ) //const G4GDMLParser& parser )
: G4VUserDetectorConstruction(), fDetectorMessenger(0)
  //, fLogiWorld(0),  fBeamBlock(0), fABlocks(0), fBBlocks(0), fCBlocks(0), fSampleTubes(0),  fSampleSpheres(0) //, fAllLead(0) {
{
    if (!fFileLoaded) SetReadFile(gdmlFileName);
    fDetectorMessenger = new G4TARCDetectorMessenger(this);
}

G4TARCDetectorConstruction::~G4TARCDetectorConstruction() {
  // if I uncomment then it crashes at the end after G4Kernel quitting.
  if (fDetectorMessenger) delete fDetectorMessenger;
}


void G4TARCDetectorConstruction::SetReadFile( const G4String& gdmlFile ) {
  fGdmlFileNAME = gdmlFile;
}


G4VPhysicalVolume* G4TARCDetectorConstruction::Construct() {
  if (!fConstructed) {
    fConstructed = true;
    if (!fFileLoaded) {
      fParser.Read(fGdmlFileNAME, true);
      fFileLoaded = true;
      fWorldPhysVol = fParser.GetWorldVolume();
    }

    addTransU();   // define extra material(s) for testSphere LV

    // Declaring SD for whole volume
    fLVS = G4LogicalVolumeStore::GetInstance();
    G4String sslog  = "sampleSphere_log";
    G4String sslog2 = "sampleSphere_log2";
    for(fLVciter = fLVS->begin(); fLVciter != fLVS->end(); fLVciter++) {
      G4String LVName = (*fLVciter)->GetName();
      if ((LVName.find(sslog) != std::string::npos) || (LVName.find(sslog2) != std::string::npos)){
	      (*fLVciter)->SetMaterial(fTc99);
	      (*fLVciter)->SetVisAttributes(new G4Colour(1.0, 0.5, 0.5));
      }
      fLVvectorMini.push_back(*fLVciter);

/*
      std::size_t found1 = LVName.find("blockA_log");
      std::size_t found2 = LVName.find("blockB_log");
      std::size_t found3 = LVName.find("blockC_log");
      std::size_t found4 = LVName.find("Sphere_log");
      std::size_t found5 = LVName.find("Tube_log");
      if ((found1 != std::string::npos) || (found2 != std::string::npos)
	     || (found3 != std::string::npos) || (found4 != std::string::npos)
	      || (found5 != std::string::npos)
      ){
	      if ((LVName.find(sslog) != std::string::npos) || (LVName.find(sslog2) != std::string::npos)){
	        (*fLVciter)->SetMaterial(fTc99);
	        (*fLVciter)->SetVisAttributes(new G4Colour(1.0, 0.5, 0.5));
	      }
	      fLVvectorMini.push_back(*fLVciter);
	      //G4cout << (*fLVciter)->GetName() << G4endl;
      }
*/
    }
  }

  // sample_phys :: CopyNo:0 sampleSphere_log
/*
  fPVS = G4PhysicalVolumeStore::GetInstance();
  for (fPVciter = fPVS->begin(); fPVciter != fPVS->end(); fPVciter++){
    G4cout << (*fPVciter)->GetName() << " copy# " << (*fPVciter)->GetCopyNo()
           << " LogName: " << (*fPVciter)->GetLogicalVolume()->GetName()
           << G4endl;
  }
*/
  //G4cout <<*(G4Material::GetMaterialTable()) << G4endl;
  return fWorldPhysVol;
}


// Sensitive Detectors
void G4TARCDetectorConstruction::ConstructSDandField() {
  /*
  if (!fConstructedSDandField) {
    fConstructedSDandField = true;
    //auto sdManager = G4SDManager::GetSDMpointer();
    //sdManager->SetVerboseLevel(1);
    (G4SDManager::GetSDMpointer())->SetVerboseLevel(1);
    //static G4ThreadLocal G4bool initialized = false;

    G4String sslog  = "sampleSphere_log";
    G4String sslog2 = "sampleSphere_log2";
    G4String fltName, fParticleName, psName, filterName;

    fNeutronFilter = new G4SDParticleFilter(filterName="neutronFilter", fParticleName = "neutron");
    fGammaFilter = new G4SDParticleFilter(filterName="gammaFilter", fParticleName = "gamma");
    fNeutronFilter = new G4SDParticleFilter(filterName="neutronFilter", fParticleName = "neutron");
    fElectronFilter = new G4SDParticleFilter(filterName="ElectronFilter", fParticleName = "e-");
    fPositronFilter = new G4SDParticleFilter(filterName="PositronFilter", fParticleName = "e+");
    fEPFilter = new G4SDParticleFilter(filterName="EPFilter");
    fEPFilter->add(fParticleName="e-");
    fEPFilter->add(fParticleName="e+");
    fProtonFilter = new G4SDParticleFilter(filterName="ProtonFilter", fParticleName = "proton");
    fAntiProtonFilter = new G4SDParticleFilter(filterName="AntiProtonFilter", fParticleName = "anti_proton");
    fMuPlusFilter = new G4SDParticleFilter(filterName="MuPFilter", fParticleName = "mu+");
    fMuMinusFilter = new G4SDParticleFilter(filterName="MuMFilter", fParticleName = "mu-");
    fMuonFilter = new G4SDParticleFilter(filterName="MuonFilter");
    fMuonFilter->add(fParticleName="mu+");
    fMuonFilter->add(fParticleName="mu-");
    fPiPlusFilter = new G4SDParticleFilter(filterName="PiPFilter", fParticleName = "pi+");
    fPiMinusFilter = new G4SDParticleFilter(filterName="PiMFilter", fParticleName = "pi-");
    fPiZeroFilter = new G4SDParticleFilter(filterName="Pi0Filter", fParticleName = "pi0");
    fPionFilter = new G4SDParticleFilter(filterName="PionFilter");
    fPionFilter->add(fParticleName="pi+");
    fPionFilter->add(fParticleName="pi-");
    fPionFilter->add(fParticleName="pi0");
    fDeuteronFilter = new G4SDParticleFilter(filterName="DeuteronFilter", fParticleName = "deuteron");
    fTritonFilter = new G4SDParticleFilter(filterName="TritonFilter", fParticleName = "triton");
    fAlphaFilter = new G4SDParticleFilter(filterName="AlphaFilter", fParticleName = "alpha");
    fHe3Filter = new G4SDParticleFilter(filterName="He3Filter", fParticleName = "He3");

    fTARCSD    = new G4MultiFunctionalDetector("TARCSD");
    fTARCSDSRC = new G4MultiFunctionalDetector("TARCSDSRC");

    (G4SDManager::GetSDMpointer())->AddNewDetector(fTARCSD);
    (G4SDManager::GetSDMpointer())->AddNewDetector(fTARCSDSRC);

    fPrimitive = new G4PSEnergyDeposit("edep");
    fTARCSD->RegisterPrimitive(fPrimitive);
    fPrimitive = new G4PSEnergyDeposit("edep");
    fTARCSDSRC->RegisterPrimitive(fPrimitive);

    fPrimitive = new G4PSNofSecondary("nNeutron");
    fPrimitive->SetFilter(fNeutronFilter);
    fTARCSD->RegisterPrimitive(fPrimitive);
    fPrimitive = new G4PSNofSecondary("nNeutron");
    fPrimitive->SetFilter(fNeutronFilter);
    fTARCSDSRC->RegisterPrimitive(fPrimitive);

    fPrimitive = new G4PSNofSecondary("nGamma");
    fPrimitive->SetFilter(fGammaFilter);
    fTARCSD->RegisterPrimitive(fPrimitive);
    fPrimitive = new G4PSNofSecondary("nGamma");
    fPrimitive->SetFilter(fGammaFilter);
    fTARCSDSRC->RegisterPrimitive(fPrimitive);

    fPrimitive = new G4PSNofSecondary("nElectron");
    fPrimitive->SetFilter(fElectronFilter);
    fTARCSD->RegisterPrimitive(fPrimitive);
    fPrimitive = new G4PSNofSecondary("nElectron");
    fPrimitive->SetFilter(fElectronFilter);
    fTARCSDSRC->RegisterPrimitive(fPrimitive);

    fPrimitive = new G4PSNofSecondary("nPositron");
    fPrimitive->SetFilter(fPositronFilter);
    fTARCSD->RegisterPrimitive(fPrimitive);
    fTARCSDSRC->RegisterPrimitive(fPrimitive);

    fPrimitive = new G4PSNofSecondary("nProton");
    fPrimitive->SetFilter(fProtonFilter);
    fTARCSD->RegisterPrimitive(fPrimitive);
    fTARCSDSRC->RegisterPrimitive(fPrimitive);

    fPrimitive = new G4PSNofSecondary("nAntiProton");
    fPrimitive->SetFilter(fAntiProtonFilter);
    fTARCSD->RegisterPrimitive(fPrimitive);
    fPrimitive = new G4PSNofSecondary("nAntiProton");
    fPrimitive->SetFilter(fAntiProtonFilter);
    fTARCSDSRC->RegisterPrimitive(fPrimitive);

    fPrimitive = new G4PSNofSecondary("nPiPlus");
    fPrimitive->SetFilter(fPiPlusFilter);
    fTARCSD->RegisterPrimitive(fPrimitive);
    fPrimitive = new G4PSNofSecondary("nPiPlus");
    fPrimitive->SetFilter(fPiPlusFilter);
    fTARCSDSRC->RegisterPrimitive(fPrimitive);

    fPrimitive = new G4PSNofSecondary("nPiMinus");
    fPrimitive->SetFilter(fPiMinusFilter);
    fTARCSD->RegisterPrimitive(fPrimitive);
    fPrimitive = new G4PSNofSecondary("nPiMinus");
    fPrimitive->SetFilter(fPiMinusFilter);
    fTARCSDSRC->RegisterPrimitive(fPrimitive);

    fPrimitive = new G4PSNofSecondary("nPiZero");
    fPrimitive->SetFilter(fPiZeroFilter);
    fTARCSD->RegisterPrimitive(fPrimitive);
    fPrimitive = new G4PSNofSecondary("nPiZero");
    fPrimitive->SetFilter(fPiZeroFilter);
    fTARCSDSRC->RegisterPrimitive(fPrimitive);

    fPrimitive = new G4PSNofSecondary("nMuPlus");
    fPrimitive->SetFilter(fMuPlusFilter);
    fTARCSD->RegisterPrimitive(fPrimitive);
    fPrimitive = new G4PSNofSecondary("nMuPlus");
    fPrimitive->SetFilter(fMuPlusFilter);
    fTARCSDSRC->RegisterPrimitive(fPrimitive);

    fPrimitive = new G4PSNofSecondary("nMuMinus");
    fPrimitive->SetFilter(fMuMinusFilter);
    fTARCSD->RegisterPrimitive(fPrimitive);
    fPrimitive = new G4PSNofSecondary("nMuMinus");
    fPrimitive->SetFilter(fMuMinusFilter);
    fTARCSDSRC->RegisterPrimitive(fPrimitive);

    fPrimitive = new G4PSNofSecondary("nDeuteron");
    fPrimitive->SetFilter(fDeuteronFilter);
    fTARCSD->RegisterPrimitive(fPrimitive);
    fPrimitive = new G4PSNofSecondary("nDeuteron");
    fPrimitive->SetFilter(fDeuteronFilter);
    fTARCSDSRC->RegisterPrimitive(fPrimitive);

    fPrimitive = new G4PSNofSecondary("nTriton");
    fPrimitive->SetFilter(fTritonFilter);
    fTARCSD->RegisterPrimitive(fPrimitive);
    fPrimitive = new G4PSNofSecondary("nTriton");
    fPrimitive->SetFilter(fTritonFilter);
    fTARCSDSRC->RegisterPrimitive(fPrimitive);

    fPrimitive = new G4PSNofSecondary("nAlpha");
    fPrimitive->SetFilter(fAlphaFilter);
    fTARCSD->RegisterPrimitive(fPrimitive);
    fPrimitive = new G4PSNofSecondary("nAlpha");
    fPrimitive->SetFilter(fAlphaFilter);
    fTARCSDSRC->RegisterPrimitive(fPrimitive);

    fPrimitive = new G4PSNofSecondary("nHe3");
    fPrimitive->SetFilter(fHe3Filter);
    fTARCSD->RegisterPrimitive(fPrimitive);
    fPrimitive = new G4PSNofSecondary("nHe3");
    fPrimitive->SetFilter(fHe3Filter);
    fTARCSDSRC->RegisterPrimitive(fPrimitive);
*/

    //    if (!initialized){
    //  G4TARCVolumeSD* fAllBlocksSD = new G4TARCVolumeSD("AllVolSD");
    //G4MultiFunctionalDetector* TARCSD = new G4MultiFunctionalDetector("TARCSD");
    //(G4SDManager::GetSDMpointer())->AddNewDetector(TARCSD);

    //G4MultiFunctionalDetector* TARCSDSRC = new G4MultiFunctionalDetector("TARCSDSRC");
    //(G4SDManager::GetSDMpointer())->AddNewDetector(TARCSDSRC);

/*
      G4TARCTargetSD* fBlockBSD = new G4TARCTargetSD("BVolSD");
      (G4SDManager::GetSDMpointer())->AddNewDetector(fBlockBSD);

      G4TARCtransmutSD* fTransmutSD = new G4TARCtransmutSD("99TcSD");
      (G4SDManager::GetSDMpointer())->AddNewDetector(fTransmutSD);


      for (std::vector<G4LogicalVolume*>::iterator it = fLVvectorMini.begin(); it != fLVvectorMini.end(); ++it) {
	//G4cout << (*it)->GetName() << G4endl;

	 if (((*it)->GetName().find(sslog)!=std::string::npos) ||((*it)->GetName().find(sslog2) != std::string::npos)){
	  SetSensitiveDetector( (*it)->GetName(), fTARCSDSRC);
	 } else {
	   SetSensitiveDetector( (*it)->GetName(), fTARCSD);
	 }

	if ((*it)->GetName().find("blockB_log")!=std::string::npos){
	  SetSensitiveDetector( (*it)->GetName(), fBlockBSD);
	} else if (((*it)->GetName().find(sslog)!=std::string::npos) ||((*it)->GetName().find(sslog2) != std::string::npos)){
	  SetSensitiveDetector( (*it)->GetName(), fTransmutSD);
	} else {
	  SetSensitiveDetector( (*it)->GetName(), fAllBlocksSD);
	}

      }
      //initialized = true;
   }
   */
}

void G4TARCDetectorConstruction::addTransU(){
  G4int ncomponents;
  G4double atMass;
  G4int atNum, nNucleons;
  G4double density, abundance, massfraction;
  G4String name, symbol;

  //  99Tc
  atMass = 98.906254 * g / mole;
  atNum = 43;
  nNucleons = 99;
  density = 11.0 * g / cm3;
  G4Isotope* Tc99Iso = new G4Isotope("Tc", atNum, nNucleons, atMass);
  G4Element* Tc99Ele = new G4Element("Tecnicium-99", "99Tc", ncomponents=1);
  Tc99Ele->AddIsotope(Tc99Iso, abundance=100.0*perCent);
  fTc99 = new G4Material("Tc", density, ncomponents=1);
  fTc99->AddElement(Tc99Ele, massfraction=100.0*perCent);
}
