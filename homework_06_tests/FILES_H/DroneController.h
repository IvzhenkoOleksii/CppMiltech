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
  void LockTargets(std::vector<TargetController*> targetRefs);
  void ChooseNearestTarget();

public:
  void RecalculateOperationalStates(float simStepTime);
  void UpdateOperationalStates();
  void CheckIfDroneReachedFirePosition();

public:
  float GetFallDistance();
  float GetDistanceToTarget(DataStructs::Position2D targetPosition);
  DataStructs::Position2D GetCurrentPosition();
  DataStructs::Position2D GetManeuverPosition(float minAttackDistance, float droneToTargetDistance, DataStructs::Position2D targetPosition);

  DataStructs::Position2D GetFirePosition(float fallDistance,
                                          float droneToTargetDistance,
                                          DataStructs::Position2D startPosition,
                                          DataStructs::Position2D targetPosition);

private:
  void CalculateDistancesToTargets();
  void CalculateDistanceToTarget(const DataStructs::Position2D& targetPosition, const int& targetIndex);
  void CalculateAnglesBetweenDroneAndTargets();
  void CalculateAngleBetweenDroneAndTarget(const DataStructs::Position2D& targetPosition, const int& targetIndex);
  void CalculateFirePointsToTargets();
  void CalculateRotationTimeToTargets();
  void CalculateTimeToReachTargets();
  void GetClosestTarget();
  void RecalculateDroneDirection();
  void RecalculateDroneVelocity();
  void RecalculateDronePosition(float simTime);

  // variables
private:
  DataStructs::DroneInputData inputData;
  ArmamentController armamentController;
  DroneCalculator droneCalculator;

private:
  float minAttackDistance;
  float acceleration;
  float accelerationTime;
  float velocityChangeStep;   // how velocity can change during single step time
  float directionChangeStep;  // how direction can change during single step time

private:
  DataStructs::DronePhysicalState
    calculatedState;  // temp state, which will be at the end of simStep, -> will be copied into operationalData
private:
  DataStructs::DroneOperationalData operationalData;

  // targets store as referncies, so drone will get updated data of targets, ie -> updated position, speed, etc
private:
  std::vector<TargetController*> targets{};
  std::vector<DataStructs::TargetOperationalData> targetsOperationalData{};
};