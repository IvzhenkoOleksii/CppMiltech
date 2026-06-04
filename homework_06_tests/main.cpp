#include <string>

#include "InputFile.h"
#include "DataStructs.h"
#include "DroneController.h"

auto main() -> int
{
  // read input file
  InputFile inputFile;
  DataStructs::InputData inputData = inputFile.ReadJsonFile("./DroneCourseHomework/DataFiles/json/Input.json");

  // create drone controller
  DroneController droneController = {inputData.DroneData, inputData.SimTestStep};

  return 0;
}