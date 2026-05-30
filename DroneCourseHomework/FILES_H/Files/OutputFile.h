#pragma once
#include "DataStructs.h"
#include "OutputController.h"

class OutputFile {
public:
  void WriteToFile(DataStructs::Point2D firePosition);
  void WriteToFile(DataStructs::Point2D maneuverPosition, DataStructs::Point2D firePosition);
  void WriteToFile(OutputController::Output output);
};