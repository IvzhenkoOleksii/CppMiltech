#include "SimulationController.h"

SimulationController::SimulationController(const float& simStepTime)
{
  currentSimulationStep = 0;
  currentSimulationTime = 0;
  simulationStepTime = simStepTime;
}

bool SimulationController::IsWorking()
{
  return currentSimulationStep < MaxSimulationSteps;
}

float SimulationController::GetSimulationStepTime()
{
  return simulationStepTime;
}

void SimulationController::Update()
{
  ++currentSimulationStep;
  currentSimulationTime += simulationStepTime;
}