#include "SimulationController.h"

SimulationController::SimulationController(const float& stepTime, const int& timeScale)
  : BaseLoop(stepTime, timeScale)
{
  currentStepIndex = 0;
}

void SimulationController::OnLoopStepStart()
{
  //  std::cout << "STEP:   " << currentStepIndex << " STARTED " << std::endl;
}

void SimulationController::OnLoopStepEnd()
{
  ++currentStepIndex;
}

void SimulationController::OnAfterStepEndAction()
{
  // check if we out of steps
  if (currentStepIndex > MaxSimulationSteps) {
    FinishLoopThread();
  }
}

float SimulationController::GetStepTime()
{
  return stepTime;
}