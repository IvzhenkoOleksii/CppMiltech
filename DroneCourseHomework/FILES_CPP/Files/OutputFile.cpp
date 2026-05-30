#include "Files/OutputFile.h"
#include <fstream>
#include <string>
#include <iostream>

const std::string FILE_PATH = "Output.json";

void OutputFile::WriteToFile(DataStructs::Point2D firePosition)
{
  std::ofstream file(FILE_PATH);
  if (file.is_open()) {
    file << firePosition.X << " " << firePosition.Y;
    file.close();
  }
  else {
    std::cout << "Output file open error!  " << std::endl;
    exit(1);
  }
}

void OutputFile::WriteToFile(DataStructs::Point2D maneuverPosition, DataStructs::Point2D firePosition)
{
  std::ofstream file(FILE_PATH);
  if (file.is_open()) {
    file << maneuverPosition.X << " " << maneuverPosition.Y << " ";
    file << firePosition.X << " " << firePosition.Y;
    file.close();
  }
  else {
    std::cout << "Output file open error!  " << std::endl;
    exit(1);
  }
}

void OutputFile::WriteToFile(OutputController::Output output)
{
  std::ofstream file(FILE_PATH);
  if (file.is_open()) {
    nlohmann::json json = output;
    file << json;
    file.close();
  }
  else {
    std::cout << "Output file open error!  " << std::endl;
    exit(1);
  }
}