#include "OutputController.h"
#include "DataStructs.h"

#include <nlohmann/json.hpp>

OutputController::OutputController()
{
  Outputs = {};
  Outputs.numberOfSteps = 0;
}

void OutputController::AddData(DataStructs::DroneOperationalData operationalData)
{
  DataStructs::Coord2D dronePosition2D = operationalData.transform.Position.GetPoint2D();
  Outputs.numberOfSteps++;

  OutputController::OutputStep step = {};
  step.pos = dronePosition2D;
  step.direction = operationalData.transform.Direction;
  step.state = operationalData.State;
  step.targetIdx = operationalData.CurrentTargetIndex;
  step.dropPoint = operationalData.DropPoint;
  step.aimPoint = operationalData.BombDropPoint;
  step.predictedTarget = operationalData.TargetedPosition;

  Outputs.steps.push_back(step);
}

void to_json(nlohmann::json& j, const OutputController::OutputStep& outputStep)
{
  j = nlohmann::json{outputStep.pos,
                     outputStep.direction,
                     outputStep.state,
                     outputStep.targetIdx,
                     outputStep.dropPoint,
                     outputStep.aimPoint,
                     outputStep.predictedTarget};
}

void to_json(nlohmann::json& j, const OutputController::Output& output)
{
  j = nlohmann::json{output.numberOfSteps, output.steps};
}