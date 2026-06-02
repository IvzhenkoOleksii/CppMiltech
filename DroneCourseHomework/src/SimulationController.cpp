#include "SimulationController.h"

SimulationController::SimulationController(const float& simStepTime)
{
  currentSimulationStep = 0;
  currentSimulationTime = 0;
  simulationStepTime = simStepTime;
  isActive = true;
}

bool SimulationController::IsWorking()
{
  return isActive && currentSimulationStep < MaxSimulationSteps;
}

int SimulationController::GetCurrentStep()
{
  return currentSimulationStep;
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

void SimulationController::Stop()
{
  isActive = false;
}