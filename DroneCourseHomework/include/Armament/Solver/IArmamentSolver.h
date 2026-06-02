#pragma once

#include "Armament/ArmamentDatabase.h"

class IArmamentSolver {
public:
  virtual ~IArmamentSolver() = default;
  virtual float CalculateFallTime(const ArmamentDatabase::Data& armData, float droneAttackSpeed, float droneZPosition) = 0;
  virtual float CalculateFallDistance(const ArmamentDatabase::Data&, float droneAttackSpeed, float fallTime) = 0;
};