#pragma once

#include "DataStructs.h"
#include "Files/Input/IInputFile.h"

class InputFileJson : public IInputFile {
public:
  InputFileJson(const std::string& filename);
  ~InputFileJson();
  DataStructs::InputData ReadFile() override;
};