#include "Target/TargetsManager.h"
#include "Files/TargetFile.h"
#include "DataStructs.h"

#include <vector>

TargetsManager::TargetsManager()
{
  // read target file
  TargetFile targetFile;
  targetData = targetFile.ReadJsonFile();
}

TargetsManager::TargetsManager(const float& arrayTimeStep, MissionFactory* factory)
  : TargetsManager()
{
  for (const auto& targetPosition : targetData.Positions) {
    ITargetController* controller = factory->CreateTargetController(targetPosition, arrayTimeStep);
    targets.push_back(controller);
  }
}

void TargetsManager::OnStepStart(const float& simStep)
{
  for (auto& target : targets) {
    target->OnStepStart(simStep);
  }
}

void TargetsManager::OnStepEnd()
{
  for (auto& target : targets) {
    target->OnStepEnd();
  }
}

std::vector<ITargetController*> TargetsManager::GetTargetReferencies()
{
  return targets;
}
