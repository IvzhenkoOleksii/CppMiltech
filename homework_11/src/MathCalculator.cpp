#include <cmath>
#include <iostream>

#include "MathCalculator.h"

// will be static
bool MathCalculator::AreEqual(float a, float b)
{
  float abs_epsilon = 1e-5f;
  float rel_epsilon = 1e-5f;

  // 1. Check absolute difference (handles numbers close to zero)
  if (std::abs(a - b) <= abs_epsilon) {
    return true;
  }

  // 2. Check relative difference (scales automatically for large numbers)
  return std::abs(a - b) <= (std::max(std::abs(a), std::abs(b)) * rel_epsilon);
}

float MathCalculator::GetSign(float value)
{
  if (value >= 0) {
    return 1;
  }

  return -1;
}

DataStructs::Coord2D MathCalculator::GetDirectionVector(const DataStructs::Coord2D& start, const DataStructs::Coord2D& end)
{
  float dX = end.X - start.X;
  float dY = end.Y - start.Y;

  DataStructs::Coord2D answ = {dX, dY};
  return answ;
}

DataStructs::Coord2D MathCalculator::GetDirectionVector(float angleInRadians)
{
  return DataStructs::Coord2D{std::cos(angleInRadians), std::sin(angleInRadians)};
}

float MathCalculator::VectorLength(float dx, float dy)
{
  return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
}

float MathCalculator::VectorLength(const DataStructs::Coord2D& delta)
{
  return std::sqrt(std::pow(delta.X, 2) + std::pow(delta.Y, 2));
}

float MathCalculator::DistanceBetweenPoints(const DataStructs::Coord2D& point1, const DataStructs::Coord2D& point2)
{
  // use Pifagor theorem
  float powDistanceX = std::pow(point1.X - point2.X, 2);
  float powDistanceY = std::pow(point1.Y - point2.Y, 2);
  return std::sqrt(powDistanceX + powDistanceY);
}

// squared much faster, use it if we can compare squared values
float MathCalculator::DistanceBetweenPointsSquared(const DataStructs::Coord2D& point1, const DataStructs::Coord2D& point2)
{
  float powDistanceX = std::pow(point1.X - point2.X, 2);
  float powDistanceY = std::pow(point1.Y - point2.Y, 2);
  return powDistanceX + powDistanceY;
}

bool MathCalculator::AreVectorsParallel(const DataStructs::Coord2D& vector1, const DataStructs::Coord2D& vector2)
{
  float vectorCrossProduct = VectorsCrossProduct(vector1, vector2);
  bool isEqual = AreEqual(vectorCrossProduct, 0);
  return isEqual;
}

float MathCalculator::VectorsCrossProduct(const DataStructs::Coord2D& vector1, const DataStructs::Coord2D& vector2)
{
  return vector1.X * vector2.Y - vector1.Y * vector2.X;
}

float MathCalculator::VectorsDotProduct(const DataStructs::Coord2D& vector1, const DataStructs::Coord2D& vector2)
{
  return vector1.X * vector2.X + vector1.Y * vector2.Y;
}

float MathCalculator::AngleBetweenVectorAndPoint(const DataStructs::Coord2D& point1, float direction1, const DataStructs::Coord2D& point2)
{
  DataStructs::Coord2D differenceVector = GetDirectionVector(point1, point2);
  float differenceVectorModule = std::pow(differenceVector.X, 2) + std::pow(differenceVector.Y, 2);

  if (AreEqual(differenceVectorModule, 0)) {
    // point1 too close to point2, no need to rotate
    return 0;
  }

  DataStructs::Coord2D dir1Vector = GetDirectionVector(direction1);
  float dir1VectorModule = std::pow(dir1Vector.X, 2) + std::pow(dir1Vector.Y, 2);

  // instead of sqrt differenceVectorModule and dir1VectorModule, we will pow dotProduct
  // it will be faster to compute a lot
  float dotProduct = VectorsDotProduct(differenceVector, dir1Vector);
  float cosAlpha = std::pow(dotProduct, 2) / (differenceVectorModule * dir1VectorModule);

  if (cosAlpha > 1) {
    //  std::cerr << "Something wrong here!  Cos alpha is greater than 1!" << std::endl;
    float difference = 1 - cosAlpha;
    if (difference < 0.000f) {
      cosAlpha = 1;
    }
  }

  float angle = std::acos(cosAlpha);
  float crossProduct = VectorsCrossProduct(differenceVector, dir1Vector);

  float sign = -GetSign(crossProduct);
  angle *= sign;
  return angle;
}