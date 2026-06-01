#pragma once
#include <string>
#include <map>
#include <cstdint>

// this class represents data-table
// I made it as a private dictionary, so it will be
// like sql file illusion or json or other data type
// Type will be like enum, so I took small type:
// 0 - free fall type
// 1 - glider type
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
  static Data GetArmament(const std::string& name);

private:
  // data map, use only as getter
  static std::map<std::string, Data> sArmaData;
  static void ReadAmmoFile();
};
