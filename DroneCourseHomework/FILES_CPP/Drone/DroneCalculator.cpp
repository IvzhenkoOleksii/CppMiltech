#include "Drone/DroneCalculator.h"

#include <cmath>
#include <iostream>

DroneCalculator::DroneCalculator()
{
  //	float axisXVectorUnderSQRT = powf(1, 2) + powf(0, 2);
  // powf(0, 2) always 0
  float axisXVectorUnderSQRT = powf(1, 2);
  axisXModule = sqrtf(axisXVectorUnderSQRT);
}

float DroneCalculator::CalculateDroneAcceleration(const float& droneAttackSpeed, const float& droneAccelerationPath)
{
  float acc = 0;
  float attackSpeedPower2 = powf(droneAttackSpeed, 2);
  acc = attackSpeedPower2 / (2 * droneAccelerationPath);

  return acc;
}

float DroneCalculator::CalculateDistance(const DataStructs::Point2D& position1, const DataStructs::Point2D& position2)
{
  // use Pifagor theorem
  float powXDistance = powf(position1.X - position2.X, 2);
  float powYDistance = powf(position1.Y - position2.Y, 2);
  float distance = sqrt(powXDistance + powYDistance);

  return distance;
}

float DroneCalculator::CalculateAngleBetweenDroneAndTarget(DataStructs::Point3D dronePosition,
                                                           const float& droneDirection,
                                                           const DataStructs::Point2D& targetPosition)
{
  DataStructs::Point2D differenceVector = {targetPosition.X - dronePosition.X, targetPosition.Y - dronePosition.Y};
  float differenceVectorModule = sqrtf(powf(differenceVector.X, 2) + powf(differenceVector.Y, 2));
  if (AreFloatsClose(differenceVectorModule, 0.0f)) {
    // Drone too close to target, no need to rotate
    return 0.0f;
  }

  DataStructs::Point2D directionVector = CalculateDirectionVector(dronePosition.GetPoint2D(), droneDirection);
  float directionVectorModule = sqrtf(powf(directionVector.X, 2) + powf(directionVector.Y, 2));

  float dotProduct = differenceVector.X * directionVector.X + differenceVector.Y * directionVector.Y;
  float cosAlpha = dotProduct / (differenceVectorModule * directionVectorModule);
  float angle = acosf(cosAlpha);

  float crossProduct = differenceVector.X * directionVector.Y - differenceVector.Y * directionVector.X;
  float sign = 1.0f;
  if (crossProduct > 0.0f) {
    sign = -1.0f;
  }

  angle *= sign;

  std::cout << "Angle between drone and target are:  " << angle << std::endl;
  return angle;
}

bool DroneCalculator::AreFloatsClose(float f1, float f2)
{
  return std::abs(f1 - f2) <= 1e-9;
}

DataStructs::Point2D DroneCalculator::CalculateDirectionVector(const DataStructs::Point2D& dronePosition, const float& droneDirection)
{
  float dirVectorx = dronePosition.X + cosf(droneDirection);
  float dirVectorY = dronePosition.Y + sinf(droneDirection);
  DataStructs::Point2D directionVector = {dirVectorx - dronePosition.X, dirVectorY - dronePosition.Y};
  return directionVector;
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

bool DroneCalculator::IsDistanceBetweenPointSameAsNeeded(const DataStructs::Point2D& position1,
                                                         const DataStructs::Point2D& position2,
                                                         float neededDistance)
{
  float distance = CalculateDistance(position1, position2);
  float difference = distance - neededDistance;
  float differenceAbs = fabs(difference);
  bool isDistanceGoodToFire = differenceAbs < 1;

  return isDistanceGoodToFire;
}