#pragma once
#include <string>

#include "Armament/ArmamentDatabase.h"
#include "Armament/Solver/IArmamentSolver.h"
#include "DataStructs.h"
#include "Threads/BaseLoop.h"

class ArmamentController : public BaseLoop {
public:
  ArmamentController(std::string ammoType,
                     std::string filePath,
                     float droneAttackSpeed,
                     float droneHeight,
                     float hitRadius,
                     std::unique_ptr<IArmamentSolver> solver);

  // messaging
public:
  using EventCallback = std::function<void(const DataStructs::Coord3D&)>;

private:
  static std::vector<EventCallback> subscribers;

  // public methods
public:
  void DropBomb(DataStructs::Coord3D startPosition, float direction);
  float GetFallDistance();
  float GetFallTime();
  bool GetIsFired();
  float GetHitRadius();
  float CalculateBombFallDistance(DataStructs::Coord3D startPoint, float speed);
  DataStructs::Coord3D GetBombPosition();

protected:
  void OnLoopStepStart() override;
  void OnLoopStepEnd() override;
  void OnAfterStepEndAction() override;

private:
  void CalculateSimulationData();
  void UpdateFallPositionPartially();
  void UpdateFallPosition();
  void UpdateFireFlag(bool newValue);

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
  std::atomic<bool> isFired;
  DataStructs::Coord3D position;
  float fallDirection;
  float fallStepHeight;     // vertical step
  float fallStepDistance;   // horizontal step
  float numberOfFallSteps;  // how many steps will it take to fall
  float currentFallStepIndex;
};