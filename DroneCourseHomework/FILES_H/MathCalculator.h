#pragma once

#include "DataStructs.h"

// will be static
class MathCalculator {
public:
  bool AreEqual(float a, float b);
  float GetSign(float value);

  DataStructs::Position2D GetDirectionVector(const DataStructs::Position2D& start, const DataStructs::Position2D& end);
  DataStructs::Position2D GetDirectionVector(float angleInRadians);

  float VectorLength(float dx, float dy);
  float VectorLength(DataStructs::Position2D delta);
  float DistanceBetweenPoints(DataStructs::Position2D point1, DataStructs::Position2D point2);
  float DistanceBetweenPointsSquared(DataStructs::Position2D point1, DataStructs::Position2D point2);

  bool AreVectorsParallel(DataStructs::Position2D vector1, DataStructs::Position2D vector2);
  float VectorsCrossProduct(DataStructs::Position2D vector1, DataStructs::Position2D vector2);
  float VectorsDotProduct(DataStructs::Position2D vector1, DataStructs::Position2D vector2);
  float AngleBetweenVectorAndPoint(DataStructs::Position2D point1, float direction1, DataStructs::Position2D point2);
};