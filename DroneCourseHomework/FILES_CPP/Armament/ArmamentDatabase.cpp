#include "Armament/ArmamentDatabase.h"
#include <iostream>

// key - AmmoType
// value - mass, kg; drag, percents; lift; gliding type
std::map<std::string, ArmamentDatabase::Data> ArmamentDatabase::sArmaData = {{"VOG-17", ArmamentDatabase::Data(0.35f, 0.07f, 0.0f, 0)},
                                                                             {"M67", ArmamentDatabase::Data(0.6f, 0.10f, 0.0f, 0)},
                                                                             {"RKG-3", ArmamentDatabase::Data(1.2f, 0.10f, 0.0f, 0)},
                                                                             {"GLIDING-VOG", ArmamentDatabase::Data(0.45f, 0.10f, 1.0f, 1)},
                                                                             {"GLIDING-RKG", ArmamentDatabase::Data(1.4f, 0.10f, 1.0f, 1)}};

ArmamentDatabase::Data ArmamentDatabase::GetArmament(std::string name)
{
  auto search = sArmaData.find(name);
  if (search == sArmaData.end()) {
    std::cout << "Error!. There is no data about: " << name << std::endl;
    exit(1);
  }
  else {
    ArmamentDatabase::Data searchedData = search->second;
    std::cout << "Data about " << name << "  mass: " << searchedData.Mass << "  drag: " << searchedData.Drag
              << "  lift: " << searchedData.Lift << std::endl;
    return searchedData;
  }
}