#include "Armament/ArmamentController.h"
#include "DataStructs.h"

#include <string>
#include <cmath>
#include <iostream>

bool ArmamentController::GetIsFired()
{
  return isFired.load();
}

void ArmamentController::UpdateFireFlag(bool value)
{
  isFired.store(value);
}

// these next 2 values are constant, no need std::atomic or mutex
float ArmamentController::GetFallDistance()
{
  return fallResult.Distance;
}

float ArmamentController::GetFallTime()
{
  return fallResult.Time;
}

float ArmamentController::GetHitRadius()
{
  return hitRadius;
}

DataStructs::Coord3D ArmamentController::GetBombPosition()
{
  return position;
}

ArmamentController::ArmamentController(std::string ammoType,
                                       std::string filePath,
                                       float droneAttackSpeed,
                                       float droneHeight,
                                       float hitRadius,
                                       std::unique_ptr<IArmamentSolver> solver)
{
  UpdateFireFlag(false);

  this->hitRadius = hitRadius;
  this->solver = std::move(solver);

  armData = ArmamentDatabase::GetArmament(ammoType, filePath);

  armamentFallHeight = droneHeight;
  fallResult = this->solver->Calculate(armData, droneAttackSpeed, armamentFallHeight);
  CalculateSimulationData();
}

// this is doing one time at start
void ArmamentController::CalculateSimulationData()
{
  numberOfFallSteps = fallResult.Time / stepTime;
  currentFallStepIndex = numberOfFallSteps;
  fallStepHeight = armamentFallHeight / numberOfFallSteps;
  fallStepDistance = fallResult.Distance / numberOfFallSteps;
}

float ArmamentController::CalculateBombFallDistance(DataStructs::Coord3D startPoint, float speed)
{
  ArmamentDatabase::FallResult result = this->solver->Calculate(armData, speed, startPoint.Z);
  return result.Distance;
}

void ArmamentController::DropBomb(DataStructs::Coord3D startPosition, float direction)
{
  position = startPosition;
  fallDirection = direction;

  UpdateFireFlag(true);
  StartLoopThread();
}

void ArmamentController::UpdateFallPosition()
{
  position.Z -= fallStepHeight;

  float xStep = fallStepDistance * cosf(fallDirection);
  float yStep = fallStepDistance * sinf(fallDirection);

  position.X += xStep;
  position.Y += yStep;
}

void ArmamentController::UpdateFallPositionPartially()
{
  position.Z -= fallStepHeight * currentFallStepIndex;

  float xStep = fallStepDistance * currentFallStepIndex * cosf(fallDirection);
  float yStep = fallStepDistance * currentFallStepIndex * sinf(fallDirection);

  position.X += xStep;
  position.Y += yStep;
}

void ArmamentController::OnLoopStepStart() {}

void ArmamentController::OnLoopStepEnd()
{
  if (currentFallStepIndex > 1) {
    currentFallStepIndex--;
    UpdateFallPosition();
  }
  else if (currentFallStepIndex < 1 && currentFallStepIndex > 0) {
    UpdateFallPositionPartially();
    currentFallStepIndex = 0;
  }
  else {
    std::cout << "Bomb exploded. Position X:   " << position.X << "   Y:   " << position.Y << "   Z:  " << position.Z << std::endl;
    FinishLoopThread();
  }
}

void ArmamentController::OnAfterStepEndAction() {}
