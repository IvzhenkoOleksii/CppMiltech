#pragma once
#include "DataStructs.h"
#include "TargetController.h"

#include <vector>

class TargetsManager {
public:
  TargetsManager();
  TargetsManager(const float& arrayTimeStep);

public:
  void OnStepStart(const float& simStep);
  void OnStepEnd();

public:
  std::vector<TargetController*> GetTargetReferencies();

private:
  DataStructs::TargetData targetData;
  std::vector<TargetController> targets;
};