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

    json.at("position").get_to<DataStructs::Coord3D>(inputData.DroneData.Position);
    json.at("initialDirection").get_to(inputData.DroneData.InitialDirection);
    json.at("attackSpeed").get_to(inputData.DroneData.AttackSpeed);
    json.at("accelerationPath").get_to(inputData.DroneData.AccelerationPath);
    json.at("ammoType").get_to(inputData.DroneData.AmmoType);
    json.at("arrayTimeStep").get_to(inputData.ArrayTimeStep);
    json.at("simTimeStep").get_to(inputData.SimStepTime);
    json.at("hitRadius").get_to(inputData.HitRadius);
    json.at("angularSpeed").get_to(inputData.DroneData.AngularSpeed);
    json.at("turnThreshold").get_to(inputData.DroneData.TurnThreshold);
    json.at("targetTimeStep").get_to(inputData.TargetStepTime);
    json.at("physicsTimeStep").get_to(inputData.PhysicsStepTime);
    json.at("timeScale").get_to(inputData.TimeScale);
  }
  else {
    std::cout << "Json Input file read error!  " << std::endl;
    exit(1);
  }
  file.close();

  return inputData;
}