#pragma once
#include <vector>
#include <iostream>
#include <string>

#include "Structures.h"

class InputFile
{
    public:std::vector<Structures::WheelsOdometrData> ReadTxtFile(std::string fileName);

    private:void ReadLine(const std::string& line, std::vector<long>& lineData);
    private:Structures::WheelsOdometrData FormData(const std::vector<long>& data);
};