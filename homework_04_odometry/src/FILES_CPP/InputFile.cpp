#include <vector>
#include <iostream>
#include <fstream>

#include "InputFile.h"
#include "Structures.h"

std::vector<Structures::WheelsOdometrData> InputFile::ReadTxtFile()
{
    std::vector<Structures::WheelsOdometrData> data = {};

    std::ifstream file("./homework_04_odometry/data/straight.txt");
    if (file.is_open())
	{
        std::string line;
        while (std::getline(file, line))
		{
			std::cout << "Line inside input file is:  " << line << std::endl;
			ReadLine(line, &data);
		}	
    }
    else
	{
		std::cout << "Input file read error!  " << std::endl;
		exit(1);
	}
	file.close();


    return data;
}

void InputFile::ReadLine(const std::string& line, std::vector<Structures::WheelsOdometrData>* data)
{

}