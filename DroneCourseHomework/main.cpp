#include <iostream>
#include <ostream>
#include "Files/InputFile.h"
#include "Files/OutputFile.h"
#include "DataStructs.h"
#include "SimulationController.h"
#include "Drone/DroneController.h"
#include "Target/TargetsManager.h"

int main()
{
  // read input file
  InputFile inputFile;
  DataStructs::InputData inputData = inputFile.ReadJsonFile();

  // create targets
  TargetsManager targetsManager{inputData.ArrayTimeStep};

  // create drone controller
  DroneController droneController = {inputData};
  droneController.LockTargets(targetsManager.GetTargetReferencies());

  // create simulation controller
  SimulationController simulation = {inputData.SimTestStep};
  float simulationStepTime = simulation.GetSimulationStepTime();

  while (simulation.IsWorking()) {
    droneController.OnStepStart(simulationStepTime);
    targetsManager.OnStepStart(simulationStepTime);

    simulation.Update();

    targetsManager.OnStepEnd();
    droneController.OnStepEnd();
  }

  return 0;
}