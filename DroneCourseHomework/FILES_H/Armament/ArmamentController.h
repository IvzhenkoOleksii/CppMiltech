#pragma once
#include <string>

#include "ArmamentDatabase.h"
#include "ArmamentFallCalculator.h"

class ArmamentController {
public:
  ArmamentController() = default;

public:
  ArmamentController(std::string ammoType, float droneAttackSpeed, float droneHeight, float hitRadius);
  void CalculateSimulationData(float simStep);

public:
  float GetFallDistance();
  float GetFallTime();

private:
  ArmamentFallCalculator calculator;

private:
  float armamentFallTime;
  float armamentFallDistance;
  float armamentFallHeight;

private:
  float hitRadius;

  // data for simulation
private:
  float position;
  float fallDirection;
  float fallStepHeight;     // vertical step
  float fallStepDistance;   // horizontal step
  float numberOfFallSteps;  // how many steps will it take to fall
  float currentFallStepIndex;
};