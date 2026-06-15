#include "Target/TargetsManager.h"
#include "Target/TargetController.h"
#include "Files/TargetFile.h"
#include "DataStructs.h"

#include <memory>
#include <vector>

TargetsManager::TargetsManager()
{
  // read target file
  TargetFile targetFile;
  targetData = targetFile.ReadJsonFile();
}

TargetsManager::TargetsManager(const float& simStep, const float& arrayTimeStep)
  : TargetsManager()
{
  for (const auto& targetPosition : targetData.Positions) {
    std::unique_ptr<TargetController> target = std::make_unique<TargetController>(simStep, arrayTimeStep, targetPosition);
    targets.push_back(std::move(target));
  }
}

size_t TargetsManager::GetSize()
{
  return targets.size();
}

float TargetsManager::GetTargetVelocityAbs(int index)
{
  return targets[index]->GetVelocityAbs();
}

DataStructs::Coord2D TargetsManager::GetTargetCurrentPosition(int index)
{
  return targets[index]->GetCurrentPosition();
}

DataStructs::Coord2D TargetsManager::GetTargetPredictedPosition(int index, float time)
{
  return targets[index]->GetPredictedPosition(time);
}