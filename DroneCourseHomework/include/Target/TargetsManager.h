#pragma once
#include "DataStructs.h"
#include "TargetController.h"

#include <cstddef>
#include <vector>

class TargetsManager {
public:
  TargetsManager();
  TargetsManager(const float& simStep, const float& arrayTimeStep);

public:
  size_t GetSize();
  float GetTargetVelocityAbs(int index);
  DataStructs::Coord2D GetTargetCurrentPosition(int index);
  DataStructs::Coord2D GetTargetPredictedPosition(int index, float time);

private:
  DataStructs::TargetData targetData;
  std::vector<std::unique_ptr<TargetController>> targets;
};