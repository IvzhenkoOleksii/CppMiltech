#pragma once
#include "Drone/DroneCalculator.h"
#include "Drone/DronePhysicalCommand.h"
#include "Threads/BaseLoop.h"
#include "DataStructs.h"

class DronePhysicalController : public BaseLoop {
public:
  // constructors, destructors
  DronePhysicalController(const float& stepTime, const int& timeScale, const DataStructs::DroneInputData& inputData);
  virtual ~DronePhysicalController() = default;
  std::function<void()> DroneStoped;

  // getters
public:
  DataStructs::DronePhysicalState GetState();
  float GetHalfStepDistance();

  // functions
public:
  void ReceiveCommand(const DronePhysicalCommand& command);

protected:
  void OnLoopStepStart() override;
  void OnLoopStepEnd() override;
  void OnAfterStepEndAction() override;

private:
  void ResetCommandToNull();
  void CalculateAcceleration(const float& accelerationPath);
  void Accelerate();
  void Decelerate();
  void Rotate();
  void UpdatePosition();

  // variables
private:
  DroneCalculator droneCalculator;
  DataStructs::DronePhysicalState state;
  std::optional<DronePhysicalCommand> currentCommand;
  std::mutex physicalMutex;

  // these are comes from constructor or calculate ones and just using, so no need to threat them specially
  float maxSpeed;
  float angularSpeed;
  float turnThreshold;

  float accelerationTime;
  float speedChangeStep;   // how velocity can change during single step time
  float rotateChangeStep;  // how drone can rotate during single step time
};