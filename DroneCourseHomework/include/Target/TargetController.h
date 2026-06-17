#pragma once
#include <cstddef>
#include <vector>

#include "DataStructs.h"
#include "MathCalculator.h"
#include "ITargetController.h"

class TargetController : public ITargetController {
public:
  TargetController(const std::vector<DataStructs::Coord2D> positionsData, const float& stepTimeData);

  // ITargetController
public:
  DataStructs::Coord2D GetCurrentPosition() override;
  DataStructs::Coord2D GetPredictedPosition(const float& time) override;
  float GetVelocity() override;
  void OnStepStart(const float& simStep) override;
  void OnStepEnd() override;

private:
  void CalculateVelocity();
  void UpdateCurrentPathStep();

  // variables
private:
  float arrayStepTime;
  float currentStepTime;

private:
  float velocityX;
  float velocityY;
  float velocity;
  size_t currentPathStep;
  DataStructs::Coord2D currentPosition;

private:
  MathCalculator calculator;
  std::vector<DataStructs::Coord2D> positionsData;
};
