#pragma once
#include "DataStructs.h"
#include "Armament/ArmamentController.h"
#include "Armament//Solver/IArmamentSolver.h"
#include "Drone/DronePhysicalController.h"
#include "DroneCalculator.h"
#include "Target/TargetsManager.h"
#include "Threads/BaseLoop.h"

#include <atomic>
#include <memory>

class DroneController : public BaseLoop {
public:
  DroneController(const DataStructs::InputData& data, std::unique_ptr<IArmamentSolver> solver);

public:
  void LockTargets(TargetsManager* targetsManager);
  DataStructs::DroneFullData GetDroneState();
  bool isBombDropped();
  void Start();
  void Finish();
  std::function<void()> BombExplodedAction;

protected:
  void OnLoopStepStart() override;
  void OnLoopStepEnd() override;
  void OnAfterStepEndAction() override;

private:
  bool IsTargetSelected();
  int GetTargetIndex();
  void SetTargetIndex(int index);
  DataStructs::Coord2D GetTargetedPosition();
  void SetTargetedPosition(const DataStructs::Coord2D& pos);

  void ChooseTarget();
  void GetTargetSolution();
  void CalculateThereToGo();
  float CalculateRotationTime(const float& angleToRotate);
  float CalcDroneTimeToPoint(const DataStructs::Coord2D& point);
  float CalculateTimeToReach(const float& distanceToTarget);
  bool IsNeedToRotate();
  bool IsNeedToAccelerate();

  void CheckIfDroneReachedFirePosition();
  void GetClosestTarget();
  void DeselectTarget();
  DataStructs::Coord2D CalculateAimPoint();
  DataStructs::Coord2D CalculateDropPoint();

private:
  // do on the start/constructor/once
  void InitState();
  void InitialCalculations();

  // variables
private:
  DataStructs::DroneInputData inputData;
  DroneCalculator droneCalculator;
  TargetsManager* targetsManager = nullptr;
  std::unique_ptr<ArmamentController> armamentController;
  std::unique_ptr<DronePhysicalController> physicalStateController;
  DataStructs::DroneOperationalState state;
  std::atomic<bool> isFinished;
  std::recursive_mutex droneMutex;

private:
  float minAttackDistance;
  float accelerationTime;
};