#pragma once

#include "DataStructs.h"

#include <vector>
#include <nlohmann/json.hpp>

class OutputController {
public:
  struct Output {
    int numberOfSteps;
    std::vector<DataStructs::Point2D> coordinates;
    std::vector<float> directions;
    std::vector<int> states;
    std::vector<int> targetIndexes;
  };

  OutputController();

public:
  Output Outputs;

public:
  void AddData(DataStructs::Point2D coordinates, float direction, DataStructs::DroneState state, int targetIndex);
  void AddData(DataStructs::DroneOperationalData operationalData);
};

void to_json(nlohmann::json& j, const OutputController::Output& output);
