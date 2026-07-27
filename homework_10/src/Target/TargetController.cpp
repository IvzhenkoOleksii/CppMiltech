#include "Target/TargetController.h"
#include "MathCalculator.h"

#include <atomic>
#include <cstdlib>
#include <vector>

TargetController::TargetController(const float& stepTime, const int& timeScale)
  : BaseLoop(stepTime, timeScale)
{
}

TargetController::TargetController(const float& stepTime,
                                   const int& timeScale,
                                   const float& arrayTimeStep,
                                   const std::vector<DataStructs::Coord2D> positionsData)
  : BaseLoop(stepTime, timeScale)
{
  this->positionsData = positionsData;
  this->arrayTimeStep = arrayTimeStep;

  currentPathStep = 0;
  currentPosition = positionsData[currentPathStep];

  CalculateVelocity();
}

DataStructs::Coord2D TargetController::GetCurrentPosition()
{
  std::lock_guard<std::mutex> lock(mutex);
  return currentPosition;
}

DataStructs::Coord2D TargetController::GetPredictedPosition(const float& time)
{
  std::lock_guard<std::mutex> lock(mutex);
  DataStructs::Coord2D predictedPosition;
  predictedPosition.X = currentPosition.X + (velocityX * time);
  predictedPosition.Y = currentPosition.Y + (velocityY * time);

  return predictedPosition;
}

float TargetController::GetVelocityAbs()
{
  return velocityAbs.load();
}

void TargetController::OnLoopStepStart()
{
  bool arePositionsEqual = false;
  {
    std::lock_guard<std::mutex> lock(mutex);
    int nextPathIndex = GetNextPathIndex();
    arePositionsEqual = MathCalculator::AreEqual(currentPosition, positionsData[nextPathIndex]);
  }

  if (arePositionsEqual) {
    UpdateCurrentPathStep();
    CalculateVelocity();
  }
}

void TargetController::OnLoopStepEnd()
{
  std::lock_guard<std::mutex> lock(mutex);
  currentPosition.X += velocityX * stepTime;
  currentPosition.Y += velocityY * stepTime;
}

void TargetController::OnAfterStepEndAction() {}

void TargetController::UpdateCurrentPathStep()
{
  std::lock_guard<std::mutex> lock(mutex);
  ++currentPathStep;
  if (currentPathStep >= positionsData.size()) {
    // drop to start of array
    currentPathStep = 0;
  }
}

void TargetController::CalculateVelocity()
{
  std::lock_guard<std::mutex> lock(mutex);
  int nextPathIndex = GetNextPathIndex();

  DataStructs::Coord2D pathStartPosition = positionsData[currentPathStep];
  DataStructs::Coord2D pathEndPosition = positionsData[nextPathIndex];

  DataStructs::Coord2D vector = MathCalculator::GetDirectionVector(pathStartPosition, pathEndPosition);

  velocityX = vector.X / arrayTimeStep;
  velocityY = vector.Y / arrayTimeStep;
  velocityAbs = MathCalculator::VectorLength(velocityX, velocityY);
}

int TargetController::GetNextPathIndex()
{
  // GetNextPathIndex doesnt use mutex inside for currentPathStep
  // so, beware to use lock/guard inside upper method - which will fire GetNextPathIndex
  size_t nextPathIndex = currentPathStep + 1;
  if (nextPathIndex >= positionsData.size()) {
    // move to the start of path. Target move by circled position
    nextPathIndex = 0;
  }

  return nextPathIndex;
}
