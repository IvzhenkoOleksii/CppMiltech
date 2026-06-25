#pragma once
#include <string>
#include <memory>

#include "Armament/ArmamentDatabase.h"
#include "Armament/Solver/IArmamentSolver.h"
#include "DataStructs.h"

class ArmamentController {
public:
  ArmamentController(
    std::string ammoType, float droneAttackSpeed, float droneHeight, float hitRadius, std::unique_ptr<IArmamentSolver> solver);

  // messaging
public:
  using EventCallback = std::function<void(const DataStructs::Coord3D&)>;

private:
  static std::vector<EventCallback> subscribers;

  // public methods
public:
  void CalculateSimulationData(const float& simStep);
  void DropBomb(DataStructs::Coord3D startPosition, float direction);
  float GetFallDistance();
  float GetFallTime();
  bool GetIsFired();
  float CalculateBombFallDistance(DataStructs::Coord3D startPoint, float speed);

public:
  void OnStepStart(const float& simStep);
  void OnStepEnd();

private:
  void UpdateFallPositionPartially();
  void UpdateFallPosition();

private:
  std::unique_ptr<IArmamentSolver> solver;
  ArmamentDatabase::Data armData;

private:
  ArmamentDatabase::FallResult fallResult;
  float armamentFallHeight;

private:
  float hitRadius;

  // data for simulation
private:
  bool isFired;
  DataStructs::Coord3D position;
  float fallDirection;
  float fallStepHeight;     // vertical step
  float fallStepDistance;   // horizontal step
  float numberOfFallSteps;  // how many steps will it take to fall
  float currentFallStepIndex;
};