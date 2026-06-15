#include "Target/TargetController.h"
#include "MathCalculator.h"

#include <cstdlib>
#include <iostream>
#include <ostream>
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

  currentPathStep = 0;
  currentStepTime = 0;

  calculator = {};

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

float TargetController::GetVelocityAbs()
{
  return velocityAbs;
}

void TargetController::LoopFunction()
{
  std::chrono::duration duration = std::chrono::duration<float>{stepTime};
  while (isLoopActive) {
    if (LoopStepStartedAction) {
      LoopStepStartedAction();
    }

    std::this_thread::sleep_for(duration);

    // do something here
    OnStepEnd();

    if (LoopStepEndedAction) {
      LoopStepEndedAction();
    }
  }
}

void TargetController::OnStepEnd()
{
  std::cout << "TargetController OnStepEnd" << std::endl;
  currentStepTime += stepTime;
  currentPosition.X += velocityX * stepTime;
  currentPosition.Y += velocityY * stepTime;

  if (currentStepTime >= arrayTimeStep) {
    currentStepTime -= arrayTimeStep;
    std::cerr << "TargetController currentStepTime >= arrayTimeStep" << std::endl;
    ;
    UpdateCurrentPathStep();
    CalculateVelocity();
  }
}

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

  velocityX = vector.X / arrayTimeStep;
  velocityY = vector.Y / arrayTimeStep;
  velocityAbs = calculator.VectorLength(velocityX, velocityY);
}
