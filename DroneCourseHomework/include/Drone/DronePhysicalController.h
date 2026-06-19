#pragma once
#include "Drone/DroneCalculator.h"
#include "Threads/BaseLoop.h"
#include "DataStructs.h"

class DronePhysicalController : public BaseLoop {
public:
  DronePhysicalController(const float& stepTime, const int& timeScale, const DataStructs::DroneInputData& inputData);
  virtual ~DronePhysicalController() = default;
  DataStructs::DronePhysicalState GetState();

  // functions
protected:
  void OnLoopStepStart() override;
  void OnLoopStepEnd() override;
  void OnAfterStepEndAction() override;

private:
  void CalculateAcceleration(const float& accelerationPath);
  void Accelerate();
  void Decelerate();

  // variables
private:
  DroneCalculator droneCalculator;
  DataStructs::DronePhysicalState state;
  float maxSpeed;
  float angularSpeed;
  float turnThreshold;

  float accelerationTime;
  float speedChangeStep;   // how velocity can change during single step time
  float rotateChangeStep;  // how drone can rotate during single step time
};