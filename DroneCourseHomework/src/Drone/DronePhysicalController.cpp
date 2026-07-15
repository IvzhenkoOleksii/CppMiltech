
#include <chrono>
#include <cmath>
#include <iostream>
#include <mutex>
#include <optional>
#include <ostream>

#include "DataStructs.h"
#include "MathCalculator.h"
#include "Drone/DronePhysicalController.h"

DronePhysicalController::DronePhysicalController(const float& stepTime, const int& timeScale, const DataStructs::DroneInputData& inputData)
  : BaseLoop(stepTime, timeScale)
{
  droneCalculator = {};

  state.Velocity = 0;
  state.TempVelocity = 0;
  state.Position = inputData.Position;
  state.Direction = inputData.InitialDirection;
  state.DroneStateType = DataStructs::STOPPED;

  startClock = std::chrono::steady_clock::now();
  ticksCounter = 0;
  state.TimeSecSinceStart = 0;

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

  auto currentTime = std::chrono::steady_clock::now();
  auto passedTime = currentTime - startClock;
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(passedTime).count();
  double millisecondsD = (double)milliseconds;
  double seconds = millisecondsD / 1000;

  state.TimeSecSinceStart = seconds;
  state.tickCounter = ticksCounter;
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

  //  std::cout << "Accelerate function. Velocity at start:   " << state.Velocity << std::endl;

  if (state.TempVelocity < currentCommand->SpeedToReach) {
    state.TempVelocity += speedChangeStep;
    state.DroneStateType = DataStructs::ACCELERATING;

    //   std::cout << "Accelerate function. Velocity became:   " << state.Velocity << "  speedChangeStep:  " << speedChangeStep <<
    //   std::endl;
  }

  if (state.TempVelocity > currentCommand->SpeedToReach) {
    state.TempVelocity = currentCommand->SpeedToReach;
  }

  if (state.TempVelocity == currentCommand->SpeedToReach) {
    state.DroneStateType = DataStructs::MOVING;
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
    state.DroneStateType = DataStructs::DECELERATING;
  }

  if (state.Velocity < currentCommand->SpeedToReach) {
    state.Velocity = currentCommand->SpeedToReach;
  }

  if (state.Velocity == currentCommand->SpeedToReach) {
    isNeedToResetCommand.store(true);
  }

  if (state.Velocity == 0) {
    state.DroneStateType = DataStructs::STOPPED;
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

  state.DroneStateType = DataStructs::TURNING;
}

void DronePhysicalController::UpdatePosition()
{
  std::lock_guard<std::mutex> lock(physicalMutex);

  ticksCounter++;
  state.Velocity = state.TempVelocity;

  if (state.Velocity <= 0) {
    // dron stay in place
    return;
  }

  float distanceToFly = state.Velocity * stepTime;

  // std::cout << "UpdatePosition function. Velocity:   " << state.Velocity << "  stepTime:  " << stepTime
  //           << "  distanceChange:  " << distanceToFly << std::endl;

  DataStructs::Coord2D directionVector = MathCalculator::GetDirectionVector(state.Direction);
  float distanceX = directionVector.X * distanceToFly;
  float distanceY = directionVector.Y * distanceToFly;

  // std::cout << "UpdatePosition function. distanceX:   " << distanceX << "  distanceY:  " << distanceY << std::endl;
  // std::cout << "UpdatePosition function. state.Position.X before:   " << state.Position.X
  //           << "  state.Position.Y before:  " << state.Position.Y << std::endl;

  state.Position.X += distanceX;
  state.Position.Y += distanceY;

  // std::cout << "UpdatePosition function. state.Position.X after:   " << state.Position.X
  //           << "  state.Position.Y after:  " << state.Position.Y << std::endl
  //           << "//////////" << std::endl;
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
