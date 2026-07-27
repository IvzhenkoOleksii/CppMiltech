#pragma once

#include "DataStructs.h"

class MathCalculator {
public:
  static bool AreEqual(float a, float b);
  static float GetSign(float value);

  static DataStructs::Coord2D GetDirectionVector(const DataStructs::Coord2D& start, const DataStructs::Coord2D& end);
  static DataStructs::Coord2D GetDirectionVector(float angleInRadians);

  static float VectorLength(float dx, float dy);
  static float VectorLength(const DataStructs::Coord2D& delta);
  static float DistanceBetweenPoints(const DataStructs::Coord2D& point1, const DataStructs::Coord2D& point2);
  static float DistanceBetweenPointsSquared(const DataStructs::Coord2D& point1, const DataStructs::Coord2D& point2);

  static bool AreVectorsParallel(const DataStructs::Coord2D& vector1, const DataStructs::Coord2D& vector2);
  static float VectorsCrossProduct(const DataStructs::Coord2D& vector1, const DataStructs::Coord2D& vector2);
  static float VectorsDotProduct(const DataStructs::Coord2D& vector1, const DataStructs::Coord2D& vector2);
  static float AngleBetweenVectorAndPoint(const DataStructs::Coord2D& point1, float direction1, const DataStructs::Coord2D& point2);
};