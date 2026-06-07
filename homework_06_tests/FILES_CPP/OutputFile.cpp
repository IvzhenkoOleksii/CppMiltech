#include "OutputFile.h"
#include <fstream>
#include <string>
#include <iostream>

const std::string FILE_PATH = "DataFiles/txt/Output.txt";

void OutputFile::WriteToFile(std::string projectPath, DataStructs::Position2D firePosition)
{
  std::string fullPath = projectPath + "/" + FILE_PATH;
  std::ofstream file(fullPath);
  if (file.is_open()) {
    file << firePosition.X << " " << firePosition.Y;
    file.close();
  }
  else {
    std::cout << "Output file open error!  " << '\n';
    exit(1);
  }
}

void OutputFile::WriteToFile(std::string projectPath, DataStructs::Position2D maneuverPosition, DataStructs::Position2D firePosition)
{
  std::string fullPath = projectPath + "/" + FILE_PATH;
  std::ofstream file(fullPath);
  if (file.is_open()) {
    file << maneuverPosition.X << " " << maneuverPosition.Y << " ";
    file << firePosition.X << " " << firePosition.Y;
    file.close();
  }
  else {
    std::cout << "Output file open error!  " << '\n';
    exit(1);
  }
}