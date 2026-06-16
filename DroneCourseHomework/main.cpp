#include <iostream>
#include <ostream>
#include <utility>
#include <memory>

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
  // some functions definition
  void OnSimulationEnds(TargetsManager * targetsManager);
  void OnSimulationStepStarted(std::unique_ptr<SimulationController> simulation, std::unique_ptr<DroneController> droneController);
  void WriteDataToOutputFile(const OutputController& controller);

  // here is main starts actually
  if (argc != 3) {
    std::cerr << "usage: Need 2 additional arguments: for inputFile and Solver <input_path>\n";
    return 1;
  }

  // factory creates all needed additional classes
  MissionFactory missionFactory{};
  auto inputFile = missionFactory.CreateInputFile(argv[1]);
  auto armamentSolver = missionFactory.CreateArmamentSolver(argv[2]);

  // start work. Read input file
  DataStructs::InputData inputData = inputFile->ReadFile();

  // create targets
  TargetsManager targetsManager{inputData.SimTestStep, inputData.ArrayTimeStep};

  // create drone controller
  DroneController droneController = {inputData, std::move(armamentSolver)};
  droneController.LockTargets(&targetsManager);

  // prepare controller for output
  OutputController outputController;

  // create simulation controller
  SimulationController simulation = {inputData.SimTestStep};
  simulation.LoopEndedAction = [&]() {
    WriteDataToOutputFile(outputController);
    OnSimulationEnds(&targetsManager);
  };

  simulation.LoopStepStartedAction = [&]() {
    float stepTime = simulation.GetStepTime();
    droneController.OnStepStart(stepTime);
    outputController.AddData(droneController.GetDroneState());
  };

  simulation.LoopStepEndedAction = [&]() {
    droneController.OnStepEnd();

    if (droneController.isBombDropped()) {
      simulation.FinishLoopThread();
    }
  };

  simulation.StartLoopThread();
  simulation.JoinThread();

  return 0;
}

void WriteDataToOutputFile(const OutputController& controller)
{
  OutputFile output;
  output.WriteToFile(controller.Outputs);
}

void OnSimulationEnds(TargetsManager* targetsManager)
{
  targetsManager->FinishTargetsThreads();
}
