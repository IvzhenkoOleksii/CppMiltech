#pragma once
#include "DataStructs.h"
#include "ITargetController.h"
#include "MissionFactory.h"

#include <vector>

class TargetsManager {
public:
  TargetsManager();
  TargetsManager(const float& arrayTimeStep, MissionFactory* factory);

public:
  void OnStepStart(const float& simStep);
  void OnStepEnd();

public:
  std::vector<ITargetController*> GetTargetReferencies();

private:
  DataStructs::TargetData targetData;
  std::vector<ITargetController*> targets;
};