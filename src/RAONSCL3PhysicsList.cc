#include "RAONSCL3PhysicsList.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "G4Deuteron.hh"

// Physics Lists (contained inside the Geant4 distribution)
#include "G4EmStandardPhysics_option3.hh"
#include "G4DecayPhysics.hh"
#include "G4StepLimiter.hh"
#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProcessManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"

#include "G4EmExtraPhysics.hh"
#include "G4EmParameters.hh"
#include "G4NuclideTable.hh"

#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "QGSP_BIC_HP.hh"
#include "G4PhysListFactory.hh"
#include "G4DeexPrecoParameters.hh"

RAONSCL3PhysicsList::RAONSCL3PhysicsList(B1DetectorConstruction* det)
  : G4VModularPhysicsList()
{
  // add new units for radioactive decays

  new G4UnitDefinition("millielectronVolt", "meV", "Energy", 1.e-3*eV);
  const G4double minute = 60*second;
  const G4double hour    = 60*minute;
  const G4double day     = 24*hour;
  const G4double year    = 365*day;
  new G4UnitDefinition("minute", "min", "Time", minute);
  new G4UnitDefinition("hour", "h", "Time", hour);
  new G4UnitDefinition("day", "d", "Time", day);
  new G4UnitDefinition("year", "y", "Time", year);

  // Mandatory for G4NclideTable
  // Half-life threshold must be set small or many short-lived isomers
  // will not be assigned life times (default to 0)
  G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(0.1*second);
  G4NuclideTable::GetInstance()->SetLevelTolerance(1.0*eV);

  //---
  fDetector = det;

  G4LossTableManager::Instance();

  defaultCutValue = 0.1*mm;
  fCutForGamma        = defaultCutValue;
  fCutForElectron     = defaultCutValue;
  fCutForPositron     = defaultCutValue;

  fThickness_foil     = defaultCutValue;
  fThickness_target   = defaultCutValue;

  fCutTargetProton    = fThickness_target;
  fCutTargetElectron  = fThickness_target;
  fCutTargetPositron  = fThickness_target;
  fCutTargetGamma     = fThickness_target;
  fCutTargetNeutron   = fThickness_target;

  fCutFoilProton      = fThickness_foil;
  fCutFoilElectron      = fThickness_foil;
  fCutFoilPositron      = fThickness_foil;
  fCutFoilGamma      = fThickness_foil;
  fCutFoilNeutron      = fThickness_foil;

  //EM physics
  fEmPhysicsList = new G4EmStandardPhysics_option3(0);
  fEmName = G4String("emstandard_opt3");

  //Decay physics and all particles
  fDecPhysicsList = new G4DecayPhysics(0);
  fRadDecayPhysicsList = new G4RadioactiveDecayPhysics(0);

  //Hadron physics
  fHadPhysicsList = new G4HadronPhysicsQGSP_BIC_AllHP(0);
}

RAONSCL3PhysicsList::~RAONSCL3PhysicsList()
{
  delete fEmPhysicsList;
  delete fDecPhysicsList;
  delete fRadDecayPhysicsList;
  delete fHadPhysicsList;
}

void RAONSCL3PhysicsList::ConstructParticle()
{
  G4Proton::ProtonDefinition();
  G4Gamma::GammaDefinition();
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4Neutron::NeutronDefinition();
  G4Deuteron::DeuteronDefinition();

  fDecPhysicsList->ConstructParticle();
}

void RAONSCL3PhysicsList::ConstructProcess()
{
  // Define transport process
  AddTransportation();

  // Electromagnetic physics list
  fEmPhysicsList->ConstructProcess();
  //em_config.AddModels();

  // Decay physics list
  fDecPhysicsList->ConstructProcess();
  fRadDecayPhysicsList->ConstructProcess();

  // Hadronic physics lists
  fHadPhysicsList->ConstructProcess();

  // Get the value of the fThickness of foil and target
  fThickness_foil;
  fThickness_target;

  // Update the cuts with the 1/2 of the thickness of the foil/target
  // SetCuts();
  SetCutTarget(0.01,
      fThickness_target*0.5,
      fThickness_target*0.5,
      fThickness_target*0.5,
      fThickness_target*0.5);
}


