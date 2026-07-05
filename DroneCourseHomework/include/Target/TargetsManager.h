#pragma once
#include "DataStructs.h"
#include "TargetController.h"

#include <mutex>
#include <vector>

class TargetsManager {
public:
  TargetsManager(const std::string& filePath);
  TargetsManager(const std::string& filePath, const float& stepTime, const float& arrayTimeStep, const int& timeScale);

public:
  size_t GetSize();
  float GetTargetVelocityAbs(const int& index);
  DataStructs::Coord2D GetTargetCurrentPosition(const int& index);
  DataStructs::Coord2D GetTargetPredictedPosition(const int& index, const float& time);
  void FinishTargetsThreads();

private:
  std::mutex mutex;
  DataStructs::TargetData targetData;
  std::vector<std::unique_ptr<TargetController>> targets;
};