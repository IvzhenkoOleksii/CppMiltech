#pragma once
#include "DataStructs.h"

class DroneCalculator {
public:
  DroneCalculator();

public:
  float CalculateDroneAcceleration(const float& droneAttackSpeed, const float& droneAccelerationPath);

public:
  float CalculateDistance(const DataStructs::Position2D& position1, const DataStructs::Position2D& position2);

public:
  float CalculateAngleBetweenDroneAndTarget(DataStructs::Position3D dronePosition,
                                            const float& droneDirection,
                                            const DataStructs::Position2D& targetPosition);

public:
  DataStructs::Position2D CalculateDirectionVector(const DataStructs::Position2D& dronePosition, const float& droneDirection);

public:
  bool AreFloatsClose(float f1, float f2);

public:
  bool IsDistanceBetweenPointSameAsNeeded(const DataStructs::Position2D& position1,
                                          const DataStructs::Position2D& position2,
                                          float neededDistance);

public:
  DataStructs::Position2D CalculateManeuverPosition(float minAttackDistance,
                                                    float droneToTargetDistance,
                                                    DataStructs::Position2D dronePosition,
                                                    DataStructs::Position2D targetPosition);

public:
  DataStructs::Position2D CalculateFirePosition(float fallDistance,
                                                float droneToTargetDistance,
                                                DataStructs::Position2D dronePosition,
                                                DataStructs::Position2D targetPosition);

private:
  float axisXModule;
};