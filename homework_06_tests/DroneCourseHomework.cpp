#include <iostream>
#include <fstream>
#include <string>

#include "InputFile.h"
#include "OutputFile.h"
#include "TargetFile.h"
#include "DataStructs.h"
#include "SimulationController.h"
#include "DroneController.h"
#include "DroneCalculator.h"
#include "TargetController.h"
#include "TargetsManager.h"

int main()
{
    // read input file
    InputFile inputFile;
    DataStructs::InputData inputData = inputFile.ReadJsonFile();

    // create drone controller
    DroneController droneController = { inputData.DroneData, inputData.SimTestStep };

    return 0;
}