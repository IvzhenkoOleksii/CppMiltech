#include <iostream>
#include <fstream>

#include "Files/Input/InputFileJson.h"
#include "DataStructs.h"

#include <nlohmann/json.hpp>

InputFileJson::~InputFileJson() {}

DataStructs::InputData InputFileJson::ReadFile()
{
  std::ifstream file("./DroneCourseHomework/DataFiles/json/Input.json");
  DataStructs::InputData inputData{};
  if (file.is_open()) {
    nlohmann::json json = nlohmann::json::parse(file);

    json.at("position").get_to<DataStructs::Coord3D>(inputData.DroneData.Position);
    json.at("initialDirection").get_to(inputData.DroneData.InitialDirection);
    json.at("attackSpeed").get_to(inputData.DroneData.AttackSpeed);
    json.at("accelerationPath").get_to(inputData.DroneData.AccelerationPath);
    json.at("ammoType").get_to(inputData.DroneData.AmmoType);
    json.at("arrayTimeStep").get_to(inputData.ArrayTimeStep);
    json.at("simTimeStep").get_to(inputData.SimTestStep);
    json.at("hitRadius").get_to(inputData.HitRadius);
    json.at("angularSpeed").get_to(inputData.DroneData.AngularSpeed);
    json.at("turnThreshold").get_to(inputData.DroneData.TurnThreshold);
  }
  else {
    std::cout << "Json Input file read error!  " << std::endl;
    exit(1);
  }
  file.close();

  return inputData;
}