#pragma once
#include <cstddef>
#include <vector>

#include "DataStructs.h"
#include "Threads/BaseLoop.h"

class TargetController : public BaseLoop {
public:
  // constructors ans destructors
  TargetController(const float& stepTime, const int& timeScale);
  TargetController(const float& stepTime,
                   const int& timeScale,
                   const float& arrayTimeStep,
                   const std::vector<DataStructs::Coord2D> positionsData);
  virtual ~TargetController() = default;

  // getters
public:
  DataStructs::Coord2D GetCurrentPosition();
  DataStructs::Coord2D GetPredictedPosition(const float& time);
  float GetVelocityAbs();

  // functions
protected:
  void OnLoopStepStart() override;
  void OnLoopStepEnd() override;
  void OnAfterStepEndAction() override;

private:
  void CalculateVelocity();
  void UpdateCurrentPathStep();
  int GetNextPathIndex();

  // variables
private:
  float arrayTimeStep;

private:
  float velocityX;
  float velocityY;
  float velocityAbs;
  size_t currentPathStep;
  DataStructs::Coord2D currentPosition;

private:
  std::vector<DataStructs::Coord2D> positionsData;
};
