#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

class DataStructs {
public:
  enum DroneState { STOPPED, ACCELERATING, DECELERATING, TURNING, MOVING };

public:
  struct Coord2D {
    float X = 0.0f;
    float Y = 0.0f;

    Coord2D() = default;
    Coord2D(float x, float y)
      : X(x)
      , Y(y)
    {
    }

  public:
    void Debug();

    Coord2D operator/(const Coord2D& other) const;
    bool operator==(const Coord2D& other) const;
  };

public:
  struct Coord3D {
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;

    Coord3D() = default;
    Coord3D(float x, float y, float z)
      : X(x)
      , Y(y)
      , Z(z)
    {
    }

  public:
    Coord2D GetPoint2D();

  public:
    void DeInitialize();

  public:
    bool IsInitialized();

    Coord3D operator/(const Coord3D& other) const;
    bool operator==(const Coord3D& other) const;
  };

public:
  struct DroneInputData {
    // fields
    Coord3D Position = {0.0f, 0.0f, 0.0f};
    std::string AmmoType;
    float InitialDirection = {-1.0f};
    float AttackSpeed = {0.0f};
    float AccelerationPath = {0.0f};
    float AngularSpeed = {0.0f};
    float TurnThreshold = {0.0f};

    // constructor
    DroneInputData() = default;

    // methods
  public:
    void CheckData();
    //	public:void SetPosition(float X, float Y, float Z);
  };

public:
  struct InputData {
    // fields
    DroneInputData DroneData{};
    float SimTestStep = {0.0f};
    float ArrayTimeStep = {0.0f};
    float HitRadius = {0.0f};

    // constructor
    InputData() = default;

    // methods
  public:
    void CheckData();
  };

public:
  struct TargetData {
    std::vector<std::vector<DataStructs::Coord2D>> Positions;
  };

public:
  struct DronePhysicalState {
    float Direction;
    float Velocity;
    Coord3D Position;
  };

public:
  struct DroneOperationalData {
    DroneState State;
    DronePhysicalState transform;
    int CurrentTargetIndex = -1;
    Coord2D TargetedPosition;

  public:
    bool IsTargetSelected();
    void DeselectTarget();
  };
};

void to_json(nlohmann::json& j, const DataStructs::Coord2D& position);
void from_json(const nlohmann::json& j, DataStructs::Coord2D& position);

void to_json(nlohmann::json& j, const DataStructs::Coord3D& position);
void from_json(const nlohmann::json& j, DataStructs::Coord3D& position);