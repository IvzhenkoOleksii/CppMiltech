#pragma once

#include "Armament/Solver/IArmamentSolver.h"
#include "Armament/ArmamentDatabase.h"

class ArmamentAnalitycalSolver : public IArmamentSolver {
public:
  ~ArmamentAnalitycalSolver();
  float CalculateFallTime(const ArmamentDatabase::Data& armData, float droneAttackSpeed, float droneZPosition) override;
  float CalculateFallDistance(const ArmamentDatabase::Data&, float droneAttackSpeed, float fallTime) override;

private:
  // time of flight = a · t*t*t + b · t*t + c = 0
  float CalculateCoefficientA(const ArmamentDatabase::Data&, float droneAttackSpeed);
  float CalculateCoefficientB(const ArmamentDatabase::Data&, float droneAttackSpeed);
  float CalculateCoefficientC(const ArmamentDatabase::Data&, float droneZPosition);

  // Tartaglia/Cardano method
private:
  float CalculateCardanoP(float a, float b);
  float CalculateCardanoQ(float a, float b, float c);
  float CalculateCardanoPHI(float p, float q);

private:
  float CalculateFallingTime(float p, float fi, float a, float b);
};