#pragma once

constexpr int MaxSimulationSteps = 10000;

class SimulationController {
public:
  bool IsWorking();

public:
  void Update();

public:
  float GetStepTime();

  SimulationController(const float& simStepTime);

private:
  int currentStepIndex;

private:
  float simulationStepTime;

private:
  float currentTime;
};