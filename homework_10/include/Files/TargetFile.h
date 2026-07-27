#pragma once
#include "DataStructs.h"

class TargetFile {
public:
  DataStructs::TargetData ReadJsonFile(const std::string& filePath);
};