#pragma once
#include "DataStructs.h"
#include "TargetController.h"

#include <vector>

class TargetsManager {
public:
  TargetsManager();

public:
  TargetsManager(const float& arrayTimeStep);

public:
  void UpdateTargets(const float& simStep);

public:
  std::vector<TargetController*> GetTargetReferencies();

private:
  DataStructs::TargetData targetData;

private:
  std::vector<TargetController> targets;
};