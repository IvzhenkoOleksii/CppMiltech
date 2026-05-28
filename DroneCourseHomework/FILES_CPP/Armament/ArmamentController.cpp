#include "Armament/ArmamentController.h"

#include <string>

float ArmamentController::GetFallDistance()
{
  return armamentFallDistance;
}

float ArmamentController::GetFallTime()
{
  return armamentFallTime;
}

ArmamentController::ArmamentController(std::string ammoType, float droneAttackSpeed, float droneHeight, float hitRadius)
{
  this->hitRadius = hitRadius;
  ArmamentDatabase::Data armData = ArmamentDatabase::GetArmament(ammoType);

  armamentFallHeight = droneHeight;
  armamentFallTime = calculator.CalculateFallTime(armData, droneAttackSpeed, armamentFallHeight);
  armamentFallDistance = calculator.CalculateFallDistance(armData, droneAttackSpeed, armamentFallHeight, armamentFallTime);
}

void ArmamentController::CalculateSimulationData(float simStep)
{
  numberOfFallSteps = armamentFallTime / simStep;
  currentFallStepIndex = numberOfFallSteps;
  fallStepHeight = armamentFallHeight / numberOfFallSteps;
  fallStepDistance = armamentFallDistance / numberOfFallSteps;
}