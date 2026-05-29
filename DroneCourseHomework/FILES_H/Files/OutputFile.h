#pragma once
#include "DataStructs.h"

class OutputFile {
public:
  void WriteToFile(DataStructs::Point2D firePosition);

public:
  void WriteToFile(DataStructs::Point2D maneuverPosition, DataStructs::Point2D firePosition);
};