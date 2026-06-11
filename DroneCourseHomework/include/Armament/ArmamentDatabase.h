#pragma once
#include <string>
#include <map>
#include <cstdint>

class ArmamentDatabase {
public:
  struct Data {
  public:
    float Mass;
    float Drag;
    float Lift;
    uint8_t Type;

    Data()
    {
      Mass = 0;
      Drag = 0;
      Lift = 0;
      Type = 0;
    }

    Data(float mass, float drag, float lift, uint8_t type)
    {
      Mass = mass;
      Drag = drag;
      Lift = lift;
      Type = type;
    }
  };

public:
  struct FallResult {
    float Time;      // time to fall
    float Distance;  // distance to fall

    FallResult()
    {
      Time = 0;
      Distance = 0;
    }

    FallResult(float time, float distance)
    {
      Time = time;
      Distance = distance;
    }
  };

public:
  static Data GetArmament(const std::string& name);

private:
  // data map, use only as getter
  static std::map<std::string, Data> sArmaData;
  static void ReadAmmoFile();
};
