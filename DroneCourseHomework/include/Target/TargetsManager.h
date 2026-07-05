#pragma once
#include "DataStructs.h"
#include "TargetController.h"

#include <vector>

class TargetsManager {
public:
  TargetsManager(const std::string& filePath);
  TargetsManager(const std::string& filePath, const float& arrayTimeStep);

public:
  void OnStepStart(const float& simStep);
  void OnStepEnd();

public:
  std::vector<TargetController*> GetTargetReferencies();

private:
  DataStructs::TargetData targetData;
  std::vector<TargetController> targets;
};