#pragma once
#include "DataStructs.h"
#include "Armament/ArmamentController.h"
#include "Armament//Solver/IArmamentSolver.h"
#include "DroneCalculator.h"
#include "Target/TargetsManager.h"

#include <memory>

class DroneController {
public:
  DroneController(const DataStructs::InputData& data, std::unique_ptr<IArmamentSolver> solver);

public:
  void LockTargets(TargetsManager* targetsManager);
  void OnStepStart(const float& simStep);
  void OnStepEnd();
  DataStructs::DroneOperationalData GetDroneState();
  bool isBombDropped();

private:
  void ChooseTarget();
  void GetTargetSolution();
  void UpdateDroneState();
  float CalculateRotationTime(const float& angleToRotate);
  float CalcDroneTimeToPoint(const DataStructs::Coord2D& point);
  float CalculateTimeToReach(const float& distanceToTarget);
  bool UpdadeDroneRotation();
  void UpdateDroneVelocity();
  void UpdateDronePosition();
  void CheckIfDroneReachedFirePosition();
  void GetClosestTarget();
  void StopDroneAndDeselectTarget();
  DataStructs::Coord2D WhereBombDrop();
  DataStructs::Coord2D WhereDroneHeading();

private:
  // do on the start/constructor/once
  void InitState();
  void CalculateAcceleration();

private:
  // variables
  DataStructs::DroneInputData inputData;
  float simStep;

private:
  DroneCalculator droneCalculator;
  ArmamentController armamentController;
  TargetsManager* targetsManager;

private:
  float minAttackDistance;
  float accelerationTime;
  float velocityChangeStep;  // how velocity can change during single step time
  float rotateChangeStep;    // how drone can rotate during single step time

private:
  DataStructs::DroneOperationalData droneState;
};