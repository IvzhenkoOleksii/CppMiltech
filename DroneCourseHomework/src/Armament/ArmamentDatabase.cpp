#include "Armament/ArmamentDatabase.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

void from_json(const nlohmann::json& j, ArmamentDatabase::Data& data)
{
  j.at("name").get_to(data.Name);
  j.at("mass").get_to(data.Mass);
  j.at("drag").get_to(data.Drag);
  j.at("lift").get_to(data.Lift);
  j.at("lift").get_to(data.Type);

  data.Type = 0;
  if (data.Lift > 0) {
    data.Type = 1;
  }
}

std::map<std::string, ArmamentDatabase::Data> ArmamentDatabase::sArmaData;

ArmamentDatabase::Data ArmamentDatabase::GetArmament(const std::string& name, const std::string& filePath)
{
  if (sArmaData.empty()) {
    ReadAmmoFile(filePath);
  }

  auto search = sArmaData.find(name);
  if (search == sArmaData.end()) {
    std::cout << "Error!. There is no data about: " << name << std::endl;
    exit(1);
  }
  else {
    ArmamentDatabase::Data searchedData = search->second;
    // std::cout << "Data about " << name << "  mass: " << searchedData.Mass << "  drag: " << searchedData.Drag
    //           << "  lift: " << searchedData.Lift << std::endl;
    return searchedData;
  }
}

void ArmamentDatabase::ReadAmmoFile(const std::string& filePath)
{
  std::ifstream file(filePath);

  if (file.is_open()) {
    nlohmann::json json = nlohmann::json::parse(file);

    std::vector<ArmamentDatabase::Data> jsonData = json.get<std::vector<ArmamentDatabase::Data>>();

    for (const auto& entity : jsonData) {
      sArmaData.insert({entity.Name, entity});
    }
  }
  else {
    std::cout << "Json Ammo file read error!  " << std::endl;
    exit(1);
  }
  file.close();
}