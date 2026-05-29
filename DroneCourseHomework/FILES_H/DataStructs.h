#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

class DataStructs {
public:
  enum DroneState { STOPPED, ACCELERATING, DECELERATING, TURNING, MOVING };

public:
  struct Point2D {
    float X = 0.0f;
    float Y = 0.0f;

    Point2D() = default;
    Point2D(float x, float y)
      : X(x)
      , Y(y)
    {
    }
  };

public:
  struct Point3D {
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;

    Point3D() = default;
    Point3D(float x, float y, float z)
      : X(x)
      , Y(y)
      , Z(z)
    {
    }

  public:
    Point2D GetPoint2D();

  public:
    void DeInitialize();

  public:
    bool IsInitialized();
  };

public:
  struct DroneInputData {
    // fields
    Point3D Position = {0.0f, 0.0f, 0.0f};
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
    std::vector<std::vector<DataStructs::Point2D>> Positions;
  };

public:
  struct TargetOperationalData {
    float DistanceToTarget;
    float AngleToTarget;
    Point3D ManeuverPoint;
    Point3D FirePoint;

    float TimeToRotate;
    float TimeToMove;
    // sum of TimeToRotate + TimeToMove
    float TimeToReach = TimeToRotate + TimeToMove;
  };

public:
  struct DronePhysicalState {
    float Direction;
    float Velocity;
    Point3D Position;
  };

public:
  struct DroneOperationalData {
    DroneState State;
    int CurrentTargetIndex = -1;
    DronePhysicalState transform;
  };
};

void to_json(nlohmann::json& j, const DataStructs::Point2D& position);
void from_json(const nlohmann::json& j, DataStructs::Point2D& position);

void to_json(nlohmann::json& j, const DataStructs::Point3D& position);
void from_json(const nlohmann::json& j, DataStructs::Point3D& position);