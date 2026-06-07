#pragma once
#include "DataStructs.h"

#include <string>

class InputFile {
public:
  DataStructs::InputData ReadTxtFile();

public:
  DataStructs::InputData ReadJsonFile(const std::string& path);

  // private methods
private:
  void ReadLine(const std::string& line, int lineIndex, DataStructs::InputData* data);
  void SetupDronePosition(const std::string& dataAsVector, DataStructs::InputData* data);
  void SetFloatField(const std::string& input, int index, float* output);
};