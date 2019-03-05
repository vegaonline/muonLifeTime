//******************************************************
//*  Muon decay while passing through parmanent magnet *
//*                                                    *
//*  Abhijit Bhattacharyya, NPD, BARC                  *
//*  Example TestEm17 was considered grossly           *
//*  Example Fields considered for field mapping       *
//*  \file muonLife.cc                                 *
//* ****************************************************
//  $Id: muonLife.cc Feb 01, 2019 15:25:08Z vega $

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4Types.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4EmParameters.hh"
#include "G4HadronicProcessStore.hh"
#include "G4PhysicsListHelper.hh"
#include "G4PhysListFactory.hh"
#include "G4StepLimiterPhysics.hh"
#include "FTFP_BERT.hh"
#include "G4Electron.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4MuonicAtom.hh"
#include "G4MuonicAtomDecay.hh"
#include "G4MuonicAtomDecayPhysics.hh"
#include "G4MuonRadiativeDecayChannelWithSpin.hh"
#include "G4MuonDecayChannel.hh"
#include "G4MuonNuclearProcess.hh"
#include "G4MuonDecayChannelWithSpin.hh"
#include "G4Transportation.hh"
#include "G4VTrajectoryModel.hh"
#include "G4TrajectoryDrawByParticleID.hh"
#include "G4CoupledTransportation.hh"
#include "Randomize.hh"

#include "G4BlineTracer.hh"

#include "muDetectorConstruction.hh"
#include "muPhysicsList.hh"
#include "muPrimaryGeneratorAction.hh"
#include "muActionInitialization.hh"

//  #include "muRunAction.hh"
//#include "muSteppingAction.hh"
//#include "muStackingAction.hh"


void PrintUsage() {
  G4cerr << G4endl;
  G4cerr << "Usage: muonLife [-m macro] [-u UIsession] [-t nThreads] " << G4endl;
  G4cerr << "\t \t Here -t option is available for multithreaded mode only." << G4endl;
}

int main(int argc, char** argv) {
  G4String macro;
  G4String session;
  //G4int nThreads;

  if (argc < 1 || argc > 7){
    PrintUsage();
    return -1;
  }

  //choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

#ifdef G4MULTITHREADED
  G4int nThreads = G4Threading::G4GetNumberOfCores();
#endif


  for (G4int i = 1; i < argc; i += 2) {
    G4String inArg = G4String (argv[i]);
    if (inArg == "-m") macro = argv[i + 1];
    if (inArg == "-u" ) session = argv[i + 1];
  // Construct the default run manager
#ifdef G4MULTITHREADED
    if (inArg == "-t") nThreads = (G4String(argv[i+1]) == "NMAX")
                                      ?  G4Threading::G4GetNumberOfCores()
                                      : G4UIcommand::ConvertToInt(argv[i + 1]);
#endif
    if (inArg != "-m" && inArg != "-u" && inArg != "-t"){
      PrintUsage();
      return -1;
    }
  }

#ifdef G4MULTITHREADED
  auto runManager = new G4MTRunManager;
  if (nThreads > 0) runManager->SetNumberOfThreads(nThreads);
  G4cout << "Code is started with " << runManager->GetNumberOfThreads() << " threads. " << G4endl;
#else
  auto runManager = new G4RunManager;
#endif


  //set mandatory initialization classes
  auto muDet= new muDetectorConstruction;
  runManager->SetUserInitialization(muDet);

  //G4BLineTracer* theBLineTool = new G4BLineTracer();

  G4PhysListFactory fPhysFactory;
  //        G4VModularPhysicsList* phys = fPhysFactory.GetReferencePhysList("FTFP_BERT");
  auto muPhys = new muPhysicsList();
  //phys->RegisterPhysics(muPhys);

  //physList->RegisterPhysics(new G4StepLimiterPhysics());
  //runManager->SetUserInitialization(physList);
  runManager->SetUserInitialization(muPhys);

  runManager->SetUserInitialization(new muActionInitialization(muDet));

/*
  auto muPrim = new muPrimaryGeneratorAction(muDet);
  runManager->SetUserAction(muPrim);
  muHistoManager* muHisto = new muHistoManager();
  auto muRun = new muRunAction(muDet, muPrim, muHisto);
  runManager->SetUserAction(muRun);
  runManager->SetUserAction(new muSteppingAction(muRun, muHisto));
  runManager->SetUserAction(new muStackingAction);
  //runManager->SetUserInitialization(new muActionInitialization(muDet, muPrim));
  */

  runManager->Initialize();

  // Create new drawByParticleID model
  auto model = new G4TrajectoryDrawByParticleID;
  // Configure model
  model->SetDefault("white");
  model->Set("gamma", "green");
  model->Set("mu+", G4Color(0.75, 0.19, 0.25));
  model->Set("mu-", G4Color(0.19, 0.75, 0.25));
  model->Set("e+", "cyan");
  model->Set("e-", "blue");
  model->Set("proton", "red");
  model->Set("neutron", "yellow"); //G4Color(0.7, 0.1, 0.4));

  //Register model with visualization manager

  //initialize visualization
  auto visManager = new G4VisExecutive;
  visManager->Initialize();
  visManager->RegisterModel(model);
 // get the pointer to the User Interface manager
  auto UImanager = G4UImanager::GetUIpointer();

  if (macro.size()){
    // batch mode
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command + macro);
  } else {
    UImanager->ApplyCommand("/control/execute vis.mac");
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv, session);
#endif
    ui->SessionStart();
    delete ui;
    //  delete theBLineTool;
  }

  G4GeometryManager::GetInstance()->OpenGeometry();
  //termination of job
#ifdef G4VIS_USE
  delete visManager;
  G4cout << "Vis manager deleted" << G4endl;
#endif
  delete runManager;
  G4cout << "run manager deleted" << G4endl;
  return 0;


}
