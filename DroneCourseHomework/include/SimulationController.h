#pragma once

#include "Threads/BaseLoop.h"

constexpr int MaxSimulationSteps = 100;

class SimulationController : public BaseLoop {
public:
  SimulationController(const float& stepTime);

protected:
  // functions
  void LoopFunction() override;

public:
  float GetSimulationStepTime();

private:
  int currentSimulationStep;
  float simulationStepTime;
  float currentSimulationTime;
};