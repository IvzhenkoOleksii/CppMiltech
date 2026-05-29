#pragma once
#include "DataStructs.h"
#include "MathCalculator.h"

class DroneCalculator {
public:
  DroneCalculator();

public:
  float CalculateDroneAcceleration(const float& droneAttackSpeed, const float& droneAccelerationPath);

public:
  bool IsDistanceBetweenPointSameAsNeeded(const DataStructs::Point2D& point1, const DataStructs::Point2D& point2, float neededDistance);

  bool IsDistanceBetweenPointSameAsNeeded(const DataStructs::Point2D& point1,
                                          const DataStructs::Point2D& point2,
                                          float neededDistance,
                                          float droneStepDistance);

public:
  DataStructs::Point2D CalculateManeuverPosition(float minAttackDistance,
                                                 float droneToTargetDistance,
                                                 DataStructs::Point2D dronePosition,
                                                 DataStructs::Point2D targetPosition);

  DataStructs::Point2D CalculateFirePosition(float fallDistance,
                                             float droneToTargetDistance,
                                             DataStructs::Point2D dronePosition,
                                             DataStructs::Point2D targetPosition);

private:
  MathCalculator math;
};
