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
  DroneController droneController = {inputData.DroneData, inputData.SimTestStep};
  droneController.LockTargets(targetsManager.GetTargetReferencies());
  droneController.ChooseNearestTarget();

  // create simulation controller
  SimulationController simulation = {inputData.SimTestStep};
  float simulationStepTime = simulation.GetSimulationStepTime();

  while (simulation.IsWorking()) {
    targetsManager.OnStepStart(simulationStepTime);
    droneController.RecalculateOperationalStates(simulationStepTime);

    simulation.Update();

    targetsManager.OnStepEnd();

    droneController.UpdateOperationalStates();
    droneController.CheckIfDroneReachedFirePosition();
  }

  return 0;
}