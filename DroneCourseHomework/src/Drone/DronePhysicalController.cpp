#include "Drone/DronePhysicalController.h"
#include <mutex>
#include <optional>
#include "DataStructs.h"
#include "MathCalculator.h"

// let in be almost private
std::optional<DronePhysicalCommand> currentCommand;
std::mutex mutex;

void ResetCommandToNull()
{
  std::lock_guard<std::mutex> lock(mutex);
  currentCommand = std::nullopt;
}

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

  ResetCommandToNull();
  CalculateAcceleration(inputData.AccelerationPath);

  StartLoopThread();
}

void DronePhysicalController::CalculateAcceleration(const float& accelerationPath)
{
  float acceleration = droneCalculator.CalculateDroneAcceleration(maxSpeed, accelerationPath);
  accelerationTime = maxSpeed / acceleration;
  speedChangeStep = maxSpeed / (accelerationTime / stepTime);
}

DataStructs::DronePhysicalState DronePhysicalController::GetState()
{
  std::lock_guard<std::mutex> lock(mutex);
  return state;
}

float DronePhysicalController::GetHalfStepDistance()
{
  // half of a distance, that drone flyes on max speed - need for point of bomb drop calculation
  // half of distance max accuracy of bomb drops
  std::lock_guard<std::mutex> lock(mutex);
  float distanceToFly = state.Velocity * stepTime;
  return distanceToFly / 2;
}

void DronePhysicalController::ReceiveCommand(const DronePhysicalCommand& command)
{
  std::lock_guard<std::mutex> lock(mutex);
  currentCommand = command;
}

void DronePhysicalController::Accelerate()
{
  std::lock_guard<std::mutex> lock(mutex);

  if (state.Velocity < currentCommand->SpeedToReach) {
    state.Velocity += speedChangeStep;
  }

  if (state.Velocity > currentCommand->SpeedToReach) {
    state.Velocity = currentCommand->SpeedToReach;
  }

  if (state.Velocity == currentCommand->SpeedToReach) {
    ResetCommandToNull();
  }
}

void DronePhysicalController::Decelerate()
{
  std::lock_guard<std::mutex> lock(mutex);

  if (state.Velocity > currentCommand->SpeedToReach) {
    state.Velocity -= speedChangeStep;
  }

  if (state.Velocity < currentCommand->SpeedToReach) {
    state.Velocity = currentCommand->SpeedToReach;
  }

  if (state.Velocity == currentCommand->SpeedToReach) {
    ResetCommandToNull();
  }

  if (state.Velocity == 0) {
    if (DroneStoped) {
      DroneStoped();
    }
  }
}

void DronePhysicalController::Rotate()
{
  std::lock_guard<std::mutex> lock(mutex);

  if (currentCommand->AngleToRotate > 0) {
    state.Direction += rotateChangeStep;
    currentCommand->AngleToRotate -= rotateChangeStep;
  }
  else {
    state.Direction -= rotateChangeStep;
    currentCommand->AngleToRotate += rotateChangeStep;
  }

  if (MathCalculator::AreEqual(currentCommand->AngleToRotate, 0)) {
    // we made a rotation, remove command
    ResetCommandToNull();
  }
}

void DronePhysicalController::UpdatePosition()
{
  std::lock_guard<std::mutex> lock(mutex);
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
}

void DronePhysicalController::OnLoopStepStart()
{
  std::lock_guard<std::mutex> lock(mutex);

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
    case DataStructs::TURNING:
      Rotate();
    default:
      break;
  }
}

void DronePhysicalController::OnLoopStepEnd()
{
  UpdatePosition();
}

void DronePhysicalController::OnAfterStepEndAction() {}
