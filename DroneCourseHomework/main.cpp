#include <iostream>
#include <utility>

#include "Armament/Solver/IArmamentSolver.h"
#include "Files/Input/IInputFile.h"

#include "Files/OutputFile.h"
#include "OutputController.h"
#include "DataStructs.h"
#include "SimulationController.h"
#include "Drone/DroneController.h"
#include "Target/TargetsManager.h"
#include "MissionFactory.h"

int main(int argc, char** argv)
{
  if (argc != 3) {
    std::cerr << "usage: Need 2 additional arguments: for inputFile and Solver <input_path>\n";
    return 1;
  }

  MissionFactory missionFactory{};

  auto inputFile = missionFactory.CreateInputFile(argv[1]);
  auto armamentSolver = missionFactory.CreateArmamentSolver(argv[2]);
  DataStructs::InputData inputData = inputFile->ReadFile();

  // create targets
  TargetsManager targetsManager{inputData.ArrayTimeStep};

  // create drone controller
  DroneController droneController = {inputData, std::move(armamentSolver)};
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