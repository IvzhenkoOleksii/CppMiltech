#include "Armament/Solver/Table/ArmamentTableSolver.h"

ArmamentTableSolver::ArmamentTableSolver()
{
  tableController = {};
}

ArmamentTableSolver::~ArmamentTableSolver() {}

float ArmamentTableSolver::CalculateFallTime(const ArmamentDatabase::Data& armData, float droneAttackSpeed, float droneZPosition)
{
  int massIndex = tableController.GetNearestTableMassIndex(armData.Mass);
  int dragIndex = tableController.GetNearestTableDragIndex(armData.Drag);
  int liftIndex = tableController.GetNearestTableLiftIndex(armData.Lift);
  int velocityIndex = tableController.GetNearestTableVelocityIndex(droneAttackSpeed);
  int heightIndex = tableController.GetNearestTableHeightIndex(droneZPosition);

  return tableController.GetNearestFallDistance(massIndex, dragIndex, liftIndex, velocityIndex, heightIndex);
}

float ArmamentTableSolver::CalculateFallDistance(const ArmamentDatabase::Data& armData, float droneAttackSpeed, float fallTime)
{
  int fallTimeIndex = tableController.GetNearestTableFallTimeIndex(fallTime);
  return tableController.GetNearestFallDistance(fallTimeIndex, 1, 1, 1, 1);
}  // namespace TableSolver
