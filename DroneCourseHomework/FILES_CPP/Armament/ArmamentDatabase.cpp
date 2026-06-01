#include "Armament/ArmamentDatabase.h"

#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>

#include <nlohmann/json.hpp>
#include <vector>

// this structure need only to parse JSON

struct JsonData {
public:
  std::string Name;
  ArmamentDatabase::Data Dt;
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
  armData.Dt = j.get<ArmamentDatabase::Data>();
}

std::map<std::string, ArmamentDatabase::Data> ArmamentDatabase::sArmaData;

ArmamentDatabase::Data ArmamentDatabase::GetArmament(const std::string& name)
{
  if (sArmaData.empty()) {
    ReadAmmoFile();
  }

  auto search = sArmaData.find(name);
  if (search == sArmaData.end()) {
    std::cout << "Error!. There is no data about: " << name << std::endl;
    exit(1);
  }
  else {
    ArmamentDatabase::Data searchedData = search->second;
    std::cout << "Data about " << name << "  mass: " << searchedData.Mass << "  drag: " << searchedData.Drag
              << "  lift: " << searchedData.Lift << std::endl;
    return searchedData;
  }
}

void ArmamentDatabase::ReadAmmoFile()
{
  std::ifstream file("./DroneCourseHomework/DataFiles/json/Ammo.json");

  if (file.is_open()) {
    nlohmann::json json = nlohmann::json::parse(file);

    std::vector<JsonData> jsonData = json.at("ammo").get<std::vector<JsonData>>();
    for (size_t i = 0; i < jsonData.size(); ++i) {
      sArmaData.insert({jsonData[i].Name, jsonData[i].Dt});
    }
  }
  else {
    std::cout << "Json Ammo file read error!  " << std::endl;
    exit(1);
  }
  file.close();
}