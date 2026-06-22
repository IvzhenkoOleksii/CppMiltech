#include <cstddef>
#include <iostream>
#include <mutex>
#include <ostream>
#include <cmath>

#include "Drone/DroneController.h"
#include "Drone/DroneCalculator.h"
#include "Armament/ArmamentController.h"
#include "Armament//Solver/IArmamentSolver.h"
#include "DataStructs.h"
#include "Drone/DronePhysicalController.h"
#include "MathCalculator.h"
#include "Threads/BaseLoop.h"

DroneController::DroneController(const DataStructs::InputData& input, std::unique_ptr<IArmamentSolver> solver)
  : BaseLoop(input.SimStepTime, input.TimeScale)
  , inputData(input.DroneData)
  , droneCalculator({})
  , armamentController(std::make_unique<ArmamentController>(inputData.AmmoType,
                                                            inputData.AttackSpeed,
                                                            inputData.Position.Z,
                                                            input.HitRadius,
                                                            input.PhysicsStepTime,
                                                            input.TimeScale,
                                                            std::move(solver)))
  , physicalStateController(std::make_unique<DronePhysicalController>(input.PhysicsStepTime, input.TimeScale, input.DroneData))
{
  isFinished.store(false);
  InitState();
  InitialCalculations();

  physicalStateController->DroneStoped = [&]() { DeselectTarget(); };

  // sign on bomb exploded action
  armamentController->LoopEndedAction = [&]() {
    if (!isFinished) {
      // means simulation isn`t ended, but bomb reached ground
      if (BombExplodedAction) {
        BombExplodedAction();
      }
    }
  };

  StartLoopThread();
}

void DroneController::Finish()
{
  isFinished.store(true);
  this->FinishLoopThread();
  physicalStateController->FinishLoopThread();
  armamentController->FinishLoopThread();
}

DataStructs::DroneFullData DroneController::GetDroneState()
{
  DataStructs::DroneFullData dt;
  dt.physicalState = physicalStateController->GetState();

  std::lock_guard<std::recursive_mutex> lock(droneMutex);
  dt.operationalState = state;

  return dt;
}

bool DroneController::IsTargetSelected()
{
  std::lock_guard<std::recursive_mutex> lock(droneMutex);
  return state.IsTargetSelected();
}

int DroneController::GetTargetIndex()
{
  std::lock_guard<std::recursive_mutex> lock(droneMutex);
  return state.CurrentTargetIndex;
}

void DroneController::SetTargetIndex(int index)
{
  std::cout << "Selected target:   " << index << std::endl;
  std::lock_guard<std::recursive_mutex> lock(droneMutex);
  state.CurrentTargetIndex = index;
}

DataStructs::Coord2D DroneController::GetTargetedPosition()
{
  std::lock_guard<std::recursive_mutex> lock(droneMutex);
  return state.TargetedPosition;
}

void DroneController::SetTargetedPosition(const DataStructs::Coord2D& pos)
{
  std::lock_guard<std::recursive_mutex> lock(droneMutex);
  state.TargetedPosition = pos;
}

bool DroneController::isBombDropped()
{
  return armamentController->GetIsFired();
}

void DroneController::InitState()
{
  std::lock_guard<std::recursive_mutex> lock(droneMutex);
  state.State = DataStructs::STOPPED;
  state.DeselectTarget();
}

void DroneController::InitialCalculations()
{
  float acceleration = droneCalculator.CalculateDroneAcceleration(inputData.AttackSpeed, inputData.AccelerationPath);
  accelerationTime = inputData.AttackSpeed / acceleration;
  minAttackDistance = armamentController->GetFallDistance() + inputData.AccelerationPath;
}

void DroneController::LockTargets(TargetsManager* targetsManager)
{
  std::lock_guard<std::recursive_mutex> lock(droneMutex);
  this->targetsManager = targetsManager;
}

void DroneController::OnLoopStepStart()
{
  ChooseTarget();
  GetTargetSolution();
  CalculateThereToGo();
};

void DroneController::OnLoopStepEnd()
{
  std::lock_guard<std::recursive_mutex> lock(droneMutex);
  state.AimPoint = CalculateAimPoint();
  state.DropPoint = CalculateDropPoint();
};
//
void DroneController::OnAfterStepEndAction() {};
//
//
//
//
//

void DroneController::ChooseTarget()
{
  if (IsTargetSelected()) {
    return;
  }

  if (armamentController->GetIsFired()) {
    return;
  }

  GetClosestTarget();
}

void DroneController::GetClosestTarget()
{
  std::lock_guard<std::recursive_mutex> lock(droneMutex);
  if (targetsManager == nullptr) {
    return;
  }

  int targetIndex = -1;
  float smallestReachTime = 0;

  for (size_t i = 0; i < targetsManager->GetSize(); ++i) {
    if (targetsManager->GetTargetVelocityAbs(i) > inputData.AttackSpeed) {
      // we cannot reach faster target
      continue;
    }

    DataStructs::Coord2D currentTargetPosition = targetsManager->GetTargetCurrentPosition(i);
    float timeToTarget = CalcDroneTimeToPoint(currentTargetPosition);

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

  SetTargetIndex(targetIndex);
}

void DroneController::GetTargetSolution()
{
  if (!IsTargetSelected()) {
    // need to select target first
    return;
  }

  if (armamentController->GetIsFired()) {
    // no more ammo
    return;
  }

  int maxSteps = 200;
  int stepIndex = 0;
  float targetTime = stepTime;
  float previosTimeDifference = -1;

  int targetIndex = GetTargetIndex();
  while (stepIndex < maxSteps) {
    DataStructs::Coord2D targetPredictedPoint = targetsManager->GetTargetPredictedPosition(targetIndex, targetTime);

    float droneTime = CalcDroneTimeToPoint(targetPredictedPoint);
    float timeDifference = std::fabs(targetTime - droneTime);

    if (timeDifference < stepTime) {
      SetTargetedPosition(targetPredictedPoint);
      //  std::cout << "Found target solution by steps:  " << stepIndex << "  new targetet point: X   " << targetPredictedPoint.X
      //            << " Y:  " << targetPredictedPoint.Y << std::endl;
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

float DroneController::CalcDroneTimeToPoint(const DataStructs::Coord2D& point)
{
  DataStructs::DronePhysicalState physicalState = physicalStateController->GetState();

  DataStructs::Coord2D dronePosition2D = DataStructs::Coord3D::GetPoint2D(physicalState.Position);
  float angle = MathCalculator::AngleBetweenVectorAndPoint(dronePosition2D, physicalState.Direction, point);
  float rotationTime = CalculateRotationTime(angle);
  float distanceToPredictedPoint = MathCalculator::DistanceBetweenPoints(dronePosition2D, point);
  float timeToFly = CalculateTimeToReach(distanceToPredictedPoint);
  return rotationTime + timeToFly + armamentController->GetFallTime();
}

float DroneController::CalculateRotationTime(const float& angleToRotate)
{
  return std::fabs(angleToRotate) / inputData.AngularSpeed;
}

float DroneController::CalculateTimeToReach(const float& distanceToTarget)
{
  float timeToMove;
  float distanceToFly = distanceToTarget - armamentController->GetFallDistance();
  if (distanceToFly < 0) {
    //   std::cerr << "Distance to fly is negative" << std::endl;
    return 0;
  }

  DataStructs::DronePhysicalState physicalState = physicalStateController->GetState();
  if (physicalState.Velocity < inputData.AttackSpeed) {
    float velocityDiff = 1.0f - physicalState.Velocity / inputData.AttackSpeed;
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

void DroneController::CalculateThereToGo()
{
  CheckIfDroneReachedFirePosition();

  if (IsNeedToRotate()) {
    // can send only one command per step
    return;
  }

  IsNeedToAccelerate();
}

void DroneController::CheckIfDroneReachedFirePosition()
{
  if (armamentController->GetIsFired()) {
    // already fired
    return;
  }

  DataStructs::DronePhysicalState physicalState = physicalStateController->GetState();
  if (physicalState.Velocity < inputData.AttackSpeed) {
    // cannot drop bomb at low speed
    return;
  }

  if (!IsTargetSelected()) {
    // need find target first
    return;
  }

  float halfStepDistance = physicalStateController->GetHalfStepDistance();
  DataStructs::Coord2D dronePosition = DataStructs::Coord3D::GetPoint2D(physicalState.Position);
  DataStructs::Coord2D targetPosition = GetTargetedPosition();
  float fallDistance = armamentController->GetFallDistance();

  bool isReadyToFire = droneCalculator.IsDistanceBetweenPointSameAsNeeded(dronePosition, targetPosition, fallDistance, halfStepDistance);
  if (isReadyToFire) {
    armamentController->DropBomb(physicalState.Position, physicalState.Direction);
  }
}

bool DroneController::IsNeedToRotate()
{
  if (!IsTargetSelected()) {
    // no need to rotate arbitrary, only on target
    return false;
  }

  if (armamentController->GetIsFired()) {
    // stop rotating - no bombs left
    return false;
  }

  DataStructs::DronePhysicalState physicalState = physicalStateController->GetState();
  DataStructs::Coord2D dronePosition = DataStructs::Coord3D::GetPoint2D(physicalState.Position);
  float angleToTarget = MathCalculator::AngleBetweenVectorAndPoint(dronePosition, physicalState.Direction, GetTargetedPosition());

  if (MathCalculator::AreEqual(angleToTarget, 0)) {
    // already rotated
    return false;
  }

  if (physicalState.Velocity > 0) {
    // we can rotate only while stop, need to decelerate
    DronePhysicalCommand rotateCommand{DataStructs::DECELERATING, angleToTarget, 0};
    physicalStateController->ReceiveCommand(rotateCommand);
    return true;
  }

  DronePhysicalCommand rotateCommand{DataStructs::TURNING, angleToTarget, 0};
  physicalStateController->ReceiveCommand(rotateCommand);
  return true;
}

bool DroneController::IsNeedToAccelerate()
{
  if (!IsTargetSelected()) {
    // no need to rotate arbitrary, only on target
    return false;
  }

  if (armamentController->GetIsFired()) {
    // stop rotating - no bombs left
    return false;
  }

  DataStructs::DronePhysicalState physicalState = physicalStateController->GetState();
  if (physicalState.Velocity < inputData.AttackSpeed) {
    DronePhysicalCommand rotateCommand{DataStructs::ACCELERATING, 0, inputData.AttackSpeed};
    physicalStateController->ReceiveCommand(rotateCommand);
    return true;
  }

  return false;
}

void DroneController::DeselectTarget()
{
  std::lock_guard<std::mutex> lock(droneMutex);
  state.DeselectTarget();
}

DataStructs::Coord2D DroneController::CalculateAimPoint()
{
  // point there bomb fall if drop it right now
  DataStructs::DronePhysicalState physicalState = physicalStateController->GetState();
  float fallDistance = armamentController->CalculateBombFallDistance(physicalState.Position, physicalState.Velocity);
  return droneCalculator.CalculateAimPoint(physicalState, fallDistance);
}

DataStructs::Coord2D DroneController::CalculateDropPoint()
{
  // point there drone need to drop bomb on selected target
  float fullBombFallDistance = armamentController->GetFallDistance();
  DataStructs::Coord2D targetPosition = GetTargetedPosition();
  DataStructs::DronePhysicalState physicalState = physicalStateController->GetState();
  return droneCalculator.CalculateDropPoint(physicalState, targetPosition, fullBombFallDistance);
}
