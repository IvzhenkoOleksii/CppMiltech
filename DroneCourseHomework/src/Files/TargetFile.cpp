#include <iostream>
#include <fstream>
#include <string>

#include "Files/TargetFile.h"
#include "DataStructs.h"

DataStructs::TargetData TargetFile::ReadJsonFile(const std::string& filePath)
{
  std::ifstream file(filePath);
  DataStructs::TargetData targetData{};

  if (file.is_open()) {
    nlohmann::json json = nlohmann::json::parse(file);
    json.at("paths").get_to(targetData.Positions);
  }
  else {
    std::cerr << "Json Targets file read error!  " << std::endl;
    exit(1);
  }
  file.close();

  return targetData;
}
