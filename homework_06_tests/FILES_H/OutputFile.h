#pragma once
#include "DataStructs.h"

class OutputFile {
public:
  void WriteToFile(std::string path, DataStructs::Position2D firePosition);

public:
  void WriteToFile(std::string path, DataStructs::Position2D maneuverPosition, DataStructs::Position2D firePosition);
};