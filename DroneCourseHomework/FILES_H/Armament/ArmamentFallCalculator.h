#pragma once
#include "DataStructs.h"
#include "ArmamentDatabase.h"

class ArmamentFallCalculator {
public:
  float CalculateFallTime(const ArmamentDatabase::Data& armData, float droneAttackSpeed, float droneZPosition);

public:
  float CalculateFallDistance(const ArmamentDatabase::Data&, float droneAttackSpeed, float droneZPosition, float fallTime);

  // time of flight = a · t*t*t + b · t*t + c = 0
private:
  float CalculateCoefficientA(const ArmamentDatabase::Data&, float droneAttackSpeed);

private:
  float CalculateCoefficientB(const ArmamentDatabase::Data&, float droneAttackSpeed);

private:
  float CalculateCoefficientC(const ArmamentDatabase::Data&, float droneZPosition);

  // Tartaglia/Cardano method
private:
  float CalculateCardanoP(float a, float b);

private:
  float CalculateCardanoQ(float a, float b, float c);

private:
  float CalculateCardanoPHI(float p, float q);

private:
  float CalculateFallingTime(float p, float fi, float a, float b);
};