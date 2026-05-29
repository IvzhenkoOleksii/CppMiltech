#pragma once

#include "DataStructs.h"

// will be static
class MathCalculator {
public:
  bool AreEqual(float a, float b);
  float GetSign(float value);

  DataStructs::Point2D GetDirectionVector(const DataStructs::Point2D& start, const DataStructs::Point2D& end);
  DataStructs::Point2D GetDirectionVector(float angleInRadians);

  float VectorLength(float dx, float dy);
  float VectorLength(DataStructs::Point2D delta);
  float DistanceBetweenPoints(DataStructs::Point2D point1, DataStructs::Point2D point2);
  float DistanceBetweenPointsSquared(DataStructs::Point2D point1, DataStructs::Point2D point2);

  bool AreVectorsParallel(DataStructs::Point2D vector1, DataStructs::Point2D vector2);
  float VectorsCrossProduct(DataStructs::Point2D vector1, DataStructs::Point2D vector2);
  float VectorsDotProduct(DataStructs::Point2D vector1, DataStructs::Point2D vector2);
  float AngleBetweenVectorAndPoint(DataStructs::Point2D point1, float direction1, DataStructs::Point2D point2);
};