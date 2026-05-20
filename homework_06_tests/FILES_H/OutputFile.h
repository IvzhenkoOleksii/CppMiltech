#pragma once
#include "DataStructs.h"

class OutputFile {
public:
  void WriteToFile(DataStructs::Position2D firePosition);

public:
  void WriteToFile(DataStructs::Position2D maneuverPosition, DataStructs::Position2D firePosition);
};