#pragma once

#include <string>

#include "DataStructs.h"
#include "Files/Input/IInputFile.h"

class InputFileTxt : public IInputFile {
public:
  ~InputFileTxt();
  DataStructs::InputData ReadFile() override;

private:
  void ReadLine(std::string line, int lineIndex, DataStructs::InputData* data);
  void SetupDronePosition(const std::string& dataAsVector, DataStructs::InputData* data);
};