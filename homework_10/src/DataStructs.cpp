#include "DataStructs.h"
#include "MathCalculator.h"

#include <iostream>
#include <nlohmann/json.hpp>

// DroneData struct methods
DataStructs::Coord2D DataStructs::Coord2D::operator/(const DataStructs::Coord2D& other) const
{
  DataStructs::Coord2D result;
  result.X = X / other.X;
  result.Y = Y / other.Y;
  return result;
}

bool DataStructs::Coord2D::operator==(const DataStructs::Coord2D& other) const
{
  if (!MathCalculator::AreEqual(X, other.X)) {
    return false;
  }

  if (!MathCalculator::AreEqual(Y, other.Y)) {
    return false;
  }

  return true;
}

void DataStructs::Coord2D::Debug()
{
  std::cout << "    x:   " << X << "   y:   " << Y << std::endl;
}

void DataStructs::Coord3D::DeInitialize()
{
  // we never work so high -> so it`s a wrong value
  Z = 99999;
}

bool DataStructs::Coord3D::IsInitialized()
{
  return Z < 99999;
}

DataStructs::Coord2D DataStructs::Coord3D::GetPoint2D(const Coord3D& original)
{
  Coord2D pos;
  pos.X = original.X;
  pos.Y = original.Y;

  return pos;
}

DataStructs::Coord3D DataStructs::Coord3D::operator/(const DataStructs::Coord3D& other) const
{
  DataStructs::Coord3D result;
  result.X = X / other.X;
  result.Y = Y / other.Y;
  result.Z = Z / other.Z;

  return result;
}

bool DataStructs::Coord3D::operator==(const DataStructs::Coord3D& other) const
{
  if (!MathCalculator::AreEqual(X, other.X)) {
    return false;
  }

  if (!MathCalculator::AreEqual(Y, other.Y)) {
    return false;
  }

  if (!MathCalculator::AreEqual(Z, other.Z)) {
    return false;
  }

  return true;
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

  if (SimStepTime <= 0) {
    std::cout << "DroneControllerStepTime <= 0" << "\n";
    exit(1);
  }

  if (PhysicsStepTime <= 0) {
    std::cout << "DronePhysicsStepTime <= 0" << "\n";
    exit(1);
  }

  if (TargetStepTime <= 0) {
    std::cout << "TargetStepTime <= 0" << "\n";
    exit(1);
  }

  if (TimeScale <= 0) {
    std::cout << "TimeScale <= 0" << "\n";
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

bool DataStructs::DroneOperationalState::IsTargetSelected()
{
  return CurrentTargetIndex != -1;
}

void DataStructs::DroneOperationalState::DeselectTarget()
{
  CurrentTargetIndex = -1;
}

// free methods for json support
void to_json(nlohmann::json& j, const DataStructs::Coord2D& position)
{
  j = nlohmann::json{position.X, position.Y};
}

void from_json(const nlohmann::json& j, DataStructs::Coord2D& position)
{
  if (j.contains("x") && j.contains("y")) {
    j.at("x").get_to(position.X);
    j.at("y").get_to(position.Y);
    return;
  }

  if (j.contains("X") && j.contains("Y")) {
    j.at("X").get_to(position.X);
    j.at("Y").get_to(position.Y);
    return;
  }

  j.at(0).get_to(position.X);
  j.at(1).get_to(position.Y);
}

void to_json(nlohmann::json& j, const DataStructs::Coord3D& position)
{
  j = nlohmann::json{position.X, position.Y, position.Z};
}

void from_json(const nlohmann::json& j, DataStructs::Coord3D& position)
{
  j.at(0).get_to(position.X);
  j.at(1).get_to(position.Y);
  j.at(2).get_to(position.Z);
}

void from_json(const nlohmann::json& j, DataStructs::DroneJsonState& droneState)
{
  j.at("altitude").get_to(droneState.altitude);
  j.at("initialDirection").get_to(droneState.initialDirection);
  j.at("attackSpeed").get_to(droneState.attackSpeed);
  j.at("accelerationPath").get_to(droneState.accelerationPath);
  j.at("angularSpeed").get_to(droneState.angularSpeed);
  j.at("turnThreshold").get_to(droneState.turnThreshold);
  j.at("position").get_to(droneState.position);
}

void from_json(const nlohmann::json& j, DataStructs::SimulationJsonState& simState)
{
  j.at("timeStep").get_to(simState.timeStep);
  j.at("hitRadius").get_to(simState.hitRadius);
  j.at("physicsTimeStep").get_to(simState.physicsTimeStep);
  j.at("timeScale").get_to(simState.timeScale);
  j.at("targetTimeStep").get_to(simState.targetTimeStep);
}