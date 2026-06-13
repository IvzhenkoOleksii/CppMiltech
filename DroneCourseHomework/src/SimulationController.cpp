#include "SimulationController.h"
#include <ostream>
#include <iostream>

SimulationController::SimulationController(const float& stepTime)
  : BaseLoop(stepTime)
{
  currentSimulationStep = 0;
  currentSimulationTime = 0;
  loopStep = stepTime;
}

void SimulationController::LoopFunction()
{
  std::chrono::duration duration = std::chrono::duration<float>{loopStep};
  while (isLoopActive) {
    std::cout << "STEP STARTED" << std::endl;

    if (LoopStepStartedAction) {
      LoopStepStartedAction();
    }

    std::this_thread::sleep_for(duration);

    ++currentSimulationStep;
    currentSimulationTime += simulationStepTime;

    if (LoopStepEndedAction) {
      LoopStepEndedAction();
    }

    std::cout << "STEP ENDED" << std::endl;
    // check if we out of steps
    if (currentSimulationStep > MaxSimulationSteps) {
      FinishLoopThread();
    }
  }
}

float SimulationController::GetSimulationStepTime()
{
  return simulationStepTime;
}