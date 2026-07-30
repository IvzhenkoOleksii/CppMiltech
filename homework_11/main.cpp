#include <iostream>

#include <iostream>
#include <ostream>
#include <string>
#include <gpiod.h>
#include <unistd.h>
#include <unistd.h>
#include <cstring>

#include "Files/FileHelper.h"
#include "Files/Input/IInputFile.h"
#include "Files/OutputFile.h"
#include "OutputController.h"
#include "DataStructs.h"
#include "SimulationController.h"
#include "Drone/DroneController.h"
#include "Target/TargetsManager.h"
#include "MissionFactory.h"
#include "UART/UartController.h"

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

int main(int argc, char** argv)
{
  UartController testController;
  int answerCode = testController.OpenUart("/tmp/ttyA");

  SimulationController simulation = {0.05f};

  while (simulation.IsWorking()) {
    testController.ReadFrame();
    simulation.Update();
  }

  return 0;
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
  // Arguments arguments = ArgumentsHandler(argc, argv);

  // MissionFactory missionFactory{};

  // auto inputFile = missionFactory.CreateInputFile(arguments.inputFilePath);
  // auto armamentSolver = missionFactory.CreateArmamentSolver(arguments.solverType, arguments.tableSolverFilePath);
  // DataStructs::InputData inputData = inputFile->ReadFile();

  // // create targets
  // TargetsManager targetsManager{arguments.targetsFilePath, inputData.ArrayTimeStep};

  // // create drone controller
  // DroneController droneController = {inputData, arguments.ammoFilePath, std::move(armamentSolver)};
  // droneController.LockTargets(targetsManager.GetTargetReferencies());

  // // create simulation controller
  // SimulationController simulation = {inputData.SimTestStep};
  // float simulationStepTime = simulation.GetSimulationStepTime();

  // // prepare controller for output
  // OutputController outputController;
  // while (simulation.IsWorking()) {
  //   droneController.OnStepStart(simulationStepTime);
  //   targetsManager.OnStepStart(simulationStepTime);

  //   outputController.AddData(droneController.GetDroneState());

  //   simulation.Update();

  //   targetsManager.OnStepEnd();
  //   droneController.OnStepEnd();

  //   if (droneController.isBombDropped()) {
  //     OutputFile output{arguments.outputFilePath};
  //     output.WriteToFile(outputController.Outputs);
  //     return 0;
  //   }
  // }

  // return 0;
}