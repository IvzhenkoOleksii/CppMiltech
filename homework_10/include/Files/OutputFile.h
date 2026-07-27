#pragma once

#include "DataStructs.h"
#include "OutputController.h"

class OutputFile {
public:
  OutputFile(const std::string& filePath);

  void WriteToFile(DataStructs::Coord2D firePosition);
  void WriteToFile(DataStructs::Coord2D maneuverPosition, DataStructs::Coord2D firePosition);
  void WriteToFile(OutputController::Output output);

private:
  std::string filePath;
};