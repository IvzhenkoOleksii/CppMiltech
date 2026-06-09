#pragma once
#include "DataStructs.h"
#include "OutputController.h"

class OutputFile {
public:
  void WriteToFile(DataStructs::Coord2D firePosition);
  void WriteToFile(DataStructs::Coord2D maneuverPosition, DataStructs::Coord2D firePosition);
  void WriteToFile(OutputController::Output output);
};