#include <cstdlib>
#include <iostream>

#include "Armament/Solver/IArmamentSolver.h"
#include "Files/Input/IInputFile.h"

#include "Files/OutputFile.h"
#include "OutputController.h"
#include "DataStructs.h"
#include "SimulationController.h"
#include "Drone/DroneController.h"
#include "Target/TargetsManager.h"

int main(int argc, char** argv)
{
  if (argc != 3) {
    std::cerr << "usage: Need 2 additional arguments: for inputFile and Solver <input_path>\n";
    std::exit(1);
  }

  MissionFactory missionFactory{};

  IInputFile* inputFile = missionFactory.CreateInputFile(argv[1]);
  if (inputFile == nullptr) {
    std::cerr << "Input file didn`t initialzied. Exit!" << std::endl;
    std::exit(1);
  }

  IArmamentSolver* armamentSolver = missionFactory.CreateArmamentSolver(argv[2]);
  if (armamentSolver == nullptr) {
    std::cerr << "Armament solver didn`t initialzied. Exit!" << std::endl;
    std::exit(1);
  }

  DataStructs::InputData inputData = inputFile->ReadFile();
  delete inputFile;

  // create targets
  TargetsManager targetsManager{inputData.ArrayTimeStep, &missionFactory};

  // create drone controller
  DroneController droneController = {inputData, armamentSolver};
  droneController.LockTargets(targetsManager.GetTargetReferencies());

  // create simulation controller
  SimulationController simulation = {inputData.SimTestStep};
  float simulationStepTime = simulation.GetSimulationStepTime();

  // prepare controller for output
  OutputController outputController;
  while (simulation.IsWorking()) {
    droneController.OnStepStart(simulationStepTime);
    targetsManager.OnStepStart(simulationStepTime);

    outputController.AddData(droneController.GetDroneState());

    simulation.Update();

    targetsManager.OnStepEnd();
    droneController.OnStepEnd();

    if (droneController.isBombDropped()) {
      OutputFile output;
      output.WriteToFile(outputController.Outputs);
      return 0;
    }
  }

  return 0;
}