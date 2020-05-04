#ifndef RAONSCL3PhysicsList_h
#define RAONSCL3PhysicsList_h 1

#include "B1DetectorConstruction.hh"
#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include <vector>

class G4PhysicsConstructor;
class B1DetectorConstruction;

class RAONSCL3PhysicsList: public G4VModularPhysicsList
{
  public:
    RAONSCL3PhysicsList(B1DetectorConstruction* det);
    virtual ~RAONSCL3PhysicsList();
    void ConstructParticle();
    void ConstrctProcess();

    void SetCuts();
    void SetCutForGamma(G4double);
    void SetCutForElectron(G4double);
    void SetCtForPositron(G4double);
    void SetCutFoil(G4double cutProton,
                    G4double cutElectron,
                    G4double cutPositron,
                    G4double cutGamma,
                    G4double cutNeutron);
    void SetCutTarget(G4double cutProton,
                      G4double cutElectron,
                      G4double cutPositron,
                      G4double cutGamma,
                      G4double cutNeutron);

  private:
    G4String                fEmName;
    G4VPhysicsConstructor*  fEmPhysicsList;
    G4VPhysicsConstructor*  fDecPhysicsList;
    G4VPhysicsConstructor*  fHadPhysicsList;
    G4VPhysicsConstructor*  fRadDecayPhysicsList;

    G4double fThickness_target;
    G4double fThickness_foil;

    G4double fCutForGamma;
    G4double fCutForElectron;
    G4double fCutForPositron;

    G4double fCutTargetProton;
    G4double fCutTargetElectron;
    G4double fCutTargetPositron;
    G4double fCutTargetGamma;
    G4double fCutTargetNeutron;

    G4double fCutFoilProton;
    G4double fCutFoilElectron;
    G4double fCutFoilPositron;
    G4double fCutFoilGamma;
    G4double fCutFoilNeutron;

    B1DetectorConstruction* fDetector;
};
#endif
