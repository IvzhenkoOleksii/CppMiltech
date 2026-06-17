#include "Target/TargetController.h"

#include <vector>

TargetController::TargetController(const std::vector<DataStructs::Coord2D> positionsData, const float& stepTimeData)
{
  currentPathStep = 0;
  currentStepTime = 0;

  calculator = {};

  this->positionsData = positionsData;
  arrayStepTime = stepTimeData;

  currentPosition = this->positionsData[currentPathStep];

  CalculateVelocity();
}

DataStructs::Coord2D TargetController::GetCurrentPosition()
{
  return currentPosition;
}

DataStructs::Coord2D TargetController::GetPredictedPosition(const float& time)
{
  DataStructs::Coord2D predictedPosition;
  predictedPosition.X = currentPosition.X + (velocityX * time);
  predictedPosition.Y = currentPosition.Y + (velocityY * time);

  return predictedPosition;
}

float TargetController::GetVelocity()
{
  return velocity;
}

void TargetController::OnStepStart(const float& simStep)
{
  currentStepTime += simStep;

  if (currentStepTime >= arrayStepTime) {
    currentStepTime -= arrayStepTime;
    UpdateCurrentPathStep();
    currentPosition = positionsData[currentPathStep];
    CalculateVelocity();
  }
  else {
    currentPosition.X += velocityX * simStep;
    currentPosition.Y += velocityY * simStep;
  }
}

void TargetController::OnStepEnd() {}

void TargetController::UpdateCurrentPathStep()
{
  ++currentPathStep;
  if (currentPathStep >= positionsData.size()) {
    // drop to start of array
    currentPathStep = 0;
  }
}

void TargetController::CalculateVelocity()
{
  size_t nextPathIndex = currentPathStep + 1;
  if (nextPathIndex >= positionsData.size()) {
    // move to the start of path. Target move by circled position
    nextPathIndex = 0;
  }

  DataStructs::Coord2D pathStartPosition = positionsData[currentPathStep];
  DataStructs::Coord2D pathEndPosition = positionsData[nextPathIndex];

  DataStructs::Coord2D vector = calculator.GetDirectionVector(pathStartPosition, pathEndPosition);

  velocityX = vector.X / arrayStepTime;
  velocityY = vector.Y / arrayStepTime;
  velocity = calculator.VectorLength(velocityX, velocityY);
}
