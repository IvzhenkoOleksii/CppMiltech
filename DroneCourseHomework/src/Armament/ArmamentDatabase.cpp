#include "Armament/ArmamentDatabase.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

// this structure need only to parse JSON
struct JsonData {
public:
  std::string Name;
  ArmamentDatabase::Data Data;
};

void from_json(const nlohmann::json& j, ArmamentDatabase::Data& data)
{
  j.at("mass").get_to(data.Mass);
  j.at("drag").get_to(data.Drag);
  j.at("lift").get_to(data.Lift);

  data.Type = 0;
  if (data.Lift > 0) {
    data.Type = 1;
  }
}

void from_json(const nlohmann::json& j, JsonData& armData)
{
  j.at("name").get_to(armData.Name);
  armData.Data = j.get<ArmamentDatabase::Data>();
}

std::map<std::string, ArmamentDatabase::Data> ArmamentDatabase::sArmaData;

ArmamentDatabase::Data ArmamentDatabase::GetArmament(const std::string& name, const std::string& filePath)
{
  if (sArmaData.empty()) {
    ReadAmmoFile(filePath);
  }

  auto search = sArmaData.find(name);
  if (search == sArmaData.end()) {
    std::cerr << "Error!. There is no data about: " << name << std::endl;
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

    std::vector<JsonData> jsonData = json.at("ammo").get<std::vector<JsonData>>();

    for (const auto& entity : jsonData) {
      sArmaData.insert({entity.Name, entity.Data});
    }
  }
  else {
    std::cerr << "Json Ammo file read error!  " << std::endl;
    exit(1);
  }
  file.close();
}