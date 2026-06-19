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

  CalculateAcceleration(inputData.AccelerationPath);

  StartLoopThread();
}

void DronePhysicalController::CalculateAcceleration(const float& accelerationPath)
{
  float acceleration = droneCalculator.CalculateDroneAcceleration(maxSpeed, accelerationPath);
  accelerationTime = maxSpeed / acceleration;
  speedChangeStep = maxSpeed / (accelerationTime / stepTime);
}

void DronePhysicalController::Accelerate()
{
  if (state.Velocity < maxSpeed) {
    state.Velocity += speedChangeStep;
  }

  if (state.Velocity > maxSpeed) {
    state.Velocity = maxSpeed;
  }
}

void DronePhysicalController::Decelerate()
{
  if (state.Velocity > 0) {
    state.Velocity -= speedChangeStep;
  }

  if (state.Velocity < 0) {
    state.Velocity = 0;
  }
}

void DronePhysicalController::OnLoopStepStart() {}

void DronePhysicalController::OnLoopStepEnd() {}

void DronePhysicalController::OnAfterStepEndAction() {}
