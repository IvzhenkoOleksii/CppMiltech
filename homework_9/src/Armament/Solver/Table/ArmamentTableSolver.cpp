#include "Armament/ArmamentDatabase.h"
#include "Armament/Solver/Table/ArmamentTableSolver.h"

ArmamentTableSolver::ArmamentTableSolver(const std::string& pathToTable)
{
  tableController = TableController(pathToTable);
}

ArmamentTableSolver::~ArmamentTableSolver() {}

ArmamentDatabase::FallResult ArmamentTableSolver::Calculate(const ArmamentDatabase::Data& armData,
                                                            float droneAttackSpeed,
                                                            float droneZPosition)
{
  TableSolver::Interp massIndex = tableController.GetNearestTableIndex(armData.Mass, tableController.table.tableMass);
  TableSolver::Interp dragIndex = tableController.GetNearestTableIndex(armData.Drag, tableController.table.tableDrag);
  TableSolver::Interp liftIndex = tableController.GetNearestTableIndex(armData.Lift, tableController.table.tableLift);
  TableSolver::Interp velocityIndex = tableController.GetNearestTableIndex(droneAttackSpeed, tableController.table.tableVelocity);
  TableSolver::Interp heightIndex = tableController.GetNearestTableIndex(droneZPosition, tableController.table.tableHeight);

  ArmamentDatabase::FallResult v[16];
  for (int a = 0; a < 2; a++)
    for (int b = 0; b < 2; b++)
      for (int c = 0; c < 2; c++)
        for (int e = 0; e < 2; e++) {
          int loverIndex = tableController.GetResultsIndex(
            heightIndex.index + a, velocityIndex.index + b, massIndex.index + c, dragIndex.index + e, liftIndex.index);
          ArmamentDatabase::FallResult lower = tableController.GetNearestResult(loverIndex);

          int upperIndex = tableController.GetResultsIndex(
            heightIndex.index + a, velocityIndex.index + b, massIndex.index + c, dragIndex.index + e, liftIndex.index + 1);
          ArmamentDatabase::FallResult upper = tableController.GetNearestResult(upperIndex);
          v[a * 8 + b * 4 + c * 2 + e] = tableController.Lerp(lower, upper, liftIndex.fraction);
        }

  // d: 16 → 8
  ArmamentDatabase::FallResult w[8];
  for (int a = 0; a < 2; a++)
    for (int b = 0; b < 2; b++)
      for (int c = 0; c < 2; c++)
        w[a * 4 + b * 2 + c] = tableController.Lerp(v[a * 8 + b * 4 + c * 2], v[a * 8 + b * 4 + c * 2 + 1], dragIndex.fraction);

  // m: 8 → 4
  ArmamentDatabase::FallResult u[4];
  for (int a = 0; a < 2; a++)
    for (int b = 0; b < 2; b++)
      u[a * 2 + b] = tableController.Lerp(w[a * 4 + b * 2], w[a * 4 + b * 2 + 1], massIndex.fraction);

  // V0: 4 → 2
  ArmamentDatabase::FallResult s[2];
  for (int a = 0; a < 2; a++)
    s[a] = tableController.Lerp(u[a * 2], u[a * 2 + 1], velocityIndex.fraction);

  ArmamentDatabase::FallResult result = tableController.Lerp(s[0], s[1], heightIndex.fraction);
  return result;
}
