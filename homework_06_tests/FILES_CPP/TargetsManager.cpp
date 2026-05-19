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

TargetsManager::TargetsManager(const float& arrayTimeStep) : TargetsManager()
{
    for (size_t i = 0; i < targetData.Positions.size(); ++i)
    {
        targets.push_back(TargetController{ targetData.Positions[i], arrayTimeStep });
    }
}

void TargetsManager::UpdateTargets(const float& simStep)
{
    for (size_t i = 0; i < targets.size(); ++i)
    {
        targets[i].Update(simStep);
    }
}

std::vector<TargetController*> TargetsManager::GetTargetReferencies()
{
    std::vector<TargetController*> targetReferencies;
    targetReferencies.reserve(targets.size());

    for (TargetController& target : targets)
    {
        targetReferencies.push_back(&target);
    }

    return targetReferencies;
}
