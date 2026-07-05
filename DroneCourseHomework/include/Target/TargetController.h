#pragma once

#include <vector>

#include "DataStructs.h"
#include "MathCalculator.h"

class TargetController {
public:
  TargetController();
  TargetController(const std::vector<DataStructs::Coord2D> positionsData, const float& stepTimeData);

public:
  DataStructs::Coord2D GetCurrentPosition();
  DataStructs::Coord2D GetPredictedPosition(const float& time);
  float GetVelocity();

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
  size_t currentPathStep;
  DataStructs::Coord2D currentPosition;

private:
  MathCalculator calculator;
  std::vector<DataStructs::Coord2D> positionsData;
};
