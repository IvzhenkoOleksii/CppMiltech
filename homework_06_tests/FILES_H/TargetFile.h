#pragma once
#include "DataStructs.h"

class TargetFile {
public:
  DataStructs::TargetData ReadJsonFile();
  DataStructs::Position2D ReadTxtFile(const std::string& path);
};