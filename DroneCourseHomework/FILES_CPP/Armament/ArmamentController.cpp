#include "Armament/ArmamentController.h"

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
  ArmamentDatabase::Data armData = ArmamentDatabase::GetArmament(ammoType);

  armamentFallHeight = droneHeight;
  armamentFallTime = calculator.CalculateFallTime(armData, droneAttackSpeed, armamentFallHeight);
  armamentFallDistance = calculator.CalculateFallDistance(armData, droneAttackSpeed, armamentFallTime);
}

void ArmamentController::DropBomb(float direction)
{
  fallDirection = direction;
  isFired = true;
  std::cout << "Fire" << std::endl;
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

    std::cout << "Bomb exploded. Position X:   " << position.X << "   Y:  " << position.Y << std::endl;

    //     BombExploded?.Invoke(new Vector3(position.x, position.y, position.z));
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