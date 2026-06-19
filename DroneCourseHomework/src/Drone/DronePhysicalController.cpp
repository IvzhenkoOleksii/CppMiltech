#include "Drone/DronePhysicalController.h"

DronePhysicalController::DronePhysicalController(const float& stepTime, const int& timeScale, const DataStructs::DroneInputData& inputData)
  : BaseLoop(stepTime, timeScale)
{
  this->state.Velocity = 0;
  this->state.Position = inputData.Position;
  this->state.Direction = inputData.InitialDirection;

  this->maxSpeed = inputData.AttackSpeed;
  this->angularSpeed = inputData.AngularSpeed;
  this->turnThreshold = inputData.TurnThreshold;

  StartLoopThread();
}

void DronePhysicalController::LoopFunction()
{
  while (isLoopActive) {
    if (LoopStepStartedAction) {
      LoopStepStartedAction();
    }

    std::this_thread::sleep_for(duration);

    // we are updating target position here

    if (LoopStepEndedAction) {
      LoopStepEndedAction();
    }
  }
}