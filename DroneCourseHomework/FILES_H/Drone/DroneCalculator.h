#pragma once
#include "DataStructs.h"

class DroneCalculator {
public:
  DroneCalculator();

public:
  float CalculateDroneAcceleration(const float& droneAttackSpeed, const float& droneAccelerationPath);

public:
  float CalculateDistance(const DataStructs::Point2D& position1, const DataStructs::Point2D& position2);

public:
  float CalculateAngleBetweenDroneAndTarget(DataStructs::Point3D dronePosition,
                                            const float& droneDirection,
                                            const DataStructs::Point2D& targetPosition);

public:
  DataStructs::Point2D CalculateDirectionVector(const DataStructs::Point2D& dronePosition, const float& droneDirection);

public:
  bool AreFloatsClose(float f1, float f2);

public:
  bool IsDistanceBetweenPointSameAsNeeded(const DataStructs::Point2D& position1,
                                          const DataStructs::Point2D& position2,
                                          float neededDistance);

public:
  DataStructs::Point2D CalculateManeuverPosition(float minAttackDistance,
                                                 float droneToTargetDistance,
                                                 DataStructs::Point2D dronePosition,
                                                 DataStructs::Point2D targetPosition);

public:
  DataStructs::Point2D CalculateFirePosition(float fallDistance,
                                             float droneToTargetDistance,
                                             DataStructs::Point2D dronePosition,
                                             DataStructs::Point2D targetPosition);

private:
  float axisXModule;
};