#include "SimulationController.h"
#include <ostream>
#include <iostream>

SimulationController::SimulationController(const float& stepTime)
  : BaseLoop(stepTime)
{
  currentTime = 0;
  currentStepIndex = 0;
}

void SimulationController::LoopFunction()
{
  std::chrono::duration duration = std::chrono::duration<float>{stepTime};
  while (isLoopActive) {
    std::cout << "STEP STARTED" << std::endl;

    if (LoopStepStartedAction) {
      LoopStepStartedAction();
    }

    std::this_thread::sleep_for(duration);

    currentTime += stepTime;
    ++currentStepIndex;

    if (LoopStepEndedAction) {
      LoopStepEndedAction();
    }

    std::cout << "STEP ENDED" << std::endl;
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