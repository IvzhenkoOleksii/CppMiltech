#pragma once
#include <vector>
#include <iostream>

#include "Structures.h"

class InputFile
{
    public:std::vector<Structures::WheelsOdometrData> ReadTxtFile();

    private:void ReadLine(const std::string& line, std::vector<int>& lineData);
    private:Structures::WheelsOdometrData FormData(const std::vector<int>& data);
};