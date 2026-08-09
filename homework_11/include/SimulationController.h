#pragma once

constexpr int MaxSimulationSteps = 2000;

class SimulationController {
public:
  SimulationController(const float& simStepTime);

public:
  bool IsWorking();
  void Update();
  float GetSimulationStepTime();
  void Stop();
  int GetCurrentStep();

private:
  bool isActive;
  int currentSimulationStep;
  float simulationStepTime;
  float currentSimulationTime;
};