#include <iostream>

#include "InputFile.h"
#include "DataStructs.h"
#include "DroneController.h"
#include "OutputFile.h"
#include "TargetFile.h"

auto main(int argc, char** argv) -> int
{
  std::cout << "argc:   " << argc << "   argv[1]:    " << argv[1] << std::endl;

  if (argc < 2) {
    std::cout << "Add path to data json as argument. exiting!" << std::endl;
    return 1;
  }

  if (argc < 3) {
    std::cout << "Add path to targets txt file as argument. exiting!" << std::endl;
    return 1;
  }

  // read input file
  std::string argument = argv[1];
  std::string inputFilePath = PROJECT_ROOT_DIR + argument;
  InputFile inputFile;
  DataStructs::InputData inputData = inputFile.ReadJsonFile(inputFilePath);

  argument = argv[2];
  std::string targetFilePath = PROJECT_ROOT_DIR + argument;
  TargetFile targetFile;
  DataStructs::Position2D targetPosition = targetFile.ReadTxtFile(targetFilePath);

  // create drone controller
  DroneController droneController = {inputData.DroneData, inputData.SimTestStep};

  float minAttackDistance = droneController.GetFallDistance() + inputData.DroneData.AccelerationPath;
  float droneToTargetDistance = droneController.GetDistanceToTarget(targetPosition);

  OutputFile outputFile;
  if (minAttackDistance > droneToTargetDistance) {
    std::cout << "Min attack distance higher, than current distance to target  " << std::endl;
    DataStructs::Position2D maneuverPosition =
      droneController.GetManeuverPosition(minAttackDistance, droneToTargetDistance, targetPosition);
    DataStructs::Position2D firePosition =
      droneController.GetFirePosition(droneController.GetFallDistance(), minAttackDistance, maneuverPosition, targetPosition);

    outputFile.WriteToFile(PROJECT_ROOT_DIR, maneuverPosition, firePosition);
  }
  else {
    DataStructs::Position2D firePosition = droneController.GetFirePosition(
      droneController.GetFallDistance(), droneToTargetDistance, droneController.GetCurrentPosition(), targetPosition);
    outputFile.WriteToFile(PROJECT_ROOT_DIR, firePosition);
  }

  return 0;
}