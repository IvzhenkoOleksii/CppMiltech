#pragma once
#include <string>

#include "ArmamentFallCalculator.h"
#include "DataStructs.h"

class ArmamentController {
public:
  ArmamentController() = default;

public:
  ArmamentController(std::string ammoType, float droneAttackSpeed, float droneHeight, float hitRadius);
  void CalculateSimulationData(const float& simStep);

public:
  float GetFallDistance();
  float GetFallTime();

public:
  void OnStepStart(const float& simStep);
  void OnStepEnd();

private:
  void UpdateFallPositionPartially();
  void UpdateFallPosition();

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
  bool isFired;
  DataStructs::Point3D position;
  float fallDirection;
  float fallStepHeight;     // vertical step
  float fallStepDistance;   // horizontal step
  float numberOfFallSteps;  // how many steps will it take to fall
  float currentFallStepIndex;
};