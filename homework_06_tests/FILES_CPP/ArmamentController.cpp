#include "ArmamentController.h"

#include <string>
#include <utility>

auto ArmamentController::GetFallDistance() -> float
{
  return armamentFallDistance;
}

ArmamentController::ArmamentController(std::string ammoType, float droneAttackSpeed, float droneHeight)
{
  ArmamentDatabase::Data armData = ArmamentDatabase::GetArmament(std::move(ammoType));

  armamentFallTime = armamentCalculator.CalculateFallTime(armData, droneAttackSpeed, droneHeight);
  armamentFallDistance = armamentCalculator.CalculateFallDistance(armData, droneAttackSpeed, armamentFallTime);
}
