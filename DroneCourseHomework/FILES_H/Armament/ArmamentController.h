#pragma once
#include <string>

#include "ArmamentFallCalculator.h"
#include "DataStructs.h"

class ArmamentController {
public:
  ArmamentController() = default;
  ArmamentController(std::string ammoType, float droneAttackSpeed, float droneHeight, float hitRadius);

public:
  void CalculateSimulationData(const float& simStep);
  void DropBomb(DataStructs::Point3D startPosition, float direction);
  float GetFallDistance();
  float GetFallTime();
  bool GetIsFired();

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