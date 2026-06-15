#pragma once

#include "Threads/BaseLoop.h"

constexpr int MaxSimulationSteps = 100;  // must be 10000

class SimulationController : public BaseLoop {
public:
  SimulationController(const float& stepTime);
  virtual ~SimulationController() = default;

protected:
  // functions
  void LoopFunction() override;

public:
  float GetStepTime();

private:
  int currentStepIndex;
  float currentTime;
};