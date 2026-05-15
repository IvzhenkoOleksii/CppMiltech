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

    // create targets
    TargetsManager targetsManager{ inputData.ArrayTimeStep };

    // create drone controller
    DroneController droneController = { inputData.DroneData, inputData.SimTestStep };
    droneController.LockTargets(targetsManager.GetTargetReferencies());
    droneController.ChooseNearestTarget();

    // create simulation controller
    SimulationController simulation = { inputData.SimTestStep };
    float simulationStepTime = simulation.GetSimulationStepTime();

    while (simulation.IsWorking())
    {
        droneController.RecalculateOperationalStates(simulationStepTime);

        simulation.Update();

        targetsManager.UpdateTargets(simulationStepTime);
        droneController.UpdateOperationalStates();
        droneController.CheckIfDroneReachedFirePosition();
    }

    return 0;
}