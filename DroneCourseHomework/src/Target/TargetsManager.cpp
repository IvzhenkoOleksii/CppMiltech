#include "Target/TargetsManager.h"
#include "Target/TargetController.h"
#include "Files/TargetFile.h"
#include "DataStructs.h"

#include <memory>
#include <mutex>
#include <vector>

TargetsManager::TargetsManager()
{
  // read target file
  TargetFile targetFile;
  targetData = targetFile.ReadJsonFile();
}

TargetsManager::TargetsManager(const float& stepTime, const float& arrayTimeStep, const int& timeScale)
  : TargetsManager()
{
  std::lock_guard<std::mutex> lock(mutex);
  for (const auto& targetPosition : targetData.Positions) {
    std::unique_ptr<TargetController> target = std::make_unique<TargetController>(stepTime, timeScale, arrayTimeStep, targetPosition);
    targets.push_back(std::move(target));
  }

  for (const auto& target : targets) {
    target->StartLoopThread();
  }
}

void TargetsManager::FinishTargetsThreads()
{
  std::lock_guard<std::mutex> lock(mutex);
  for (const auto& target : targets) {
    target->FinishLoopThread();
  }

  for (const auto& target : targets) {
    target->JoinThread();
  }
}

size_t TargetsManager::GetSize()
{
  std::lock_guard<std::mutex> lock(mutex);
  return targets.size();
}

float TargetsManager::GetTargetVelocityAbs(const int& index)
{
  std::lock_guard<std::mutex> lock(mutex);
  return targets[index]->GetVelocityAbs();
}

DataStructs::Coord2D TargetsManager::GetTargetCurrentPosition(const int& index)
{
  std::lock_guard<std::mutex> lock(mutex);
  return targets[index]->GetCurrentPosition();
}

DataStructs::Coord2D TargetsManager::GetTargetPredictedPosition(const int& index, const float& time)
{
  std::lock_guard<std::mutex> lock(mutex);
  return targets[index]->GetPredictedPosition(time);
}