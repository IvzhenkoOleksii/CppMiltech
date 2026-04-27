#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include "TargetFile.h"
#include "DataStructs.h"

DataStructs::TargetData TargetFile::ReadJsonFile()
{
	std::ifstream file("DataFiles/json/Targets.json");
	DataStructs::TargetData targetData{};

    if (file.is_open())
    {
        nlohmann::json json = nlohmann::json::parse(file);
		json.at("paths").get_to(targetData.Positions);
    }
    else
    {
        std::cout << "Json Targets file read error!  " << std::endl;
        exit(1);
    }
    file.close();

	return targetData;
}
