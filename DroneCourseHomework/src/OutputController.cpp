#include "OutputController.h"
#include "DataStructs.h"

#include <nlohmann/json.hpp>

OutputController::OutputController()
{
  Outputs = {};
  Outputs.numberOfSteps = 0;
}

void OutputController::AddData(const DataStructs::DroneFullData& data)
{
  DataStructs::Coord2D dronePosition2D = DataStructs::Coord3D::GetPoint2D(data.physicalState.Position);
  Outputs.numberOfSteps++;

  OutputController::OutputStep step = {};
  step.pos = dronePosition2D;
  step.direction = data.physicalState.Direction;
  step.state = data.operationalState.State;
  step.targetIdx = data.operationalState.CurrentTargetIndex;
  step.dropPoint = data.operationalState.DropPoint;
  step.aimPoint = data.operationalState.AimPoint;
  step.predictedTarget = data.operationalState.TargetedPosition;

  Outputs.steps.push_back(step);
}

void to_json(nlohmann::json& j, const OutputController::OutputStep& outputStep)
{
  nlohmann::json j_step;

  j_step["position"] = {{"x", outputStep.pos.X}, {"y", outputStep.pos.Y}};
  j_step["direction"] = outputStep.direction;
  j_step["state"] = outputStep.state;
  j_step["targetIdx"] = outputStep.targetIdx;
  j_step["dropPoint"] = {{"x", outputStep.dropPoint.X}, {"y", outputStep.dropPoint.Y}};
  j_step["aimPoint"] = {{"x", outputStep.aimPoint.X}, {"y", outputStep.aimPoint.Y}};
  j_step["predictedTarget"] = {{"x", outputStep.predictedTarget.X}, {"y", outputStep.predictedTarget.Y}};

  j = j_step;
}

void to_json(nlohmann::json& j, const OutputController::Output& output)
{
  nlohmann::json j_steps;
  j_steps["steps"] = output.steps;
  j = nlohmann::json{j_steps};
}