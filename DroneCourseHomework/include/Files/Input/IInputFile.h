#pragma once
#include "DataStructs.h"

class IInputFile {
public:
  virtual ~IInputFile() = default;
  virtual DataStructs::InputData ReadFile() = 0;
};