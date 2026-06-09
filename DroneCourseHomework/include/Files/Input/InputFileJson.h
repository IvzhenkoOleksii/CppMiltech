#pragma once

#include "DataStructs.h"
#include "Files/Input/IInputFile.h"

class InputFileJson : public IInputFile {
public:
  ~InputFileJson();
  DataStructs::InputData ReadFile() override;
};