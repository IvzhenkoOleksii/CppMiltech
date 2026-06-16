#include "SimulationController.h"

SimulationController::SimulationController(const float& stepTime)
  : BaseLoop(stepTime)
{
  currentStepIndex = 0;
}

void SimulationController::LoopFunction()
{
  std::chrono::duration duration = std::chrono::duration<float>{stepTime};
  while (isLoopActive) {
    if (LoopStepStartedAction) {
      LoopStepStartedAction();
    }

    std::this_thread::sleep_for(duration);

    ++currentStepIndex;

    if (LoopStepEndedAction) {
      LoopStepEndedAction();
    }

    // check if we out of steps
    if (currentStepIndex > MaxSimulationSteps) {
      FinishLoopThread();
    }
  }
}

float SimulationController::GetStepTime()
{
  return stepTime;
}