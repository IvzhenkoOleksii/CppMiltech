#include "Target/TargetController.h"
#include "MathCalculator.h"

#include <cstdlib>
#include <vector>

TargetController::TargetController(const float& stepTime)
  : BaseLoop(stepTime)
{
}

TargetController::TargetController(const float& stepTime, const float& arrayTimeStep, const std::vector<DataStructs::Coord2D> positionsData)
  : BaseLoop(stepTime)
{
  this->positionsData = positionsData;
  this->arrayTimeStep = arrayTimeStep;

  this->currentPathStep = 0;
  this->currentPosition = this->positionsData[currentPathStep];

  CalculateVelocity();
}

DataStructs::Coord2D TargetController::GetCurrentPosition()
{
  return this->currentPosition;
}

DataStructs::Coord2D TargetController::GetPredictedPosition(const float& time)
{
  DataStructs::Coord2D predictedPosition;
  predictedPosition.X = this->currentPosition.X + (this->velocityX * time);
  predictedPosition.Y = this->currentPosition.Y + (this->velocityY * time);

  return predictedPosition;
}

float TargetController::GetVelocityAbs()
{
  return this->velocityAbs;
}

void TargetController::LoopFunction()
{
  std::chrono::duration duration = std::chrono::duration<float>{stepTime};
  while (isLoopActive) {
    if (LoopStepStartedAction) {
      LoopStepStartedAction();
    }

    std::this_thread::sleep_for(duration);

    // we are updating target position here
    OnStepEnd();

    if (LoopStepEndedAction) {
      LoopStepEndedAction();
    }
  }
}

void TargetController::OnStepEnd()
{
  this->currentPosition.X += velocityX * stepTime;
  this->currentPosition.Y += velocityY * stepTime;

  int nextPathIndex = GetNextPathIndex();
  if (MathCalculator::AreEqual(this->currentPosition, this->positionsData[nextPathIndex])) {
    UpdateCurrentPathStep();
    CalculateVelocity();
  }
}

void TargetController::UpdateCurrentPathStep()
{
  ++this->currentPathStep;
  if (this->currentPathStep >= this->positionsData.size()) {
    // drop to start of array
    this->currentPathStep = 0;
  }
}

void TargetController::CalculateVelocity()
{
  int nextPathIndex = GetNextPathIndex();

  DataStructs::Coord2D pathStartPosition = this->positionsData[currentPathStep];
  DataStructs::Coord2D pathEndPosition = this->positionsData[nextPathIndex];

  DataStructs::Coord2D vector = MathCalculator::GetDirectionVector(pathStartPosition, pathEndPosition);

  this->velocityX = vector.X / this->arrayTimeStep;
  this->velocityY = vector.Y / this->arrayTimeStep;
  this->velocityAbs = MathCalculator::VectorLength(this->velocityX, this->velocityY);
}

int TargetController::GetNextPathIndex()
{
  size_t nextPathIndex = this->currentPathStep + 1;
  if (nextPathIndex >= this->positionsData.size()) {
    // move to the start of path. Target move by circled position
    nextPathIndex = 0;
  }

  return nextPathIndex;
}
