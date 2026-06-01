#include "Armament/ArmamentController.h"
#include "Armament/Solver/ArmamentAnaliticalSolver.h"
#include "DataStructs.h"

#include <string>
#include <cmath>
#include <iostream>

float ArmamentController::GetFallDistance()
{
  return armamentFallDistance;
}

float ArmamentController::GetFallTime()
{
  return armamentFallTime;
}

bool ArmamentController::GetIsFired()
{
  return isFired;
}

ArmamentController::ArmamentController(std::string ammoType, float droneAttackSpeed, float droneHeight, float hitRadius)
{
  isFired = false;
  this->hitRadius = hitRadius;
  armData = ArmamentDatabase::GetArmament(ammoType);
  solver = new ArmamentAnalitycalSolver;

  armamentFallHeight = droneHeight;
  armamentFallTime = solver->CalculateFallTime(armData, droneAttackSpeed, armamentFallHeight);
  armamentFallDistance = solver->CalculateFallDistance(armData, droneAttackSpeed, armamentFallTime);
}

ArmamentController::~ArmamentController()
{
  delete solver;
}

float ArmamentController::CalculateBombFallDistance(DataStructs::Coord3D startPoint, float speed)
{
  float fallTime = solver->CalculateFallTime(armData, speed, startPoint.Z);
  float fallDistance = solver->CalculateFallDistance(armData, speed, fallTime);
  return fallDistance;
}

void ArmamentController::DropBomb(DataStructs::Coord3D startPosition, float direction)
{
  position = startPosition;
  fallDirection = direction;
  isFired = true;
}

void ArmamentController::CalculateSimulationData(const float& simStep)
{
  numberOfFallSteps = armamentFallTime / simStep;
  currentFallStepIndex = numberOfFallSteps;
  fallStepHeight = armamentFallHeight / numberOfFallSteps;
  fallStepDistance = armamentFallDistance / numberOfFallSteps;
}

void ArmamentController::OnStepStart(const float& simStep)
{
  if (!isFired) {
    return;
  }

  if (currentFallStepIndex > 1) {
    currentFallStepIndex--;
    UpdateFallPosition();
  }
  else if (currentFallStepIndex < 1 && currentFallStepIndex > 0) {
    UpdateFallPositionPartially();
    currentFallStepIndex = 0;
  }
  else {
    isFired = false;
    std::cout << "Bomb exploded. Position X:   " << position.X << "   Y:   " << position.Y << "   Z:  " << position.Z << std::endl;
  }
}

void ArmamentController::UpdateFallPosition()
{
  position.Z -= fallStepHeight;

  float xStep = fallStepDistance * cosf(fallDirection);
  float yStep = fallStepDistance * sinf(fallDirection);

  position.X += xStep;
  position.Y += yStep;
}

void ArmamentController::UpdateFallPositionPartially()
{
  position.Z -= fallStepHeight * currentFallStepIndex;

  float xStep = fallStepDistance * currentFallStepIndex * cosf(fallDirection);
  float yStep = fallStepDistance * currentFallStepIndex * sinf(fallDirection);

  position.X += xStep;
  position.Y += yStep;
}

void ArmamentController::OnStepEnd() {}
