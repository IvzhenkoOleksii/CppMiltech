#include "DataStructs.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <ostream>

// DroneData struct methods
void DataStructs::Point2D::Debug()
{
  std::cout << "    x:   " << X << "   y:   " << Y << std::endl;
}

void DataStructs::Point3D::DeInitialize()
{
  // we never work too high -> so it`s a wrong value
  Z = 99999;
}

bool DataStructs::Point3D::IsInitialized()
{
  return Z < 99999;
}

DataStructs::Point2D DataStructs::Point3D::GetPoint2D()
{
  Point2D pos;
  pos.X = X;
  pos.Y = Y;

  return pos;
}

void DataStructs::DroneInputData::CheckData()
{
  if (Position.Z <= 0) {
    std::cout << "Wrong Drone input position. Height cannot be zero or less" << "\n";
    exit(1);
  }

  if (AttackSpeed <= 0) {
    std::cout << "Wrong Input Attack Speed" << "\n";
    exit(1);
  }

  if (AccelerationPath <= 0) {
    std::cout << "Wrong Input Acceleration path" << "\n";
    exit(1);
  }

  if (AmmoType.empty()) {
    std::cout << "Empty ammo type" << "\n";
    exit(1);
  }

  if (AngularSpeed <= 0) {
    std::cout << "AngularSpeed <= 0" << "\n";
    exit(1);
  }

  if (TurnThreshold <= 0) {
    std::cout << "TurnThreshold <= 0" << "\n";
    exit(1);
  }
}

// InputData struct methods
void DataStructs::InputData::CheckData()
{
  DroneData.CheckData();

  if (SimTestStep <= 0) {
    std::cout << "SimTestStep <= 0" << "\n";
    exit(1);
  }

  if (ArrayTimeStep <= 0) {
    std::cout << "ArrayTimeStep <= 0" << "\n";
    exit(1);
  }

  if (HitRadius <= 0) {
    std::cout << "HitRadius <= 0" << "\n";
    exit(1);
  }
}

bool DataStructs::DroneOperationalData::IsTargetSelected()
{
  return CurrentTargetIndex != -1;
}

void DataStructs::DroneOperationalData::DeselectTarget()
{
  CurrentTargetIndex = -1;
}

// free methods for json support
void to_json(nlohmann::json& j, const DataStructs::Point2D& position)
{
  j = nlohmann::json{position.X, position.Y};
}

void from_json(const nlohmann::json& j, DataStructs::Point2D& position)
{
  j.at(0).get_to(position.X);
  j.at(1).get_to(position.Y);
}

void to_json(nlohmann::json& j, const DataStructs::Point3D& position)
{
  j = nlohmann::json{position.X, position.Y, position.Z};
}

void from_json(const nlohmann::json& j, DataStructs::Point3D& position)
{
  j.at(0).get_to(position.X);
  j.at(1).get_to(position.Y);
  j.at(2).get_to(position.Z);
}