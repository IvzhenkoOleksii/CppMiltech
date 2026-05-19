#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "InputFile.h"
#include "Structures.h"

std::vector<Structures::WheelsOdometrData> InputFile::ReadTxtFile(std::string fileName)
{
    std::vector<Structures::WheelsOdometrData> data = {};
	std::vector<long> odmRawData = {};

	std::cout << "fileName:  " << fileName << std::endl;
    std::ifstream file("./homework_04_odometry/data/" + fileName);
    if (file.is_open())
	{
        std::string line;
        while (std::getline(file, line))
		{
			ReadLine(line, odmRawData);
			Structures::WheelsOdometrData odmValues = FormData(odmRawData);
			odmRawData.clear();
			data.push_back(odmValues);
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

void InputFile::ReadLine(const std::string& line, std::vector<long>& lineData)
{
	std::stringstream splitted(line);
	std::string word;
	std::vector<std::string> stringVector;

	while (splitted >> word)
	{
		stringVector.push_back(word);
	}

	for(size_t i = 0; i < stringVector.size(); ++i)
	{
		std::string val = stringVector[i];
		long longVal = std::stof(val);
		lineData.push_back(longVal);
	}
}

Structures::WheelsOdometrData InputFile::FormData(const std::vector<long>& data)
{
	Structures::WheelsOdometrData odmData = {};
	odmData.TimeStamp = data[0];
	odmData.ForwardLeftWheel = data[1];
	odmData.ForwardRightWheel = data[2];
	odmData.BackwardLeftWheel = data[3];
	odmData.BackwardRightWheel = data[4];

	return odmData;
}