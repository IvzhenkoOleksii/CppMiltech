
#include <cmath>
#include <mutex>
#include <optional>

#include "DataStructs.h"
#include "MathCalculator.h"
#include "Drone/DronePhysicalController.h"

DronePhysicalController::DronePhysicalController(const float& stepTime, const int& timeScale, const DataStructs::DroneInputData& inputData)
  : BaseLoop(stepTime, timeScale)
{
  droneCalculator = {};

  state.Velocity = 0;
  state.Position = inputData.Position;
  state.Direction = inputData.InitialDirection;

  maxSpeed = inputData.AttackSpeed;
  angularSpeed = inputData.AngularSpeed;
  turnThreshold = inputData.TurnThreshold;

  rotateChangeStep = inputData.AngularSpeed * stepTime;

  currentCommand = std::nullopt;
  CalculateAcceleration(inputData.AccelerationPath);
}

void DronePhysicalController::CalculateAcceleration(const float& accelerationPath)
{
  float acceleration = droneCalculator.CalculateDroneAcceleration(maxSpeed, accelerationPath);
  accelerationTime = maxSpeed / acceleration;
  speedChangeStep = maxSpeed / (accelerationTime / stepTime);
}

DataStructs::DronePhysicalState DronePhysicalController::GetState()
{
  std::lock_guard<std::mutex> lock(physicalMutex);
  return state;
}

DataStructs::Coord3D DronePhysicalController::GetPosition()
{
  std::lock_guard<std::mutex> lock(physicalMutex);
  return state.Position;
}

void DronePhysicalController::ReceiveCommand(const DronePhysicalCommand& command)
{
  std::lock_guard<std::mutex> lock(physicalMutex);
  currentCommand = command;
}

void DronePhysicalController::Accelerate()
{
  // do not add mutex guard here -> have already at OnLoopStepStart
  // if lock mutex here -> we will get self deadlock

  Rotate();

  if (state.Velocity < currentCommand->SpeedToReach) {
    state.Velocity += speedChangeStep;
  }

  if (state.Velocity > currentCommand->SpeedToReach) {
    state.Velocity = currentCommand->SpeedToReach;
  }

  if (state.Velocity == currentCommand->SpeedToReach) {
    isNeedToResetCommand.store(true);
  }

  //  std::cout << "Accelerate. state.Velocity: " << state.Velocity << std::endl;
}

void DronePhysicalController::Decelerate()
{
  // do not add mutex guard here -> have already at OnLoopStepStart
  // if lock mutex here -> we will get self deadlock

  if (state.Velocity > currentCommand->SpeedToReach) {
    state.Velocity -= speedChangeStep;
  }

  if (state.Velocity < currentCommand->SpeedToReach) {
    state.Velocity = currentCommand->SpeedToReach;
  }

  if (state.Velocity == currentCommand->SpeedToReach) {
    isNeedToResetCommand.store(true);
  }

  if (state.Velocity == 0) {
    shouldNotifyStopped.store(true);
  }

  //  std::cout << "Decelerate. state.Velocity: " << state.Velocity << std::endl;
}

void DronePhysicalController::Rotate()
{
  // do not add mutex guard here -> have already at OnLoopStepStart
  // if lock mutex here -> we will get self deadlock

  float angleToRotateAbs = std::fabs(currentCommand->AngleToRotate);
  if (angleToRotateAbs < rotateChangeStep) {
    RotateOnSmallAngle(angleToRotateAbs);
    return;
  }

  // if angleToRotateAbs > rotateChangeStep
  RotateOnSignificantAngle();
}

void DronePhysicalController::RotateOnSmallAngle(float angleAbs)
{
  // do not add mutex guard here -> have already at OnLoopStepStart
  // if lock mutex here -> we will get self deadlock

  if (currentCommand->AngleToRotate > 0) {
    state.Direction += angleAbs;
  }
  else {
    state.Direction -= angleAbs;
  }

  currentCommand->AngleToRotate = 0;
  if (currentCommand->commandType == DataStructs::TURNING) {
    isNeedToResetCommand.store(true);
  }
}
void DronePhysicalController::RotateOnSignificantAngle()
{
  // do not add mutex guard here -> have already at OnLoopStepStart
  // if lock mutex here -> we will get self deadlock

  if (currentCommand->AngleToRotate > 0) {
    state.Direction += rotateChangeStep;
    currentCommand->AngleToRotate -= rotateChangeStep;
    // std::cout << "Rotate. AngleToRotate: " << currentCommand->AngleToRotate << " ++++ " << std::endl;
  }
  else {
    state.Direction -= rotateChangeStep;
    currentCommand->AngleToRotate += rotateChangeStep;
    // std::cout << "Rotate. AngleToRotate: " << currentCommand->AngleToRotate << " ---- " << std::endl;
  }
}

void DronePhysicalController::UpdatePosition()
{
  std::lock_guard<std::mutex> lock(physicalMutex);
  if (state.Velocity <= 0) {
    // dron stay in place
    return;
  }

  float distanceToFly = state.Velocity * stepTime;

  DataStructs::Coord2D directionVector = MathCalculator::GetDirectionVector(state.Direction);
  float distanceX = directionVector.X * distanceToFly;
  float distanceY = directionVector.Y * distanceToFly;

  state.Position.X += distanceX;
  state.Position.Y += distanceY;

  //  std::cout << "UpdatePosition. state.Position.X: " << state.Position.X << "   Y:   " << state.Position.Y << std::endl;
}

void DronePhysicalController::ProcessCommand()
{
  std::lock_guard<std::mutex> lock(physicalMutex);
  if (!currentCommand.has_value()) {
    // no commands, don`t do anything
    return;
  }

  switch (currentCommand->commandType) {
    case DataStructs::ACCELERATING:
      Accelerate();
      break;
    case DataStructs::DECELERATING:
      Decelerate();
      break;
    case DataStructs::TURNING:
      Rotate();
    default:
      break;
  }

  if (isNeedToResetCommand.exchange(false)) {
    currentCommand = std::nullopt;
  }
}

void DronePhysicalController::CheckDroneStopped()
{
  if (shouldNotifyStopped.exchange(false)) {
    if (DroneStopedAction) {
      DroneStopedAction();
    }
  }
}

void DronePhysicalController::OnLoopStepStart()
{
  ProcessCommand();
  CheckDroneStopped();
}

void DronePhysicalController::OnLoopStepEnd()
{
  UpdatePosition();
}

void DronePhysicalController::OnAfterStepEndAction() {}
