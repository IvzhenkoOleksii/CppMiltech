#include "TargetsManager.h"
#include "TargetController.h"
#include "TargetFile.h"
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
  for (const auto& Position : targetData.Positions) {
    targets.emplace_back(Position, arrayTimeStep);
  }
}

void TargetsManager::UpdateTargets(const float& simStep)
{
  for (auto& target : targets) {
    target.Update(simStep);
  }
}

auto TargetsManager::GetTargetReferencies() -> std::vector<TargetController*>
{
  std::vector<TargetController*> targetReferencies;
  targetReferencies.reserve(targets.size());

  for (TargetController& target : targets) {
    targetReferencies.push_back(&target);
  }

  return targetReferencies;
}
