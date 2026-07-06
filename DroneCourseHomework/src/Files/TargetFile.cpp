#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Files/TargetFile.h"
#include "DataStructs.h"

DataStructs::TargetData TargetFile::ReadJsonFile(const std::string& filePath)
{
  std::ifstream file(filePath);
  DataStructs::TargetData targetData{};
  targetData.Positions = {};

  if (file.is_open()) {
    nlohmann::json json = nlohmann::json::parse(file);
    nlohmann::json j_targets = json.at("targets");
    for (size_t i = 0; i < j_targets.size(); ++i) {
      nlohmann::json j_positions = j_targets[i].at("positions");
      std::vector<DataStructs::Coord2D> positions;
      j_positions.get_to(positions);
      targetData.Positions.push_back(positions);
    }
  }
  else {
    std::cerr << "Json Targets file read error!  " << std::endl;
    exit(1);
  }
  file.close();

  return targetData;
}
