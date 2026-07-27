#pragma once

#include "Armament/ArmamentDatabase.h"

class IArmamentSolver {
public:
  virtual ~IArmamentSolver() = default;
  virtual ArmamentDatabase::FallResult Calculate(const ArmamentDatabase::Data& armData, float droneAttackSpeed, float droneZPosition) = 0;
};