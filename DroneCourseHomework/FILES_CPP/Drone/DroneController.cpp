#include <cstddef>
#include <iostream>
#include <string>
#include <cmath>

#include "Drone/DroneController.h"
#include "Drone/DroneCalculator.h"
#include "Armament/ArmamentController.h"
#include "DataStructs.h"
#include "Target/TargetController.h"
#include "MathCalculator.h"

DroneController::DroneController(const DataStructs::InputData& input)
{
  inputData = input.DroneData;
  simStep = input.SimTestStep;

  math = {};
  droneCalculator = {};
  armamentController = {inputData.AmmoType, inputData.AttackSpeed, inputData.Position.Z, input.HitRadius};
  armamentController.CalculateSimulationData(input.SimTestStep);

  InitState();
  CalculateAcceleration();

  // this also can be counted once
  rotateChangeStep = inputData.AngularSpeed / simStep;
  minAttackDistance = armamentController.GetFallDistance() + inputData.AccelerationPath;
}

void DroneController::InitState()
{
  droneState = {};
  droneState.State = DataStructs::STOPPED;
  droneState.transform.Position = inputData.Position;
  droneState.transform.Direction = inputData.InitialDirection;
  droneState.transform.Velocity = 0;
  droneState.DeselectTarget();
}

void DroneController::CalculateAcceleration()
{
  float acceleration = droneCalculator.CalculateDroneAcceleration(inputData.AttackSpeed, inputData.AccelerationPath);
  accelerationTime = inputData.AttackSpeed / acceleration;
  velocityChangeStep = inputData.AttackSpeed / (accelerationTime / simStep);
}

void DroneController::LockTargets(std::vector<TargetController*> targetRefs)
{
  targets = targetRefs;
}

void DroneController::OnStepStart(const float& simStep)
{
  ChooseTarget();
  GetTargetSolution();
  UpdateDroneState();

  armamentController.OnStepStart(simStep);
}

void DroneController::OnStepEnd()
{
  armamentController.OnStepEnd();
}

void DroneController::ChooseTarget()
{
  if (droneState.IsTargetSelected()) {
    return;
  }

  if (armamentController.GetIsFired()) {
    return;
  }

  GetClosestTarget();
}

void DroneController::GetClosestTarget()
{
  int targetIndex = -1;
  float smallestReachTime = 0;
  for (size_t i = 0; i < targets.size(); ++i) {
    if (targets[i]->GetVelocity() > inputData.AttackSpeed) {
      // we cannot reach faster target
      continue;
    }

    float timeToTarget = CalcDroneTimeToPoint(targets[i]->GetCurrentPosition());

    if (targetIndex == -1) {
      smallestReachTime = timeToTarget;
      targetIndex = i;
      continue;
    }

    if (timeToTarget < smallestReachTime) {
      smallestReachTime = timeToTarget;
      targetIndex = i;
    }
  }

  droneState.CurrentTargetIndex = targetIndex;
}

void DroneController::GetTargetSolution()
{
  if (!droneState.IsTargetSelected()) {
    // need to select target first
    return;
  }

  if (armamentController.GetIsFired()) {
    // no more ammo
    return;
  }

  int maxSteps = 200;
  int stepIndex = 0;
  float targetTime = simStep;
  float previosTimeDifference = -1;

  while (stepIndex < maxSteps) {
    TargetController* target = targets[droneState.CurrentTargetIndex];
    DataStructs::Point2D targetPredictedPoint = target->GetPredictedPosition(targetTime);

    float droneTime = CalcDroneTimeToPoint(targetPredictedPoint);
    float timeDifference = std::fabs(targetTime - droneTime);

    if (timeDifference < simStep) {
      droneState.TargetedPosition = targetPredictedPoint;
      std::cout << "Found target solution by steps:  " << stepIndex << "new targetet point: X   " << targetPredictedPoint.X
                << " Y:  " << targetPredictedPoint.Y << std::endl;
      return;
    }

    ++stepIndex;
    bool needRollback = false;
    if (previosTimeDifference > 0 && previosTimeDifference < timeDifference) {
      targetTime -= timeDifference / 2;
      needRollback = true;
    }
    previosTimeDifference = timeDifference;

    if (needRollback) {
      continue;
    }

    if (droneTime > targetTime) {
      targetTime += timeDifference / 2;
    }
    else {
      targetTime -= timeDifference / 2;
    }
  }

  std::cout << "Didn`t found target solution" << std::endl;
}

float DroneController::CalcDroneTimeToPoint(const DataStructs::Point2D& point)
{
  DataStructs::Point2D dronePosition2D = droneState.transform.Position.GetPoint2D();
  float angle = math.AngleBetweenVectorAndPoint(dronePosition2D, droneState.transform.Direction, point);
  float rotationTime = CalculateRotationTime(angle);
  float distanceToPredictedPoint = math.DistanceBetweenPoints(dronePosition2D, point);
  float timeToFly = CalculateTimeToReach(distanceToPredictedPoint);
  return rotationTime + timeToFly + armamentController.GetFallTime();
}

float DroneController::CalculateRotationTime(const float& angleToRotate)
{
  return std::fabs(angleToRotate) / inputData.AngularSpeed;
}

float DroneController::CalculateTimeToReach(const float& distanceToTarget)
{
  float timeToMove;
  float distanceToFly = distanceToTarget - armamentController.GetFallDistance();
  if (distanceToFly < 0) {
    std::cerr << "Distance to fly is negative" << std::endl;
    return 0;
  }

  if (droneState.transform.Velocity < inputData.AttackSpeed) {
    float velocityDiff = 1.0f - droneState.transform.Velocity / inputData.AttackSpeed;
    float distanceWithAcceleration = inputData.AccelerationPath * velocityDiff;
    float timeToReachAttackSpead = accelerationTime * velocityDiff;

    distanceToFly -= distanceWithAcceleration;
    timeToMove = timeToReachAttackSpead + distanceToFly / inputData.AttackSpeed;
  }
  else {
    timeToMove = distanceToFly / inputData.AttackSpeed;
  }

  return timeToMove;
}

void DroneController::UpdateDroneState()
{
  CheckIfDroneReachedFirePosition();
  bool isRotated = UpdadeDroneRotation();
  if (!isRotated) {
    UpdateDroneVelocity();
    UpdateDronePosition();
  }
}

void DroneController::CheckIfDroneReachedFirePosition()
{
  if (armamentController.GetIsFired()) {
    // already fired
    return;
  }

  if (droneState.transform.Velocity < inputData.AttackSpeed) {
    // cannot drop bomb at low speed
    return;
  }

  if (!droneState.IsTargetSelected()) {
    // need find target first
    return;
  }

  float distanceToFly = droneState.transform.Velocity * simStep;
  DataStructs::Point2D dronePosition = droneState.transform.Position.GetPoint2D();
  bool isReadyToFire = droneCalculator.IsDistanceBetweenPointSameAsNeeded(
    dronePosition, droneState.TargetedPosition, armamentController.GetFallDistance(), distanceToFly);
  if (isReadyToFire) {
    armamentController.DropBomb(droneState.transform.Direction);
  }
}

bool DroneController::UpdadeDroneRotation()
{
  auto Rotate = [&](float angleToTarget, float rotationAngle) {
    if (angleToTarget > 0) {
      droneState.transform.Direction += rotationAngle;
    }
    else {
      droneState.transform.Direction -= rotationAngle;
    }
  };

  if (!droneState.IsTargetSelected()) {
    // no need to rotate arbitrary, only on target
    return false;
  }

  DataStructs::Point2D dronePosition = droneState.transform.Position.GetPoint2D();
  float angleToTarget = math.AngleBetweenVectorAndPoint(dronePosition, droneState.transform.Direction, droneState.TargetedPosition);

  if (math.AreEqual(angleToTarget, 0)) {
    // already rotated
    return false;
  }

  float angleToTargetAbs = std::fabs(angleToTarget);
  if (angleToTargetAbs <= inputData.TurnThreshold) {
    Rotate(angleToTarget, angleToTargetAbs);
    if (droneState.State == DataStructs::ROTATING) {
      droneState.State = DataStructs::STOPPED;
    }
    // finished rotation, turn on inputData.TurnThreshold is not count
    return false;
  }

  switch (droneState.State) {
    case DataStructs::ACCELERATING:
    case DataStructs::DECELERATING:
    case DataStructs::CRUISE:
      // we need to rotate, but we can do it only stopped
      droneState.State = DataStructs::DECELERATING;
      return false;
    case DataStructs::STOPPED:
    case DataStructs::ROTATING:
      // rotation in progress
      droneState.State = DataStructs::ROTATING;
      float angleToRotate = rotateChangeStep;
      if (angleToTargetAbs <= rotateChangeStep) {
        angleToRotate = angleToTargetAbs;
        StopDroneAndDeselectTarget();
      }

      Rotate(angleToTarget, angleToRotate);

      return true;
  }
}

void DroneController::UpdateDroneVelocity()
{
  if (!droneState.IsTargetSelected()) {
    if (droneState.transform.Velocity > 0) {
      // need to stop, to lock new target and rotate to it
      droneState.State = DataStructs::DECELERATING;
    }
    else {
      return;
    }
  }

  if (droneState.State == DataStructs::STOPPED) {
    droneState.State = DataStructs::ACCELERATING;
  }

  if (droneState.State == DataStructs::ACCELERATING) {
    if (droneState.transform.Velocity < inputData.AttackSpeed) {
      droneState.transform.Velocity += velocityChangeStep;
    }
    else {
      droneState.State = DataStructs::CRUISE;
      std::cout << "Drone reach max velocity" << std::endl;
    }

    return;
  }

  if (droneState.State == DataStructs::DECELERATING) {
    if (droneState.transform.Velocity > 0) {
      droneState.transform.Velocity -= velocityChangeStep;
    }
    else {
      StopDroneAndDeselectTarget();
    }
  }
}

void DroneController::UpdateDronePosition()
{
  if (droneState.transform.Velocity > 0) {
    float distanceToFly = droneState.transform.Velocity * simStep;

    DataStructs::Point2D directionVector = math.GetDirectionVector(droneState.transform.Direction);
    float distanceX = directionVector.X * distanceToFly;
    float distanceY = directionVector.Y * distanceToFly;

    droneState.transform.Position.X += distanceX;
    droneState.transform.Position.Y += distanceY;
  }
}

void DroneController::StopDroneAndDeselectTarget()
{
  droneState.State = DataStructs::STOPPED;
  droneState.DeselectTarget();
}