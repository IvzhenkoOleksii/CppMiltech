#pragma once
#include "Threads/BaseLoop.h"
#include "DataStructs.h"

class DronePhysicalController : public BaseLoop {
public:
  DronePhysicalController(const float& stepTime, const int& timeScale, const DataStructs::DroneInputData& inputData);
  virtual ~DronePhysicalController() = default;
  DataStructs::DronePhysicalState GetState();

protected:
  void LoopFunction() override;

private:
  DataStructs::DronePhysicalState state;
  float maxSpeed;
  float angularSpeed;
  float turnThreshold;
};