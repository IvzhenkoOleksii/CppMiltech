#pragma once
#include <string>

#include "ArmamentFallCalculator.h"

class ArmamentController {
public:
  ArmamentController() = default;

public:
  ArmamentController(std::string ammoType, float droneAttackSpeed, float droneHeight);

public:
  float GetFallDistance();

private:
  ArmamentFallCalculator armamentCalculator;

private:
  float armamentFallTime;

private:
  float armamentFallDistance;
};