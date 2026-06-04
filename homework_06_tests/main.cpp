#include <iostream>

#include "InputFile.h"
#include "DataStructs.h"
#include "DroneController.h"

auto main(int argc, char** argv) -> int
{
  std::cout << "argc:   " << argc << "   argv[1]:    " << argv[1] << std::endl;

  if (argc < 2) {
    std::cout << "Add path to data json as argument. exiting!" << std::endl;
    return 1;
  }

  // read input file
  std::string argument = argv[1];
  std::string path = PROJECT_ROOT_DIR + argument;

  InputFile inputFile;
  DataStructs::InputData inputData = inputFile.ReadJsonFile(path);

  // create drone controller
  DroneController droneController = {inputData.DroneData, inputData.SimTestStep};

  return 0;
}