#include "Drone/DroneCalculator.h"
#include "MathCalculator.h"

#include <cmath>

float DroneCalculator::CalculateDroneAcceleration(const float& droneAttackSpeed, const float& droneAccelerationPath)
{
  float attackSpeedPower2 = powf(droneAttackSpeed, 2);
  float acc = attackSpeedPower2 / (2 * droneAccelerationPath);

  return acc;
}

bool DroneCalculator::IsDistanceBetweenPointSameAsNeeded(const DataStructs::Coord2D& point1,
                                                         const DataStructs::Coord2D& point2,
                                                         float neededDistance)
{
  float distance = MathCalculator::DistanceBetweenPoints(point1, point2);
  float difference = distance - neededDistance;
  float differenceAbs = fabs(difference);
  bool isDistanceGood = differenceAbs < 1;

  return isDistanceGood;
}

bool DroneCalculator::IsDistanceBetweenPointSameAsNeeded(const DataStructs::Coord2D& point1,
                                                         const DataStructs::Coord2D& point2,
                                                         float neededDistance,
                                                         float halfDroneStepDistance)
{
  float distance = MathCalculator::DistanceBetweenPoints(point1, point2);
  float difference = distance - neededDistance;
  float differenceAbs = fabs(difference);
  bool isDistanceGood = differenceAbs < halfDroneStepDistance;

  return isDistanceGood;
}

DataStructs::Coord2D DroneCalculator::CalculateManeuverPosition(float minAttackDistance,
                                                                float droneToTargetDistance,
                                                                DataStructs::Coord2D dronePosition,
                                                                DataStructs::Coord2D targetPosition)
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

  //  std::cout << "Maneuver position X: " << x << "    Y: " << y << std::endl;
  return DataStructs::Coord2D(x, y);
}

DataStructs::Coord2D DroneCalculator::CalculateFirePosition(float fallDistance,
                                                            float droneToTargetDistance,
                                                            DataStructs::Coord2D dronePosition,
                                                            DataStructs::Coord2D targetPosition)
{
  float ratio = 0;
  if (droneToTargetDistance > 0.0) {
    ratio = (droneToTargetDistance - fallDistance) / droneToTargetDistance;
  }

  float fireX = dronePosition.X + (targetPosition.X - dronePosition.X) * ratio;
  float fireY = dronePosition.Y + (targetPosition.Y - dronePosition.Y) * ratio;

  DataStructs::Coord2D firePosition = DataStructs::Coord2D(fireX, fireY);
  //  std::cout << "Fire position X: " << fireX << "    Y: " << fireY << std::endl;

  return firePosition;
}

DataStructs::Coord2D DroneCalculator::CalculateAimPoint(const DataStructs::DronePhysicalState& physicalState, const float& fallDistance)
{
  // point there bomb fall if drop it right now
  DataStructs::Coord2D dronePosition2D = DataStructs::Coord3D::GetPoint2D(physicalState.Position);

  DataStructs::Coord2D answer = {};
  answer.X = dronePosition2D.X + fallDistance * cosf(physicalState.Direction);
  answer.Y = dronePosition2D.Y + fallDistance * sinf(physicalState.Direction);
  return answer;
}

DataStructs::Coord2D DroneCalculator::CalculateDropPoint(const DataStructs::DronePhysicalState& physicalState,
                                                         const DataStructs::Coord2D& targetPosition,
                                                         const float& fallDistance)
{
  // point there need to drop bomb on target
  float direction = physicalState.Direction * (-1);  // we need point at opposite direction

  DataStructs::Coord2D answer = {};
  answer.X = targetPosition.X + fallDistance * cosf(direction);
  answer.Y = targetPosition.Y + fallDistance * sinf(direction);
  return answer;
}
