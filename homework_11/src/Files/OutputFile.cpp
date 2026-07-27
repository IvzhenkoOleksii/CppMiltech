#include "Files/OutputFile.h"
#include <fstream>
#include <string>
#include <iostream>

OutputFile::OutputFile(const std::string& filePath)
{
  this->filePath = filePath;
}

void OutputFile::WriteToFile(DataStructs::Coord2D firePosition)
{
  std::ofstream file(filePath);
  if (file.is_open()) {
    file << firePosition.X << " " << firePosition.Y;
    file.close();
  }
  else {
    std::cerr << "Output file open error!  " << std::endl;
    exit(1);
  }
}

void OutputFile::WriteToFile(DataStructs::Coord2D maneuverPosition, DataStructs::Coord2D firePosition)
{
  std::ofstream file(filePath);
  if (file.is_open()) {
    file << maneuverPosition.X << " " << maneuverPosition.Y << " ";
    file << firePosition.X << " " << firePosition.Y;
    file.close();
  }
  else {
    std::cerr << "Output file open error!  " << std::endl;
    exit(1);
  }
}

void OutputFile::WriteToFile(OutputController::Output output)
{
  std::ofstream file(filePath);
  if (file.is_open()) {
    nlohmann::json json = output;
    file << json;
    file.close();
  }
  else {
    std::cerr << "Output file open error!  " << std::endl;
    exit(1);
  }
}