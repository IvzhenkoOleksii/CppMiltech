#include "SimulationController.h"

SimulationController::SimulationController(const float& simStepTime)
{
  currentStepIndex = 0;
  currentTime = 0;
  simulationStepTime = simStepTime;
}

auto SimulationController::IsWorking() -> bool
{
  return currentStepIndex < MaxSimulationSteps;
}

auto SimulationController::GetStepTime() -> float
{
  return simulationStepTime;
}

void SimulationController::Update()
{
  ++currentStepIndex;
  currentTime += simulationStepTime;
}