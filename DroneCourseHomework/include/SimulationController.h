#pragma once

#include "Threads/BaseLoop.h"

constexpr int MaxSimulationSteps = 200;  // must be 10000

class SimulationController : public BaseLoop {
public:
  SimulationController(const float& stepTime, const int& timeScale);
  virtual ~SimulationController() = default;

protected:
  // functions
  void OnLoopStepStart() override;
  void OnLoopStepEnd() override;
  void OnAfterStepEndAction() override;

public:
  float GetStepTime();

private:
  int currentStepIndex;
};