#include "Files/OutputFile.h"
#include <fstream>
#include <string>
#include <iostream>

const std::string FILE_PATH = "DataFiles/txt/Output.txt";

void OutputFile::WriteToFile(DataStructs::Position2D firePosition)
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

void OutputFile::WriteToFile(DataStructs::Position2D maneuverPosition, DataStructs::Position2D firePosition)
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