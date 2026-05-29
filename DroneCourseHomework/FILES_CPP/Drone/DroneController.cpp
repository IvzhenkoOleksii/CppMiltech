#include <iostream>
#include <string>
#include <cmath>

#include "Drone/DroneController.h"
#include "Drone/DroneCalculator.h"
#include "DataStructs.h"
#include "Armament/ArmamentFallCalculator.h"
#include "Target/TargetController.h"

DroneController::DroneController(const DataStructs::DroneInputData& input, const float& simStep)
{
  inputData = input;
  calculatedState = {};
  droneCalculator = {};

  operationalData = {};
  operationalData.State = DataStructs::STOPPED;
  operationalData.transform.Position = inputData.Position;
  operationalData.transform.Direction = inputData.InitialDirection;
  operationalData.transform.Velocity = 0;

  calculatedState.Position = operationalData.transform.Position;
  calculatedState.Direction = operationalData.transform.Direction;
  calculatedState.Velocity = operationalData.transform.Velocity;

  // armamentController = {inputData.AmmoType, inputData.AttackSpeed, inputData.Position.Z, input.};

  // this can be counted once
  acceleration = droneCalculator.CalculateDroneAcceleration(inputData.AttackSpeed, inputData.AccelerationPath);
  accelerationTime = inputData.AttackSpeed / acceleration;
  velocityChangeStep = inputData.AttackSpeed / (accelerationTime / simStep);
  directionChangeStep = inputData.AngularSpeed / simStep;
  minAttackDistance = armamentController.GetFallDistance() + inputData.AccelerationPath;
}

void DroneController::LockTarget(TargetController* target, const int& targetIndex)
{
  targets[targetIndex] = target;
  targetsOperationalData[targetIndex] = DataStructs::TargetOperationalData();
}

void DroneController::LockTargets(std::vector<TargetController*> targetRefs)
{
  targets = targetRefs;
  for (size_t i = 0; i < targets.size(); ++i) {
    targetsOperationalData.push_back(DataStructs::TargetOperationalData());
    targetsOperationalData[i].FirePoint.DeInitialize();
    targetsOperationalData[i].ManeuverPoint.DeInitialize();
  }
}

void DroneController::ChooseNearestTarget()
{
  if (operationalData.CurrentTargetIndex == -1) {
    CalculateAnglesBetweenDroneAndTargets();
    CalculateRotationTimeToTargets();

    CalculateDistancesToTargets();
    CalculateFirePointsToTargets();

    CalculateTimeToReachTargets();
    GetClosestTarget();
  }
}

void DroneController::CalculateDistancesToTargets()
{
  for (int i = 0; i < targets.size(); ++i) {
    CalculateDistanceToTarget(targets[i]->GetCurrentPosition(), i);
  }
}

void DroneController::CalculateDistanceToTarget(const DataStructs::Point2D& targetPosition, const int& targetIndex)
{
  DataStructs::Point2D dronePosition2D = operationalData.transform.Position.GetPoint2D();
  targetsOperationalData[targetIndex].DistanceToTarget = droneCalculator.CalculateDistance(dronePosition2D, targetPosition);
}

void DroneController::CalculateAnglesBetweenDroneAndTargets()
{
  for (int i = 0; i < targets.size(); ++i) {
    CalculateAngleBetweenDroneAndTarget(targets[i]->GetCurrentPosition(), i);
  }
}

void DroneController::CalculateAngleBetweenDroneAndTarget(const DataStructs::Point2D& targetPosition, const int& targetIndex)
{
  targetsOperationalData[targetIndex].AngleToTarget = droneCalculator.CalculateAngleBetweenDroneAndTarget(
    operationalData.transform.Position, operationalData.transform.Direction, targetPosition);
  std::cout << "Angle between drone and target: " << targetsOperationalData[targetIndex].AngleToTarget << std::endl;
}

void DroneController::CalculateFirePointsToTargets()
{
  float armFallDistance = armamentController.GetFallDistance();
  DataStructs::Point2D dronePosition = operationalData.transform.Position.GetPoint2D();

  for (int i = 0; i < targets.size(); ++i) {
    DataStructs::Point2D targetPosition = targets[i]->GetCurrentPosition();
    float distanceToTarget = targetsOperationalData[i].DistanceToTarget;

    if (minAttackDistance > distanceToTarget) {
      DataStructs::Point2D maneuverPoint =
        droneCalculator.CalculateManeuverPosition(minAttackDistance, distanceToTarget, dronePosition, targetPosition);
      targetsOperationalData[i].ManeuverPoint.X = maneuverPoint.X;
      targetsOperationalData[i].ManeuverPoint.Y = maneuverPoint.Y;
      targetsOperationalData[i].ManeuverPoint.Z = 0;  // point initialized

      DataStructs::Point2D firePoint =
        droneCalculator.CalculateFirePosition(armFallDistance, minAttackDistance, maneuverPoint, targetPosition);
      targetsOperationalData[i].FirePoint.X = firePoint.X;
      targetsOperationalData[i].FirePoint.Y = firePoint.Y;
      targetsOperationalData[i].FirePoint.Z = 0;  // point initialized
    }
    else {
      DataStructs::Point2D firePoint =
        droneCalculator.CalculateFirePosition(armFallDistance, minAttackDistance, dronePosition, targetPosition);
      targetsOperationalData[i].FirePoint.X = firePoint.X;
      targetsOperationalData[i].FirePoint.Y = firePoint.Y;
      targetsOperationalData[i].FirePoint.Z = 0;  // point initialized
    }
  }
}

void DroneController::CalculateRotationTimeToTargets()
{
  for (int i = 0; i < targets.size(); ++i) {
    float angleToRotate = targetsOperationalData[i].AngleToTarget;
    float timeToRotate = abs(angleToRotate) / inputData.AngularSpeed;
    targetsOperationalData[i].TimeToRotate = timeToRotate;
  }
}

void DroneController::CalculateTimeToReachTargets()
{
  DataStructs::Point2D dronePosition2D = operationalData.transform.Position.GetPoint2D();

  for (int i = 0; i < targets.size(); ++i) {
    float distanceToTarget = targetsOperationalData[i].DistanceToTarget;
    DataStructs::Point3D maneuverPoint = targetsOperationalData[i].ManeuverPoint;
    if (maneuverPoint.IsInitialized()) {
      distanceToTarget = droneCalculator.CalculateDistance(dronePosition2D, maneuverPoint.GetPoint2D());

      DataStructs::Point2D maneuverPoint2D = maneuverPoint.GetPoint2D();
      distanceToTarget += droneCalculator.CalculateDistance(maneuverPoint2D, targetsOperationalData[i].FirePoint.GetPoint2D());
    }

    float timeToMove = distanceToTarget / inputData.AttackSpeed;
    targetsOperationalData[i].TimeToMove = timeToMove;
  }
}

void DroneController::GetClosestTarget()
{
  int targetIndex = 0;
  float smallestReachTime = 0;
  for (int i = 0; i < targets.size(); ++i) {
    if (i == 0) {
      smallestReachTime = targetsOperationalData[i].TimeToReach;
    }
    else {
      float timeToReach = targetsOperationalData[i].TimeToReach;
      if (timeToReach < smallestReachTime) {
        smallestReachTime = timeToReach;
        targetIndex = i;
      }
    }
  }

  operationalData.CurrentTargetIndex = targetIndex;
}

void DroneController::RecalculateOperationalStates(float simStepTime)
{
  RecalculateDroneDirection();
  RecalculateDroneVelocity();
  RecalculateDronePosition(simStepTime);
}

void DroneController::RecalculateDroneDirection()
{
  if (operationalData.State != DataStructs::TURNING && operationalData.State != DataStructs::STOPPED) {
    // turning allowed only when stopped
    return;
  }

  int currentTargetIndex = operationalData.CurrentTargetIndex;
  DataStructs::Point2D targetPosition = targets[currentTargetIndex]->GetCurrentPosition();

  float angleToTarget =
    droneCalculator.CalculateAngleBetweenDroneAndTarget(calculatedState.Position, calculatedState.Direction, targetPosition);

  if (!droneCalculator.AreFloatsClose(calculatedState.Direction, angleToTarget)) {
    operationalData.State = DataStructs::TURNING;
    float angleDifference = abs(angleToTarget);
    float adding = directionChangeStep;
    if (angleDifference <= directionChangeStep) {
      adding = angleToTarget;
      operationalData.State = DataStructs::STOPPED;
    }

    if (angleToTarget > 0) {
      calculatedState.Direction += adding;
    }
    else {
      calculatedState.Direction -= adding;
    }
  }
}

void DroneController::RecalculateDroneVelocity()
{
  if (operationalData.State == DataStructs::STOPPED) {
    operationalData.State = DataStructs::ACCELERATING;
  }

  if (operationalData.State == DataStructs::ACCELERATING) {
    if (calculatedState.Velocity < inputData.AttackSpeed) {
      calculatedState.Velocity += velocityChangeStep;
    }
    else {
      operationalData.State = DataStructs::MOVING;
    }

    return;
  }

  if (operationalData.State == DataStructs::DECELERATING) {
    if (calculatedState.Velocity > 0) {
      calculatedState.Velocity -= velocityChangeStep;
    }
    else {
      operationalData.State = DataStructs::STOPPED;
    }

    return;
  }
}

void DroneController::RecalculateDronePosition(float simStepTime)
{
  if (calculatedState.Velocity > 0) {
    float distanceToFly = calculatedState.Velocity * simStepTime;

    DataStructs::Point2D directionVector =
      droneCalculator.CalculateDirectionVector(calculatedState.Position.GetPoint2D(), calculatedState.Direction);
    float distancex = directionVector.X * distanceToFly;
    float distanceY = directionVector.Y * distanceToFly;

    calculatedState.Position.X += distancex;
    calculatedState.Position.Y += distanceY;
  }
}

void DroneController::UpdateOperationalStates()
{
  operationalData.transform.Position = calculatedState.Position;
  operationalData.transform.Direction = calculatedState.Direction;
  operationalData.transform.Velocity = calculatedState.Velocity;
}

void DroneController::CheckIfDroneReachedFirePosition()
{
  if (operationalData.transform.Velocity < inputData.AttackSpeed) {
    // cannot drop bomb at low speed
    return;
  }

  int targetIndex = operationalData.CurrentTargetIndex;
  DataStructs::Point2D targetPosition = targets[targetIndex]->GetCurrentPosition();
  bool isReadyToFire = droneCalculator.IsDistanceBetweenPointSameAsNeeded(
    operationalData.transform.Position.GetPoint2D(), targetPosition, armamentController.GetFallDistance());

  if (isReadyToFire) {
    std::cout << "Fire" << std::endl;
  }
}
