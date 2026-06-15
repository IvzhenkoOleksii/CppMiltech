#include <iostream>
#include <ostream>
#include <thread>
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

#include "Test/Class1.h"

int main(int argc, char** argv)
{
  // some functions definition
  void OnSimulationEnds();
  void OnSimulationStepStarted(std::unique_ptr<SimulationController> simulation, std::unique_ptr<DroneController> droneController);
  void WriteDataToOutputFile(const OutputController& controller);

  // Class1 class1{};
  // Class2 class2{};

  // class1.InsertFunction([&class2]() { return class2.GetPassedSteps(); });
  // class2.LoopEndedAction = [&class1]() { class1.FinishThreadLoop(); };

  // class1.StartWork();
  // class2.StartWork();

  // class1.JoinThread();
  // class2.JoinThread();

  // std::cout << "END" << std::endl;
  // return 0;

  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //

  if (argc != 3) {
    std::cerr << "usage: Need 2 additional arguments: for inputFile and Solver <input_path>\n";
    return 1;
  }

  MissionFactory missionFactory{};

  auto inputFile = missionFactory.CreateInputFile(argv[1]);
  auto armamentSolver = missionFactory.CreateArmamentSolver(argv[2]);
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
    OnSimulationEnds();
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

  // while (simulation.IsWorking()) {
  //   droneController.OnStepStart(simulationStepTime);
  //   targetsManager.OnStepStart(simulationStepTime);

  //   outputController.AddData(droneController.GetDroneState());

  //   simulation.Update();

  //   targetsManager.OnStepEnd();
  //   droneController.OnStepEnd();

  //   if (droneController.isBombDropped()) {
  //     OutputFile output;
  //     output.WriteToFile(outputController.Outputs);
  //     return 0;
  //   }
  // }

  return 0;
}

void WriteDataToOutputFile(const OutputController& controller)
{
  OutputFile output;
  output.WriteToFile(controller.Outputs);
}

void OnSimulationEnds()
{
  std::cout << "INNER FUNCTION ON SIMULATION ENDS";
}

void OnSimulationStepStarted(std::unique_ptr<SimulationController> simulation, std::unique_ptr<DroneController> droneController)
{
  float stepTime = simulation->GetStepTime();
  droneController->OnStepStart(stepTime);
}
