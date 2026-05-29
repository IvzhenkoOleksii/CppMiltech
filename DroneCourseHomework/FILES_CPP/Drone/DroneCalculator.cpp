#include "Drone/DroneCalculator.h"

#include <cmath>
#include <iostream>

DroneCalculator::DroneCalculator()
{
  math = {};
}

float DroneCalculator::CalculateDroneAcceleration(const float& droneAttackSpeed, const float& droneAccelerationPath)
{
  float attackSpeedPower2 = powf(droneAttackSpeed, 2);
  float acc = attackSpeedPower2 / (2 * droneAccelerationPath);

  return acc;
}

bool DroneCalculator::IsDistanceBetweenPointSameAsNeeded(const DataStructs::Point2D& point1,
                                                         const DataStructs::Point2D& point2,
                                                         float neededDistance)
{
  float distance = math.DistanceBetweenPoints(point1, point2);
  float difference = distance - neededDistance;
  float differenceAbs = fabs(difference);
  bool isDistanceGoodToFire = differenceAbs < 1;

  return isDistanceGoodToFire;
}

bool DroneCalculator::IsDistanceBetweenPointSameAsNeeded(const DataStructs::Point2D& point1,
                                                         const DataStructs::Point2D& point2,
                                                         float neededDistance,
                                                         float droneStepDistance)
{
  float distance = math.DistanceBetweenPoints(point1, point2);
  float difference = distance - neededDistance;
  float differenceAbs = fabs(difference);
  float halfDroneStepDistance = droneStepDistance / 2;
  bool isDistanceGoodToFire = differenceAbs < halfDroneStepDistance;

  return isDistanceGoodToFire;
}

DataStructs::Point2D DroneCalculator::CalculateManeuverPosition(float minAttackDistance,
                                                                float droneToTargetDistance,
                                                                DataStructs::Point2D dronePosition,
                                                                DataStructs::Point2D targetPosition)
{
  float x;
  float y;
  if (droneToTargetDistance > 0.0) {
    x = targetPosition.X - ((targetPosition.X - dronePosition.X) * minAttackDistance / droneToTargetDistance);
    y = targetPosition.Y - ((targetPosition.Y - dronePosition.Y) * minAttackDistance / droneToTargetDistance);
  }
  else {
    x = targetPosition.X + minAttackDistance;
    y = targetPosition.Y;
  }

  std::cout << "Maneuver position X: " << x << "    Y: " << y << std::endl;
  return DataStructs::Point2D(x, y);
}

DataStructs::Point2D DroneCalculator::CalculateFirePosition(float fallDistance,
                                                            float droneToTargetDistance,
                                                            DataStructs::Point2D dronePosition,
                                                            DataStructs::Point2D targetPosition)
{
  float ratio = 0;
  if (droneToTargetDistance > 0.0) {
    ratio = (droneToTargetDistance - fallDistance) / droneToTargetDistance;
  }

  float fireX = dronePosition.X + (targetPosition.X - dronePosition.X) * ratio;
  float fireY = dronePosition.Y + (targetPosition.Y - dronePosition.Y) * ratio;

  DataStructs::Point2D firePosition = DataStructs::Point2D(fireX, fireY);
  std::cout << "Fire position X: " << fireX << "    Y: " << fireY << std::endl;

  return firePosition;
}
