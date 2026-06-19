#include "SimulationController.h"

SimulationController::SimulationController(const float& stepTime, const int& timeScale)
  : BaseLoop(stepTime, timeScale)
{
  currentStepIndex = 0;
}

void SimulationController::LoopFunction()
{
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