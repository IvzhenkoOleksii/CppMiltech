#include <atomic>
#include <iostream>
#include <ostream>
#include <string>

#include "Files/FileHelper.h"
#include "Files/Input/IInputFile.h"
#include "Files/OutputFile.h"

#include "OutputController.h"
#include "DataStructs.h"
#include "SimulationController.h"
#include "Drone/DroneController.h"
#include "Target/TargetsManager.h"
#include "MissionFactory.h"

struct Arguments {
  // order of arguments of build is same as here:
  std::string inputFilePath;
  std::string ammoFilePath;
  std::string targetsFilePath;
  std::string outputFilePath;
  std::string solverType;
  std::string tableSolverFilePath;
};

Arguments ArgumentsHandler(int argc, char** argv)
{
  if (argc < 6) {
    std::cerr << "Cannot start: Need at least 5 arguments: inputFilePath, ammoFilePath, targetsFilePath, outputFilePath, solverType and "
                 "solverFilePath if needed\n";
    std::exit(1);
  }

  Arguments arguments{};

  // argv[0] - path to executable
  // argv[1] - must be path or part of path to inputFile
  arguments.inputFilePath = FileHelper::CheckFilePath(argv[1]);

  // argv[2] - must be path or part of path to ammo data file
  arguments.ammoFilePath = FileHelper::CheckFilePath(argv[2]);

  // argv[3] - must be path or part of path to targets movement data file
  arguments.targetsFilePath = FileHelper::CheckFilePath(argv[3]);

  // argv[4] - must be path to future output file
  arguments.outputFilePath = argv[4];

  // argv[5] - must be type of ballistic solver
  arguments.solverType = argv[5];

  // if argv[5] == Table, we must have another argument - path to table data
  if (arguments.solverType == "Table") {
    if (argc < 7) {
      std::cerr << "Cannot start: Need one more argument: path to table data file" << std::endl;
      std::exit(1);
    }

    arguments.tableSolverFilePath = FileHelper::CheckFilePath(argv[6]);
  }

  return arguments;
}

std::atomic<bool> isSimulationEnded;
std::atomic<bool> isBombExploded;

int main(int argc, char** argv)
{
  // some functions definition
  void OnSimulationEnds(TargetsManager * targetsManager, DroneController * drone);
  void OnSimulationStepStarted(std::unique_ptr<SimulationController> simulation, std::unique_ptr<DroneController> droneController);
  void WriteDataToOutputFile(const std::string& filePath, const OutputController& controller);

  // here is main starts actually
  Arguments arguments = ArgumentsHandler(argc, argv);

  isSimulationEnded.store(false);
  isBombExploded.store(false);

  // factory creates all needed additional classes
  MissionFactory missionFactory{};
  auto inputFile = missionFactory.CreateInputFile(arguments.inputFilePath);
  auto armamentSolver = missionFactory.CreateArmamentSolver(arguments.solverType, arguments.tableSolverFilePath);

  // start work. Read input file
  DataStructs::InputData inputData = inputFile->ReadFile();

  // create targets
  TargetsManager targetsManager{arguments.targetsFilePath, inputData.TargetStepTime, inputData.ArrayTimeStep, inputData.TimeScale};

  // create drone controller
  DroneController droneController = {inputData, arguments.ammoFilePath, std::move(armamentSolver)};
  droneController.LockTargets(&targetsManager);

  // prepare controller for output
  OutputController outputController;

  // create simulation controller
  SimulationController simulation = {inputData.SimStepTime, inputData.TimeScale};  // previous sim step time
  simulation.LoopEndedAction = [&]() {
    if (!isSimulationEnded.exchange(true)) {
      isBombExploded.store(true);
      WriteDataToOutputFile(arguments.outputFilePath, outputController);
      OnSimulationEnds(&targetsManager, &droneController);
    }
  };

  droneController.BombExplodedAction = [&]() {
    if (!isBombExploded.exchange(true)) {
      simulation.FinishLoopThread();
    }
  };

  droneController.Start();

  simulation.LoopStepStartedAction = [&]() { outputController.AddData(droneController.GetDroneState()); };

  simulation.StartLoopThread();
  simulation.JoinThread();

  return 0;
}

void WriteDataToOutputFile(const std::string& filePath, const OutputController& controller)
{
  OutputFile output{filePath};
  output.WriteToFile(controller.Outputs);
}

void OnSimulationEnds(TargetsManager* targetsManager, DroneController* drone)
{
  targetsManager->FinishTargetsThreads();
  drone->Finish();
}
