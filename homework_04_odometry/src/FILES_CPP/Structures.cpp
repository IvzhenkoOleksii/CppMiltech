#include "Structures.h"

#include <iostream>

void Structures::WheelsOdometrData::Print()
{
    std::cout << "TimeStamp is:  " << TimeStamp << std::endl;
    std::cout << "ForwardLeftWheel is:  " << ForwardLeftWheel << std::endl;
    std::cout << "ForwardRightWheel is:  " << ForwardRightWheel << std::endl;
    std::cout << "BackwardLeftWheel is:  " << BackwardLeftWheel << std::endl;
    std::cout << "BackwardRightWheel is:  " << BackwardRightWheel << std::endl;
}