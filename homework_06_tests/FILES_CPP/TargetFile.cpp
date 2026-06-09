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
    std::cerr << "Json Targets file read error!  " << '\n';
    exit(1);
  }
  file.close();

  return targetData;
}

DataStructs::Position2D TargetFile::ReadTxtFile(const std::string& path)
{
  std::ifstream file(path);
  DataStructs::Position2D targetPosition;
  if (file.is_open()) {
    std::string fileAsLine;
    std::getline(file, fileAsLine);
    std::stringstream splitted(fileAsLine);
    std::string word;
    std::vector<std::string> lines;

    while (splitted >> word) {
      lines.push_back(word);
    }

    targetPosition.X = std::stof(lines[0]);
    targetPosition.Y = std::stof(lines[1]);
  }
  else {
    std::cerr << "Txt Targets file read error!  " << '\n';
    exit(1);
  }
  file.close();
  return targetPosition;
}
