#pragma once
#include "DataStructs.h"

class DroneCalculator {
public:
  DroneCalculator() = default;

public:
  float CalculateDroneAcceleration(const float& droneAttackSpeed, const float& droneAccelerationPath);

  bool IsDistanceBetweenPointSameAsNeeded(const DataStructs::Coord2D& point1, const DataStructs::Coord2D& point2, float neededDistance);

  bool IsDistanceBetweenPointSameAsNeeded(const DataStructs::Coord2D& point1,
                                          const DataStructs::Coord2D& point2,
                                          float neededDistance,
                                          float droneStepDistance);

  DataStructs::Coord2D CalculateManeuverPosition(float minAttackDistance,
                                                 float droneToTargetDistance,
                                                 DataStructs::Coord2D dronePosition,
                                                 DataStructs::Coord2D targetPosition);

  DataStructs::Coord2D CalculateFirePosition(float fallDistance,
                                             float droneToTargetDistance,
                                             DataStructs::Coord2D dronePosition,
                                             DataStructs::Coord2D targetPosition);

  DataStructs::Coord2D CalculateAimPoint(const DataStructs::DronePhysicalState& physicalState, const float& fallDistance);

  DataStructs::Coord2D CalculateDropPoint(const DataStructs::DronePhysicalState& physicalState,
                                          const DataStructs::Coord2D& targetPosition,
                                          const float& fallDistance);
};
