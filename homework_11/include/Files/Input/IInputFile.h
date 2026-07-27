#pragma once
#include <string>
#include "DataStructs.h"

class IInputFile {
public:
  IInputFile(const std::string& filePath);
  virtual ~IInputFile() = default;
  virtual DataStructs::InputData ReadFile() = 0;

protected:
  std::string filePath;
};