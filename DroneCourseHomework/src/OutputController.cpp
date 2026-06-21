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