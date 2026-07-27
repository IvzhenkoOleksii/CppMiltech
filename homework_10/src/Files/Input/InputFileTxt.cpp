
#include "Files/Input/IInputFile.h"
#include "Files/Input/InputFileTxt.h"
#include "DataStructs.h"

#include <iostream>
#include <fstream>
#include <vector>

InputFileTxt::InputFileTxt(const std::string& filePath)
  : IInputFile(filePath)
{
}

InputFileTxt::~InputFileTxt() {}

DataStructs::InputData InputFileTxt::ReadFile()
{
  std::string line;
  std::ifstream file(filePath);
  DataStructs::InputData inputData{};

  if (file.is_open()) {
    int lineIndex = 0;
    while (std::getline(file, line)) {
      std::cout << "Line inside input file is:  " << line << std::endl;
      ReadLine(line, lineIndex, &inputData);
      lineIndex++;
    }
  }
  else {
    std::cout << "Input file read error!  " << line << std::endl;
    exit(1);
  }
  file.close();

  inputData.CheckData();
  return inputData;
}

void SetFloatField(const std::string& input, int index, float* output)
{
  try {
    if (input.empty()) {
      std::cout << "Empty input field at index: " << index << "\n";
      exit(1);
    }

    *output = std::stof(input);
  }
  catch (const std::exception&) {
    std::cout << "   An Error inside float Input file. Index of error field:   " << index << "\n";
    exit(1);
  }
}

void SetIntField(const std::string& input, int index, int* output)
{
  try {
    if (input.empty()) {
      std::cout << "Empty input field at index: " << index << "\n";
      exit(1);
    }

    *output = std::stoi(input);
  }
  catch (const std::exception&) {
    std::cout << "   An Error inside integer Input file. Index of error field:   " << index << "\n";
    exit(1);
  }
}

void InputFileTxt::ReadLine(std::string line, int lineIndex, DataStructs::InputData* data)
{
  switch (lineIndex) {
    case 0:
      SetupDronePosition(line, data);
      break;
    case 1:
      SetFloatField(line, lineIndex, &data->DroneData.InitialDirection);
      break;
    case 2:
      SetFloatField(line, lineIndex, &data->DroneData.AttackSpeed);
      break;
    case 3:
      SetFloatField(line, lineIndex, &data->DroneData.AccelerationPath);
      break;
    case 4:
      data->DroneData.AmmoType = line;
      break;
    case 5:
      SetFloatField(line, lineIndex, &data->ArrayTimeStep);
      break;
    case 6:
      SetFloatField(line, lineIndex, &data->SimStepTime);
      break;
    case 7:
      SetFloatField(line, lineIndex, &data->HitRadius);
      break;
    case 8:
      SetFloatField(line, lineIndex, &data->DroneData.AngularSpeed);
      break;
    case 9:
      SetFloatField(line, lineIndex, &data->DroneData.TurnThreshold);
      break;
    case 10:
      SetFloatField(line, lineIndex, &data->TargetStepTime);
      break;
    case 11:
      SetFloatField(line, lineIndex, &data->PhysicsStepTime);
      break;
    case 12:
      SetIntField(line, lineIndex, &data->TimeScale);
      break;
    default:
      break;
  }
}

void InputFileTxt::SetupDronePosition(const std::string& line, DataStructs::InputData* data)
{
  std::stringstream splitted(line);
  std::string word;
  std::vector<std::string> dataAsVector;

  while (splitted >> word) {
    dataAsVector.push_back(word);
  }

  if (dataAsVector.size() < 3) {
    std::cerr << "Input data is not full. Check input file.Drone Position line! " << "\n";
    exit(1);
  }

  float x, y, z;
  for (size_t index = 0; index < dataAsVector.size(); index++) {
    switch (index) {
      case 0:
        SetFloatField(dataAsVector[index], index, &x);
        break;
      case 1:
        SetFloatField(dataAsVector[index], index, &y);
        break;
      case 2:
        SetFloatField(dataAsVector[index], index, &z);
        break;
      default:
        break;
    }
  }

  data->DroneData.Position = {x, y, z};
}
