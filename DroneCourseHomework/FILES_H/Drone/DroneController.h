#pragma once
#include "DataStructs.h"
#include "Target/TargetController.h"
#include "Armament/ArmamentController.h"
#include "DroneCalculator.h"
#include "MathCalculator.h"

#include <vector>

class DroneController {
public:
  DroneController(const DataStructs::InputData& data);

public:
  void LockTargets(std::vector<TargetController*> targetRefs);
  void OnStepStart(const float& simStep);
  void OnStepEnd();
  DataStructs::DroneOperationalData GetDroneState();
  bool isBombDropped();

private:
  void ChooseTarget();
  void GetTargetSolution();
  void UpdateDroneState();
  float CalculateRotationTime(const float& angleToRotate);
  float CalcDroneTimeToPoint(const DataStructs::Point2D& point);
  float CalculateTimeToReach(const float& distanceToTarget);
  bool UpdadeDroneRotation();
  void UpdateDroneVelocity();
  void UpdateDronePosition();
  void CheckIfDroneReachedFirePosition();
  void GetClosestTarget();
  void StopDroneAndDeselectTarget();

private:
  // do on the start/constructor/once
  void InitState();
  void CalculateAcceleration();

private:
  // variables
  DataStructs::DroneInputData inputData;
  float simStep;

private:
  ArmamentController armamentController;
  DroneCalculator droneCalculator;
  MathCalculator math;

private:
  float minAttackDistance;
  float accelerationTime;
  float velocityChangeStep;  // how velocity can change during single step time
  float rotateChangeStep;    // how drone can rotate during single step time

private:
  DataStructs::DroneOperationalData droneState;

  // targets store as referencies, so drone will get updated data of targets, ie -> updated position, speed, etc
private:
  std::vector<TargetController*> targets{};
};