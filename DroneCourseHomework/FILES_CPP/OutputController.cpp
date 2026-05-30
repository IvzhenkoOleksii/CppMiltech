#include "OutputController.h"
#include "DataStructs.h"

#include <nlohmann/json.hpp>

OutputController::OutputController()
{
  Outputs = {};
  Outputs.numberOfSteps = 0;
}

void OutputController::AddData(DataStructs::Point2D coordinates, float direction, DataStructs::DroneState state, int targetIndex)
{
  Outputs.numberOfSteps++;
  Outputs.coordinates.push_back(coordinates);
  Outputs.directions.push_back(direction);
  Outputs.states.push_back(state);
  Outputs.targetIndexes.push_back(targetIndex);
}

void OutputController::AddData(DataStructs::DroneOperationalData operationalData)
{
  DataStructs::Point2D dronePosition2D = operationalData.transform.Position.GetPoint2D();
  Outputs.numberOfSteps++;
  Outputs.coordinates.push_back(dronePosition2D);
  Outputs.directions.push_back(operationalData.transform.Direction);
  Outputs.states.push_back(operationalData.State);
  Outputs.targetIndexes.push_back(operationalData.CurrentTargetIndex);
}

void to_json(nlohmann::json& j, const OutputController::Output& output)
{
  j = nlohmann::json{output.numberOfSteps, output.coordinates, output.directions, output.states, output.targetIndexes};
}