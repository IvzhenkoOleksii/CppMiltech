#pragma once

#include "DataStructs.h"

class MathCalculator {
public:
  bool static AreEqual(float a, float b);
  float static GetSign(float value);

  DataStructs::Coord2D static GetDirectionVector(const DataStructs::Coord2D& start, const DataStructs::Coord2D& end);
  DataStructs::Coord2D static GetDirectionVector(float angleInRadians);

  float static VectorLength(float dx, float dy);
  float static VectorLength(const DataStructs::Coord2D& delta);
  float static DistanceBetweenPoints(const DataStructs::Coord2D& point1, const DataStructs::Coord2D& point2);
  float static DistanceBetweenPointsSquared(const DataStructs::Coord2D& point1, const DataStructs::Coord2D& point2);

  bool static AreVectorsParallel(const DataStructs::Coord2D& vector1, const DataStructs::Coord2D& vector2);
  float static VectorsCrossProduct(const DataStructs::Coord2D& vector1, const DataStructs::Coord2D& vector2);
  float static VectorsDotProduct(const DataStructs::Coord2D& vector1, const DataStructs::Coord2D& vector2);
  float static AngleBetweenVectorAndPoint(const DataStructs::Coord2D& point1, float direction1, const DataStructs::Coord2D& point2);
};