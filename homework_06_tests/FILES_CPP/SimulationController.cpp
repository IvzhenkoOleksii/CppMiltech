#include "SimulationController.h"

SimulationController::SimulationController(const float& simStepTime)
{
  currentSimulationStep = 0;
  currentSimulationTime = 0;
  simulationStepTime = simStepTime;
}

auto SimulationController::IsWorking() -> bool
{
  return currentSimulationStep < MaxSimulationSteps;
}

auto SimulationController::GetSimulationStepTime() -> float
{
  return simulationStepTime;
}

void SimulationController::Update()
{
  ++currentSimulationStep;
  currentSimulationTime += simulationStepTime;
}