#include <iostream>
#include <fstream>

#include "TargetFile.h"
#include "DataStructs.h"

auto TargetFile::ReadJsonFile() -> DataStructs::TargetData
{
  std::ifstream file("./DroneCourseHomework/DataFiles/json/Targets.json");
  DataStructs::TargetData targetData{};

  if (file.is_open()) {
    nlohmann::json json = nlohmann::json::parse(file);
    json.at("paths").get_to(targetData.Positions);
  }
  else {
    std::cout << "Json Targets file read error!  " << '\n';
    exit(1);
  }
  file.close();

  return targetData;
}
