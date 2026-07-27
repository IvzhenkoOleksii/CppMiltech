#include "Files/Input/IInputFile.h"
#include "Files/Input/InputFileJson.h"
#include "DataStructs.h"

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

InputFileJson::InputFileJson(const std::string& filePath)
  : IInputFile(filePath)
{
}

InputFileJson::~InputFileJson() {}

DataStructs::InputData InputFileJson::ReadFile()
{
  std::ifstream file(filePath);
  DataStructs::InputData inputData{};
  if (file.is_open()) {
    nlohmann::json json = nlohmann::json::parse(file);

    json.at("ammo").get_to(inputData.DroneData.AmmoType);
    json.at("targetArrayTimeStep").get_to(inputData.ArrayTimeStep);

    DataStructs::DroneJsonState droneJsonState;
    json.at("drone").get_to(droneJsonState);

    DataStructs::SimulationJsonState simJsonState;
    json.at("simulation").get_to(simJsonState);

    inputData.DroneData.Position = {droneJsonState.position.X, droneJsonState.position.Y, droneJsonState.altitude};
    inputData.DroneData.InitialDirection = droneJsonState.initialDirection;
    inputData.DroneData.AngularSpeed = droneJsonState.angularSpeed;
    inputData.DroneData.AccelerationPath = droneJsonState.accelerationPath;
    inputData.DroneData.AttackSpeed = droneJsonState.attackSpeed;
    inputData.DroneData.TurnThreshold = droneJsonState.turnThreshold;

    inputData.SimStepTime = simJsonState.timeStep;
    inputData.TargetStepTime = simJsonState.targetTimeStep;
    inputData.PhysicsStepTime = simJsonState.physicsTimeStep;
    inputData.HitRadius = simJsonState.hitRadius;
    inputData.TimeScale = simJsonState.timeScale;
  }
  else {
    std::cout << "Json Input file read error!  " << std::endl;
    exit(1);
  }
  file.close();

  return inputData;
}