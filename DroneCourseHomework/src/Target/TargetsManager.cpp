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

TargetsManager::TargetsManager(const float& stepTime, const float& arrayTimeStep)
  : TargetsManager()
{
  for (const auto& targetPosition : targetData.Positions) {
    std::unique_ptr<TargetController> target = std::make_unique<TargetController>(stepTime, arrayTimeStep, targetPosition);
    targets.push_back(std::move(target));
  }

  for (const auto& target : targets) {
    target->StartLoopThread();
  }
}

void TargetsManager::FinishTargetsThreads()
{
  for (const auto& target : targets) {
    target->FinishLoopThread();
  }
}

size_t TargetsManager::GetSize()
{
  return targets.size();
}

float TargetsManager::GetTargetVelocityAbs(const int& index)
{
  return targets[index]->GetVelocityAbs();
}

DataStructs::Coord2D TargetsManager::GetTargetCurrentPosition(const int& index)
{
  return targets[index]->GetCurrentPosition();
}

DataStructs::Coord2D TargetsManager::GetTargetPredictedPosition(const int& index, const float& time)
{
  return targets[index]->GetPredictedPosition(time);
}