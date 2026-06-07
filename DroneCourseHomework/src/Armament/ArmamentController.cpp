#include "Armament/ArmamentController.h"
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

ArmamentController::ArmamentController(
  std::string ammoType, float droneAttackSpeed, float droneHeight, float hitRadius, std::unique_ptr<IArmamentSolver> solver)
{
  isFired = false;
  this->hitRadius = hitRadius;
  this->solver = std::move(solver);

  armData = ArmamentDatabase::GetArmament(ammoType);

  armamentFallHeight = droneHeight;
  armamentFallTime = this->solver->CalculateFallTime(armData, droneAttackSpeed, armamentFallHeight);
  armamentFallDistance = this->solver->CalculateFallDistance(armData, droneAttackSpeed, armamentFallTime);
}

float ArmamentController::CalculateBombFallDistance(DataStructs::Coord3D startPoint, float speed)
{
  float fallTime = this->solver->CalculateFallTime(armData, speed, startPoint.Z);
  float fallDistance = this->solver->CalculateFallDistance(armData, speed, fallTime);
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
