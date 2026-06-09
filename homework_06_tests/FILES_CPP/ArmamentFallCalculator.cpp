#define USE_MATH_DEFINES

#include "ArmamentFallCalculator.h"

#include <cmath>
#include <iostream>

const float GRAVITY = 9.80665f;

auto ArmamentFallCalculator::CalculateFallTime(const ArmamentDatabase::Data& armData, float droneAttackSpeed, float droneZPosition) -> float
{
  std::cout << '\n';
  float coefA = CalculateCoefficientA(armData, droneAttackSpeed);
  float coefB = CalculateCoefficientB(armData, droneAttackSpeed);
  float coefC = CalculateCoefficientC(armData, droneZPosition);

  float cardanoP = CalculateCardanoP(coefA, coefB);
  float cardanoQ = CalculateCardanoQ(coefA, coefB, coefC);
  float cardanoPHI = CalculateCardanoPHI(cardanoP, cardanoQ);

  float fallTime = CalculateFallingTime(cardanoP, cardanoPHI, coefA, coefB);

  if (fallTime <= 0) {
    std::cout << "Error! Fall time less than zero  " << '\n';
    exit(1);
  }

  return fallTime;
}

auto ArmamentFallCalculator::CalculateCoefficientA(const ArmamentDatabase::Data& armData, float droneAttackSpeed) -> float
{
  float leftPart = armData.Drag * GRAVITY * armData.Mass;
  float rightPart = 2 * powf(armData.Drag, 2) * armData.Lift * droneAttackSpeed;
  float coefA = leftPart - rightPart;
  std::cout << "ArmamentFallCalculator coefA: " << coefA << '\n';

  return coefA;
}

auto ArmamentFallCalculator::CalculateCoefficientB(const ArmamentDatabase::Data& armData, float droneAttackSpeed) -> float
{
  float leftPart = 3 * GRAVITY * powf(armData.Mass, 2);
  float rightPart = 3 * armData.Drag * armData.Lift * armData.Mass * droneAttackSpeed;
  float coefB = -leftPart + rightPart;
  std::cout << "ArmamentFallCalculator coefB: " << coefB << '\n';

  return coefB;
}

auto ArmamentFallCalculator::CalculateCoefficientC(const ArmamentDatabase::Data& armData, float droneZPosition) -> float
{
  float coefC = 6 * powf(armData.Mass, 2) * droneZPosition;
  std::cout << "ArmamentFallCalculator coefC: " << coefC << '\n';

  return coefC;
}

auto ArmamentFallCalculator::CalculateCardanoP(float a, float b) -> float
{
  float upperValue = powf(b, 2);
  float lowerValue = 3 * powf(a, 2);

  float cardanoP = 0;
  if (lowerValue != 0) {
    cardanoP = -upperValue / lowerValue;
  }

  std::cout << "ArmamentFallCalculator cardanoP: " << cardanoP << '\n';
  return cardanoP;
}

auto ArmamentFallCalculator::CalculateCardanoQ(float a, float b, float c) -> float
{
  float first_up = 2 * powf(b, 3);
  float first_down = 27 * powf(a, 3);
  float first = 0;
  if (first_down != 0) {
    first = first_up / first_down;
  }

  float second = 0;
  if (a != 0) {
    second = c / a;
  }

  float cardanoQ = first + second;
  std::cout << "ArmamentFallCalculator cardanoQ: " << cardanoQ << '\n';

  return cardanoQ;
}

auto ArmamentFallCalculator::CalculateCardanoPHI(float p, float q) -> float
{
  float first = 0;
  float second = 0;
  if (p != 0) {
    first = (3 * q) / (2 * p);
    second = sqrtf(-3 / p);
  }

  float value = first * second;
  float phi = acosf(value);
  std::cout << "ArmamentFallCalculator cardanoPHI: " << phi << '\n';

  return phi;
}

auto ArmamentFallCalculator::CalculateFallingTime(float p, float phi, float a, float b) -> float
{
  float first = -p / 3;
  float firstSquare = 2 * std::sqrt(first);

  float cosValue = (phi + 4 * M_PI) / 3;
  float second = cosf(cosValue);

  float third = 0;
  if (a != 0) {
    third = b / (3 * a);
  }

  float timeToFall = firstSquare * second - third;
  std::cout << "ArmamentFallCalculator timeToFall: " << timeToFall << '\n';

  return timeToFall;
}

auto ArmamentFallCalculator::CalculateFallDistance(const ArmamentDatabase::Data& armData, float droneAttackSpeed, float fallTime) -> float
{
  std::cout << '\n';
  // pre-calculate value, which used often
  float dragPower2 = powf(armData.Drag, 2);
  float dragPower3 = powf(armData.Drag, 3);
  float liftPower2 = powf(armData.Lift, 2);
  float liftPower2Plus1 = liftPower2 + 1;
  float liftPower4 = powf(armData.Lift, 4);

  float first = fallTime * droneAttackSpeed;
  std::cout << "ArmamentFallCalculator horizontal distance first coeff: " << first << '\n';

  float second = (powf(fallTime, 2) * armData.Drag * droneAttackSpeed) / (2 * armData.Mass);
  std::cout << "ArmamentFallCalculator horizontal distance second coeff: " << second << '\n';

  float third_1 = 6 * armData.Drag * GRAVITY * armData.Lift * armData.Mass;
  float third_2 = 6 * dragPower2 * (liftPower2 - 1) * droneAttackSpeed;
  float third_down = 36 * powf(armData.Mass, 2);
  float third = (powf(fallTime, 3) * (third_1 - third_2)) / third_down;
  std::cout << "ArmamentFallCalculator horizontal distance third coeff: " << third << '\n';

  float fourth_1 = 3 * dragPower3 * liftPower2 * liftPower2Plus1 * droneAttackSpeed;
  float fourth_2 = 6 * dragPower3 * liftPower2Plus1 * liftPower4 * droneAttackSpeed;
  float fourth_3 = 6 * dragPower2 * GRAVITY * armData.Lift * armData.Mass * (liftPower4 + liftPower2Plus1);
  float fourth_down = 36 * powf(armData.Mass, 3) * powf(liftPower2Plus1, 2);
  float fourth = (powf(fallTime, 4) * (fourth_1 + fourth_2 - fourth_3)) / fourth_down;
  std::cout << "ArmamentFallCalculator horizontal distance fourth coeff: " << fourth << '\n';

  float fifth_1 = 3 * dragPower3 * GRAVITY * powf(armData.Lift, 3) * armData.Mass;
  float fifth_2 = 3 * powf(armData.Drag, 4) * liftPower2 * liftPower2Plus1 * droneAttackSpeed;
  float fifth_down = 36 * liftPower2Plus1 * powf(armData.Mass, 4);
  float fifth = (powf(fallTime, 5) * (fifth_1 - fifth_2)) / fifth_down;
  std::cout << "ArmamentFallCalculator horizontal distance fifth coeff: " << fifth << '\n';

  float fallDistance = first - second + third + fourth + fifth;
  std::cout << "ArmamentFallCalculator horizontal fall distance: " << fallDistance << '\n';

  if (fallDistance <= 0) {
    std::cout << "Error! Fall distance less than zero  " << '\n';
    exit(1);
  }

  return fallDistance;
}