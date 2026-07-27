#include "OutputController.h"
#include "DataStructs.h"
#include "MathCalculator.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <ostream>

OutputController::OutputController()
{
  Outputs = {};
  Outputs.numberOfSteps = 0;
}

void OutputController::AddData(const DataStructs::DroneFullData& data)
{
  DataStructs::Coord2D dronePosition2D = DataStructs::Coord3D::GetPoint2D(data.physicalState.Position);

  OutputController::OutputStep step = {};
  step.pos = dronePosition2D;
  step.direction = data.physicalState.Direction;
  step.state = data.physicalState.DroneStateType;
  step.timeSecSinceStart = data.physicalState.TimeSecSinceStart;
  step.physicalTickCounter = data.physicalState.tickCounter;
  //
  step.targetIdx = data.operationalState.CurrentTargetIndex;
  step.dropPoint = data.operationalState.DropPoint;
  step.aimPoint = data.operationalState.AimPoint;
  step.predictedTarget = data.operationalState.TargetedPosition;
  //
  step.stepIndex = Outputs.numberOfSteps;

  if (Outputs.numberOfSteps > 1) {
    OutputStep prevStep = Outputs.steps[Outputs.numberOfSteps - 1];
    float deltaDistance = MathCalculator::DistanceBetweenPoints(step.pos, prevStep.pos);
    float deltaTime = step.timeSecSinceStart - prevStep.timeSecSinceStart;
    float velocityDelta = deltaDistance / deltaTime;
    int phSteps = step.physicalTickCounter - prevStep.physicalTickCounter;

    step.velocityDelta = velocityDelta - prevStep.velocityDelta;

    // std::cout << "  CURRENT VELOCITY DELTA:  " << velocityDelta << "    at deltaTime:  " << deltaTime << "    ph steps:  " << phSteps
    //           << "    dist:  " << deltaDistance << std::endl
    //           << "  STEP INDEX:  " << Outputs.numberOfSteps << "    VEL DELTA BETWEEN STEPS:  " << step.velocityDelta
    //           << "    physical velocity:  " << data.physicalState.Velocity << std::endl
    //           << "    saved velocity:  " <<
    //           << std::endl;
  }

  Outputs.numberOfSteps++;
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
  j_step["timeSecSinceStart"] = outputStep.timeSecSinceStart;
  j_step["stepIndex"] = outputStep.stepIndex;
  j_step["phTickCounter"] = outputStep.physicalTickCounter;

  j = j_step;
}

void to_json(nlohmann::json& j, const OutputController::Output& output)
{
  nlohmann::json j_steps;
  j_steps["steps"] = output.steps;
  j = nlohmann::json{j_steps};
}