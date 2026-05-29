#pragma once
#include <vector>

#include "DataStructs.h"
#include "MathCalculator.h"

class TargetController {
public:
  TargetController();
  TargetController(const std::vector<DataStructs::Point2D> positionsData, const float& stepTimeData);

public:
  DataStructs::Point2D GetCurrentPosition();

public:
  DataStructs::Point2D GetPredictedPosition(const float& time);

public:
  void OnStepStart(const float& simStep);
  void OnStepEnd();

private:
  void CalculateVelocity();

private:
  void UpdateCurrentPathStep();

  // variables
private:
  float arrayStepTime;
  float currentStepTime;

private:
  float velocityX;
  float velocityY;
  float velocity;
  int currentPathStep;
  DataStructs::Point2D currentPosition;

private:
  MathCalculator calculator;
  std::vector<DataStructs::Point2D> positionsData;
};
