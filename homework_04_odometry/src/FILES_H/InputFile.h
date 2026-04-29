#pragma once
#include <vector>
#include <iostream>

#include "Structures.h"

class InputFile
{
    public:std::vector<Structures::WheelsOdometrData> ReadTxtFile();

    private:void ReadLine(const std::string& line, 
        std::vector<Structures::WheelsOdometrData>* data
    );
};