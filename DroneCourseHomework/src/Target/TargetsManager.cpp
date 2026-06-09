#include "Target/TargetsManager.h"
#include "Target/TargetController.h"
#include "Files/TargetFile.h"
#include "DataStructs.h"

#include <vector>

TargetsManager::TargetsManager()
{
  // read target file
  TargetFile targetFile;
  targetData = targetFile.ReadJsonFile();
}

TargetsManager::TargetsManager(const float& arrayTimeStep)
  : TargetsManager()
{
  for (const auto& targetPosition : targetData.Positions) {
    targets.push_back(TargetController{targetPosition, arrayTimeStep});
  }
}

void TargetsManager::OnStepStart(const float& simStep)
{
  for (auto& target : targets) {
    target.OnStepStart(simStep);
  }
}

void TargetsManager::OnStepEnd()
{
  for (auto& target : targets) {
    target.OnStepEnd();
  }
}

std::vector<TargetController*> TargetsManager::GetTargetReferencies()
{
  std::vector<TargetController*> targetReferencies;
  targetReferencies.reserve(targets.size());

  for (auto& target : targets) {
    targetReferencies.push_back(&target);
  }

  return targetReferencies;
}
