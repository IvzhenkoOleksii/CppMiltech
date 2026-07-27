#pragma once
#include "DataStructs.h"

struct DronePhysicalCommand {
  DataStructs::DroneState commandType;
  float AngleToRotate;
  float SpeedToReach;

  DronePhysicalCommand(DataStructs::DroneState type, float angle, float speed)
  {
    commandType = type;
    AngleToRotate = angle;
    SpeedToReach = speed;
  }
};