#include "ArmamentController.h"

#include <string>

float ArmamentController::GetFallDistance()
{
	return armamentFallDistance;
}

ArmamentController::ArmamentController(std::string ammoType, float droneAttackSpeed, float droneHeight)
{
	ArmamentDatabase::Data armData = ArmamentDatabase::GetArmament(ammoType);

	armamentFallTime = armamentCalculator.CalculateFallTime(armData, droneAttackSpeed, droneHeight);
	armamentFallDistance = armamentCalculator.CalculateFallDistance(armData, droneAttackSpeed, droneHeight, armamentFallTime);
}