#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>

#include "Calculator.h"
#include "Constants.h"
#include "Structures.h"

Calculator::Calculator()
{
    distancePerTick = (2 * M_PI * WHEEL_RADIUS_M) / TICKS_PER_REVOLUTION;
    std::cout << "distancePerTick:  " << distancePerTick << std::endl;
}

void Calculator::UpdateState(
    const Structures::WheelsOdometrData& currentStep, 
    const Structures::WheelsOdometrData& previousStep,
    Structures::NrkState& currentState)
{
    double leftDeltaMeters = CalculateDistanceLeft (currentStep, previousStep);
    double rightDeltaMeters = CalculateDistanceRight (currentStep, previousStep);

    double distanceFromCenterMeters = (leftDeltaMeters + rightDeltaMeters) / 2;

    double thetaDelta = (rightDeltaMeters - leftDeltaMeters) / WHEEL_BASE_M;

    double updateTheta =  currentState.Theta + thetaDelta / 2;
    float updateThetaCos = cosf(updateTheta);
    float updateThetaSin = sinf(updateTheta);

    float xUpdate = distanceFromCenterMeters * updateThetaCos;
    float yUpdate = distanceFromCenterMeters * updateThetaSin;
    
    currentState.X += xUpdate;
    currentState.Y += yUpdate;
    currentState.Theta += thetaDelta;
}

double Calculator::CalculateDistanceLeft(    
    const Structures::WheelsOdometrData& currentStep, 
    const Structures::WheelsOdometrData& previousStep)
{
    long forwardLeftDelta = currentStep.ForwardLeftWheel - previousStep.ForwardLeftWheel;
    long backLeftDelta = currentStep.BackwardLeftWheel - previousStep.BackwardLeftWheel;
    long leftDelta = (forwardLeftDelta + backLeftDelta) / 2;
    double leftDeltaMeters = distancePerTick * leftDelta;

    return leftDeltaMeters;
}

double Calculator::CalculateDistanceRight(    
    const Structures::WheelsOdometrData& currentStep, 
    const Structures::WheelsOdometrData& previousStep)
{
    long forwardRightDelta = currentStep.ForwardRightWheel - previousStep.ForwardRightWheel;
    long backRightDelta = currentStep.BackwardRightWheel - previousStep.BackwardRightWheel;
    long rightDelta = (forwardRightDelta + backRightDelta) / 2;
    float rightDeltaMeters = distancePerTick * rightDelta;

    return rightDeltaMeters;
}