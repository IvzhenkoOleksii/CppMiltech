#include "DroneCalculator.h"

#include <cmath>
#include <iostream>

DroneCalculator::DroneCalculator()
{
  //	float axisXVectorUnderSQRT = powf(1, 2) + powf(0, 2);
  // powf(0, 2) always 0
  float axisXVectorUnderSQRT = powf(1, 2);
  axisXModule = sqrtf(axisXVectorUnderSQRT);
}

auto DroneCalculator::CalculateDroneAcceleration(const float& droneAttackSpeed, const float& droneAccelerationPath) -> float
{
  float acc = 0;
  float attackSpeedPower2 = powf(droneAttackSpeed, 2);
  acc = attackSpeedPower2 / (2 * droneAccelerationPath);

  return acc;
}

auto DroneCalculator::CalculateDistance(const DataStructs::Position2D& position1, const DataStructs::Position2D& position2) -> float
{
  // use Pifagor theorem
  float powXDistance = powf(position1.X - position2.X, 2);
  float powYDistance = powf(position1.Y - position2.Y, 2);
  float distance = std::sqrt(powXDistance + powYDistance);

  return distance;
}

auto DroneCalculator::CalculateAngleBetweenDroneAndTarget(DataStructs::Position3D dronePosition,
                                                          const float& droneDirection,
                                                          const DataStructs::Position2D& targetPosition) -> float
{
  DataStructs::Position2D differenceVector = {targetPosition.X - dronePosition.X, targetPosition.Y - dronePosition.Y};
  float differenceVectorModule = sqrtf(powf(differenceVector.X, 2) + powf(differenceVector.Y, 2));
  if (AreFloatsClose(differenceVectorModule, 0.0f)) {
    // Drone too close to target, no need to rotate
    return 0.0f;
  }

  DataStructs::Position2D directionVector = CalculateDirectionVector(dronePosition.GetPosition2D(), droneDirection);
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

  std::cout << "Angle between drone and target are:  " << angle << '\n';
  return angle;
}

auto DroneCalculator::AreFloatsClose(float f1, float f2) -> bool
{
  return std::abs(f1 - f2) <= 1e-9;
}

auto DroneCalculator::CalculateDirectionVector(const DataStructs::Position2D& dronePosition,
                                               const float& droneDirection) -> DataStructs::Position2D
{
  float dirVectorx = dronePosition.X + cosf(droneDirection);
  float dirVectorY = dronePosition.Y + sinf(droneDirection);
  DataStructs::Position2D directionVector = {dirVectorx - dronePosition.X, dirVectorY - dronePosition.Y};
  return directionVector;
}

auto DroneCalculator::CalculateManeuverPosition(float minAttackDistance,
                                                float droneToTargetDistance,
                                                DataStructs::Position2D dronePosition,
                                                DataStructs::Position2D targetPosition) -> DataStructs::Position2D
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

  std::cout << "Maneuver position X: " << x << "    Y: " << y << '\n';
  return {x, y};
}

auto DroneCalculator::CalculateFirePosition(float fallDistance,
                                            float droneToTargetDistance,
                                            DataStructs::Position2D dronePosition,
                                            DataStructs::Position2D targetPosition) -> DataStructs::Position2D
{
  float ratio = 0;
  if (droneToTargetDistance > 0.0) {
    ratio = (droneToTargetDistance - fallDistance) / droneToTargetDistance;
  }

  float fireX = dronePosition.X + (targetPosition.X - dronePosition.X) * ratio;
  float fireY = dronePosition.Y + (targetPosition.Y - dronePosition.Y) * ratio;

  DataStructs::Position2D firePosition = DataStructs::Position2D(fireX, fireY);
  std::cout << "Fire position X: " << fireX << "    Y: " << fireY << '\n';

  return firePosition;
}

auto DroneCalculator::IsDistanceBetweenPointSameAsNeeded(const DataStructs::Position2D& position1,
                                                         const DataStructs::Position2D& position2,
                                                         float neededDistance) -> bool
{
  float distance = CalculateDistance(position1, position2);
  float difference = distance - neededDistance;
  float differenceAbs = std::fabs(difference);
  bool isDistanceGoodToFire = differenceAbs < 1;

  return isDistanceGoodToFire;
}