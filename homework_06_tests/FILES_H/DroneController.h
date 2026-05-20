#pragma once
#include "DataStructs.h"
#include "TargetController.h"
#include "ArmamentController.h"
#include "DroneCalculator.h"

#include <vector>

class DroneController {
public:
  DroneController(const DataStructs::DroneInputData& data, const float& simStep);

public:
  void LockTarget(TargetController* target, const int& targetIndex);

public:
  void LockTargets(std::vector<TargetController*> targetRefs);

public:
  void ChooseNearestTarget();

public:
  void RecalculateOperationalStates(float simStepTime);

public:
  void UpdateOperationalStates();

public:
  void CheckIfDroneReachedFirePosition();

private:
  void CalculateDistancesToTargets();

private:
  void CalculateDistanceToTarget(const DataStructs::Position2D& targetPosition, const int& targetIndex);

private:
  void CalculateAnglesBetweenDroneAndTargets();

private:
  void CalculateAngleBetweenDroneAndTarget(const DataStructs::Position2D& targetPosition, const int& targetIndex);

private:
  void CalculateFirePointsToTargets();

private:
  void CalculateRotationTimeToTargets();

private:
  void CalculateTimeToReachTargets();

private:
  void GetClosestTarget();

private:
  void RecalculateDroneDirection();

private:
  void RecalculateDroneVelocity();

private:
  void RecalculateDronePosition(float simTime);

  // variables
private:
  DataStructs::DroneInputData inputData;

private:
  ArmamentController armamentController;

private:
  DroneCalculator droneCalculator;

private:
  float minAttackDistance;

private:
  float acceleration;

private:
  float accelerationTime;

private:
  float velocityChangeStep;  // how velocity can change during single step time
private:
  float directionChangeStep;  // how direction can change during single step time

private:
  DataStructs::DronePhysicalState
    calculatedState;  // temp state, which will be at the end of simStep, -> will be copied into operationalData
private:
  DataStructs::DroneOperationalData operationalData;

  // targets store as referncies, so drone will get updated data of targets, ie -> updated position, speed, etc
private:
  std::vector<TargetController*> targets{};

private:
  std::vector<DataStructs::TargetOperationalData> targetsOperationalData{};
};