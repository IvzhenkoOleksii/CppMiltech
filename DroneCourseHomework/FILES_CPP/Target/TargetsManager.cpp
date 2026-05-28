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
  for (int i = 0; i < targetData.Positions.size(); ++i) {
    targets.push_back(TargetController{targetData.Positions[i], arrayTimeStep});
  }
}

void TargetsManager::UpdateTargets(const float& simStep)
{
  for (int i = 0; i < targets.size(); ++i) {
    targets[i].Update(simStep);
  }
}

std::vector<TargetController*> TargetsManager::GetTargetReferencies()
{
  std::vector<TargetController*> targetReferencies;
  targetReferencies.reserve(targets.size());

  for (TargetController& target : targets) {
    targetReferencies.push_back(&target);
  }

  return targetReferencies;
}
