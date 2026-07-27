#define _USE_MATH_DEFINES

#include "Armament/Solver/ArmamentAnalyticalSolver.h"
#include "Armament/ArmamentDatabase.h"
#include "MathCalculator.h"

#include <cmath>
#include <iostream>

const float GRAVITY = 9.80665f;

ArmamentAnalitycalSolver::~ArmamentAnalitycalSolver() {}

ArmamentDatabase::FallResult ArmamentAnalitycalSolver::Calculate(const ArmamentDatabase::Data& armData,
                                                                 float droneAttackSpeed,
                                                                 float droneZPosition)
{
  float fallTime = CalculateFallTime(armData, droneAttackSpeed, droneZPosition);
  float fallDistance = CalculateFallDistance(armData, droneAttackSpeed, fallTime);
  ArmamentDatabase::FallResult result{fallTime, fallDistance};

  return result;
}

float ArmamentAnalitycalSolver::CalculateFallDistance(const ArmamentDatabase::Data& armData, float droneAttackSpeed, float fallTime)
{
  if (MathCalculator::AreEqual(droneAttackSpeed, 0)) {
    // bomb just fall down
    return 0;
  }

  std::cout << std::endl;
  // pre-calculate value, which used often
  float dragPower2 = powf(armData.Drag, 2);
  float dragPower3 = powf(armData.Drag, 3);
  float liftPower2 = powf(armData.Lift, 2);
  float liftPower2Plus1 = liftPower2 + 1;
  float liftPower4 = powf(armData.Lift, 4);

  float first = fallTime * droneAttackSpeed;
<<<<<<<< HEAD:homework_9/src/Armament/Solver/ArmamentAnalyticalSolver.cpp
  float second = (powf(fallTime, 2) * armData.Drag * droneAttackSpeed) / (2 * armData.Mass);
========
  // std::cout << "ArmamentAnalitycalSolver horizontal distance first coeff: " << first << std::endl;

  float second = (powf(fallTime, 2) * armData.Drag * droneAttackSpeed) / (2 * armData.Mass);
  // std::cout << "ArmamentAnalitycalSolver horizontal distance second coeff: " << second << std::endl;
>>>>>>>> Homework10:homework_10/src/Armament/Solver/ArmamentAnalyticalSolver.cpp

  float third_1 = 6 * armData.Drag * GRAVITY * armData.Lift * armData.Mass;
  float third_2 = 6 * dragPower2 * (liftPower2 - 1) * droneAttackSpeed;
  float third_down = 36 * powf(armData.Mass, 2);
  float third = (powf(fallTime, 3) * (third_1 - third_2)) / third_down;
<<<<<<<< HEAD:homework_9/src/Armament/Solver/ArmamentAnalyticalSolver.cpp
========
  //  std::cout << "ArmamentAnalitycalSolver horizontal distance third coeff: " << third << std::endl;
>>>>>>>> Homework10:homework_10/src/Armament/Solver/ArmamentAnalyticalSolver.cpp

  float fourth_1 = 3 * dragPower3 * liftPower2 * liftPower2Plus1 * droneAttackSpeed;
  float fourth_2 = 6 * dragPower3 * liftPower2Plus1 * liftPower4 * droneAttackSpeed;
  float fourth_3 = 6 * dragPower2 * GRAVITY * armData.Lift * armData.Mass * (liftPower4 + liftPower2Plus1);
  float fourth_down = 36 * powf(armData.Mass, 3) * powf(liftPower2Plus1, 2);
  float fourth = (powf(fallTime, 4) * (fourth_1 + fourth_2 - fourth_3)) / fourth_down;
<<<<<<<< HEAD:homework_9/src/Armament/Solver/ArmamentAnalyticalSolver.cpp
========
  //  std::cout << "ArmamentAnalitycalSolver horizontal distance fourth coeff: " << fourth << std::endl;
>>>>>>>> Homework10:homework_10/src/Armament/Solver/ArmamentAnalyticalSolver.cpp

  float fifth_1 = 3 * dragPower3 * GRAVITY * powf(armData.Lift, 3) * armData.Mass;
  float fifth_2 = 3 * powf(armData.Drag, 4) * liftPower2 * liftPower2Plus1 * droneAttackSpeed;
  float fifth_down = 36 * liftPower2Plus1 * powf(armData.Mass, 4);
  float fifth = (powf(fallTime, 5) * (fifth_1 - fifth_2)) / fifth_down;
<<<<<<<< HEAD:homework_9/src/Armament/Solver/ArmamentAnalyticalSolver.cpp

  float fallDistance = first - second + third + fourth + fifth;
========
  //  std::cout << "ArmamentAnalitycalSolver horizontal distance fifth coeff: " << fifth << std::endl;

  float fallDistance = first - second + third + fourth + fifth;
  //  std::cout << "ArmamentAnalitycalSolver horizontal fall distance: " << fallDistance << std::endl;
  std::cout << std::endl;
>>>>>>>> Homework10:homework_10/src/Armament/Solver/ArmamentAnalyticalSolver.cpp

  if (fallDistance <= 0) {
    std::cerr << "Error! Fall distance less than zero  " << std::endl;
    exit(1);
  }

  return fallDistance;
}

float ArmamentAnalitycalSolver::CalculateFallTime(float p, float phi, float a, float b)
{
  float first = -p / 3;
  float firstSquare = 2 * sqrt(first);

  float cosValue = (phi + 4 * M_PI) / 3;
  float second = cosf(cosValue);

  float third = 0;
  if (a != 0) {
    third = b / (3 * a);
  }

  float timeToFall = firstSquare * second - third;
<<<<<<<< HEAD:homework_9/src/Armament/Solver/ArmamentAnalyticalSolver.cpp
========
  //  std::cout << "ArmamentAnalitycalSolver timeToFall: " << timeToFall << std::endl;

>>>>>>>> Homework10:homework_10/src/Armament/Solver/ArmamentAnalyticalSolver.cpp
  return timeToFall;
}

float ArmamentAnalitycalSolver::CalculateFallTime(const ArmamentDatabase::Data& armData, float droneAttackSpeed, float droneZPosition)
{
  float coefA = CalculateCoefficientA(armData, droneAttackSpeed);
  float coefB = CalculateCoefficientB(armData, droneAttackSpeed);
  float coefC = CalculateCoefficientC(armData, droneZPosition);

  float cardanoP = CalculateCardanoP(coefA, coefB);
  float cardanoQ = CalculateCardanoQ(coefA, coefB, coefC);
  float cardanoPHI = CalculateCardanoPHI(cardanoP, cardanoQ);

  float fallTime = CalculateFallTime(cardanoP, cardanoPHI, coefA, coefB);

  if (fallTime <= 0) {
    std::cerr << "Error! Fall time less than zero  " << std::endl;
    exit(1);
  }

  return fallTime;
}

// private part: actual implementation
float ArmamentAnalitycalSolver::CalculateCoefficientA(const ArmamentDatabase::Data& armData, float droneAttackSpeed)
{
  float leftPart = armData.Drag * GRAVITY * armData.Mass;
  float rightPart = 2 * powf(armData.Drag, 2) * armData.Lift * droneAttackSpeed;
  float coefA = leftPart - rightPart;
<<<<<<<< HEAD:homework_9/src/Armament/Solver/ArmamentAnalyticalSolver.cpp
========
  //  std::cout << "ArmamentAnalitycalSolver coefA: " << coefA << std::endl;
>>>>>>>> Homework10:homework_10/src/Armament/Solver/ArmamentAnalyticalSolver.cpp

  return coefA;
}

float ArmamentAnalitycalSolver::CalculateCoefficientB(const ArmamentDatabase::Data& armData, float droneAttackSpeed)
{
  float leftPart = 3 * GRAVITY * powf(armData.Mass, 2);
  float rightPart = 3 * armData.Drag * armData.Lift * armData.Mass * droneAttackSpeed;
  float coefB = -leftPart + rightPart;
<<<<<<<< HEAD:homework_9/src/Armament/Solver/ArmamentAnalyticalSolver.cpp
========
  //  std::cout << "ArmamentAnalitycalSolver coefB: " << coefB << std::endl;
>>>>>>>> Homework10:homework_10/src/Armament/Solver/ArmamentAnalyticalSolver.cpp

  return coefB;
}

float ArmamentAnalitycalSolver::CalculateCoefficientC(const ArmamentDatabase::Data& armData, float droneZPosition)
{
  float coefC = 6 * powf(armData.Mass, 2) * droneZPosition;
<<<<<<<< HEAD:homework_9/src/Armament/Solver/ArmamentAnalyticalSolver.cpp
========
  //  std::cout << "ArmamentAnalitycalSolver coefC: " << coefC << std::endl;
>>>>>>>> Homework10:homework_10/src/Armament/Solver/ArmamentAnalyticalSolver.cpp

  return coefC;
}

float ArmamentAnalitycalSolver::CalculateCardanoP(float a, float b)
{
  float upperValue = powf(b, 2);
  float lowerValue = 3 * powf(a, 2);

  float cardanoP = 0;
  if (lowerValue != 0) {
    cardanoP = -upperValue / lowerValue;
  }

<<<<<<<< HEAD:homework_9/src/Armament/Solver/ArmamentAnalyticalSolver.cpp
========
  //  std::cout << "ArmamentAnalitycalSolver cardanoP: " << cardanoP << std::endl;
>>>>>>>> Homework10:homework_10/src/Armament/Solver/ArmamentAnalyticalSolver.cpp
  return cardanoP;
}

float ArmamentAnalitycalSolver::CalculateCardanoQ(float a, float b, float c)
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
<<<<<<<< HEAD:homework_9/src/Armament/Solver/ArmamentAnalyticalSolver.cpp
========
  //  std::cout << "ArmamentAnalitycalSolver cardanoQ: " << cardanoQ << std::endl;
>>>>>>>> Homework10:homework_10/src/Armament/Solver/ArmamentAnalyticalSolver.cpp

  return cardanoQ;
}

float ArmamentAnalitycalSolver::CalculateCardanoPHI(float p, float q)
{
  float first = 0;
  float second = 0;
  if (p != 0) {
    first = (3 * q) / (2 * p);
    second = sqrtf(-3 / p);
  }

  float value = first * second;
  float phi = acosf(value);
<<<<<<<< HEAD:homework_9/src/Armament/Solver/ArmamentAnalyticalSolver.cpp
========
  //  std::cout << "ArmamentAnalitycalSolver cardanoPHI: " << phi << std::endl;
>>>>>>>> Homework10:homework_10/src/Armament/Solver/ArmamentAnalyticalSolver.cpp

  return phi;
}
