#include "Armament/Solver/Table/ArmamentTableSolver.h"
#include <iostream>
#include <ostream>

ArmamentTableSolver::ArmamentTableSolver()
{
  tableController = {};
}

ArmamentTableSolver::~ArmamentTableSolver() {}

float ArmamentTableSolver::CalculateFallTime(const ArmamentDatabase::Data& armData, float droneAttackSpeed, float droneZPosition)
{
  int massIndex1 = tableController.GetNearestTableMassIndex(armData.Mass);
  int dragIndex1 = tableController.GetNearestTableDragIndex(armData.Drag);
  int liftIndex1 = tableController.GetNearestTableLiftIndex(armData.Lift);
  int velocityIndex1 = tableController.GetNearestTableVelocityIndex(droneAttackSpeed);
  int heightIndex1 = tableController.GetNearestTableHeightIndex(droneZPosition);

  TableSolver::Interp massIndex2 = tableController.FindInterpolatedIndex(armData.Mass, tableController.table.tableMass);
  TableSolver::Interp dragIndex2 = tableController.FindInterpolatedIndex(armData.Drag, tableController.table.tableDrag);
  TableSolver::Interp liftIndex2 = tableController.FindInterpolatedIndex(armData.Lift, tableController.table.tableLift);
  TableSolver::Interp velocityIndex2 = tableController.FindInterpolatedIndex(droneAttackSpeed, tableController.table.tableVelocity);
  TableSolver::Interp heightIndex2 = tableController.FindInterpolatedIndex(droneZPosition, tableController.table.tableHeight);

  TableResult v[16];
  for (int a = 0; a < 2; a++)
    for (int b = 0; b < 2; b++)
      for (int c = 0; c < 2; c++)
        for (int e = 0; e < 2; e++) {
          int resultIndex1 = tableController.GetResultsIndex(
            heightIndex2.index + a, velocityIndex2.index + b, massIndex2.index + c, dragIndex2.index + e, liftIndex2.index);
          TableResult lower = tableController.GetNearestResult(resultIndex1);

          int resultIndex2 = tableController.GetResultsIndex(
            heightIndex2.index + a, velocityIndex2.index + b, massIndex2.index + c, dragIndex2.index + e, liftIndex2.index + 1);
          TableResult upper = tableController.GetNearestResult(resultIndex2);
          v[a * 8 + b * 4 + c * 2 + e] = tableController.Lerp(lower, upper, liftIndex2.fraction);
        }

  // d: 16 → 8
  TableResult w[8];
  for (int a = 0; a < 2; a++)
    for (int b = 0; b < 2; b++)
      for (int c = 0; c < 2; c++)
        w[a * 4 + b * 2 + c] = tableController.Lerp(v[a * 8 + b * 4 + c * 2], v[a * 8 + b * 4 + c * 2 + 1], dragIndex2.fraction);

  // m: 8 → 4
  TableResult u[4];
  for (int a = 0; a < 2; a++)
    for (int b = 0; b < 2; b++)
      u[a * 2 + b] = tableController.Lerp(w[a * 4 + b * 2], w[a * 4 + b * 2 + 1], massIndex2.fraction);

  // V0: 4 → 2
  TableResult s[2];
  for (int a = 0; a < 2; a++)
    s[a] = tableController.Lerp(u[a * 2], u[a * 2 + 1], velocityIndex2.fraction);

  TableResult fallResult = tableController.Lerp(s[0], s[1], heightIndex2.fraction);

  TableResult v1[16];
  for (int a = 0; a < 2; a++)
    for (int b = 0; b < 2; b++)
      for (int c = 0; c < 2; c++)
        for (int e = 0; e < 2; e++) {
          int resultIndex1 =
            tableController.GetResultsIndex(heightIndex1 + a, velocityIndex1 + b, massIndex1 + c, dragIndex1 + e, liftIndex1);
          TableResult lower = tableController.GetNearestResult(resultIndex1);

          int resultIndex2 =
            tableController.GetResultsIndex(heightIndex1 + a, velocityIndex1 + b, massIndex1 + c, dragIndex1 + e, liftIndex1 + 1);
          TableResult upper = tableController.GetNearestResult(resultIndex2);
          v1[a * 8 + b * 4 + c * 2 + e] = tableController.Lerp(lower, upper, 0);
        }

  // d: 16 → 8
  TableResult w1[8];
  for (int a = 0; a < 2; a++)
    for (int b = 0; b < 2; b++)
      for (int c = 0; c < 2; c++)
        w1[a * 4 + b * 2 + c] = tableController.Lerp(v1[a * 8 + b * 4 + c * 2], v1[a * 8 + b * 4 + c * 2 + 1], 0);

  // m: 8 → 4
  TableResult u1[4];
  for (int a = 0; a < 2; a++)
    for (int b = 0; b < 2; b++)
      u1[a * 2 + b] = tableController.Lerp(w1[a * 4 + b * 2], w1[a * 4 + b * 2 + 1], 0);

  // V0: 4 → 2
  TableResult s1[2];
  for (int a = 0; a < 2; a++)
    s1[a] = tableController.Lerp(u1[a * 2], u1[a * 2 + 1], 0);

  TableResult fallResult1 = tableController.Lerp(s1[0], s1[1], 0);

  if (fallResult1.fallTime != fallResult.fallTime || fallResult1.fallDistance != fallResult.fallDistance) {
    std::cout << " WRONG WRONG WRONG  " << std::endl;
  }

  return fallResult.fallTime;
}

float ArmamentTableSolver::CalculateFallDistance(const ArmamentDatabase::Data& armData, float droneAttackSpeed, float fallTime)
{
  int fallTimeIndex = tableController.GetNearestTableFallTimeIndex(fallTime);
  float fallDistance = tableController.GetNearestFallDistance(fallTimeIndex);
  return fallDistance;
}  // namespace TableSolver
