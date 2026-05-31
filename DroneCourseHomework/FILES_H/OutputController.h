#pragma once

#include "DataStructs.h"

#include <vector>
#include <nlohmann/json.hpp>

class OutputController {
public:
  struct OutputStep {
    DataStructs::Coord2D pos;
    float direction;
    int state;
    int targetIdx;
    DataStructs::Coord2D dropPoint;
    DataStructs::Coord2D aimPoint;
    DataStructs::Coord2D predictedTarget;
  };

  struct Output {
    int numberOfSteps;
    std::vector<OutputStep> steps;
  };

  OutputController();

public:
  Output Outputs;

public:
  void AddData(DataStructs::DroneOperationalData operationalData);
};

void to_json(nlohmann::json& j, const OutputController::OutputStep& outputStep);
void to_json(nlohmann::json& j, const OutputController::Output& output);
