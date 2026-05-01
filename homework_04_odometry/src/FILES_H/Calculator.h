#include "Structures.h"

class Calculator
{
    public:Calculator();
    public:void UpdateState(
        const Structures::WheelsOdometrData& currentStep, 
        const Structures::WheelsOdometrData& previousStep,
        Structures::NrkState& currentState);

    private:double CalculateDistanceLeft(
        const Structures::WheelsOdometrData& currentStep, 
        const Structures::WheelsOdometrData& previousStep);

    private:double CalculateDistanceRight(
        const Structures::WheelsOdometrData& currentStep, 
        const Structures::WheelsOdometrData& previousStep);

    private:float distancePerTick;
};