#include "Armament/Solver/Table/TableController.h"
#include <asm-generic/errno.h>

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <cmath>

TableSolver::TableController::TableController()
{
  std::ifstream file("./DroneCourseHomework/DataFiles/txt/BallisticTable.txt");

  if (file.is_open()) {
    table = {};
    tableResults = {};

    int nZ, nV, nM, nD, nL;
    file >> nZ >> nV >> nM >> nD >> nL;

    table.tableHeight.resize(nZ);
    for (auto& v : table.tableHeight)
      file >> v;
    table.tableVelocity.resize(nV);
    for (auto& v : table.tableVelocity)
      file >> v;
    table.tableMass.resize(nM);
    for (auto& v : table.tableMass)
      file >> v;
    table.tableDrag.resize(nD);
    for (auto& v : table.tableDrag)
      file >> v;
    table.tableLift.resize(nL);
    for (auto& v : table.tableLift)
      file >> v;

    size_t total = (size_t)nZ * nV * nM * nD * nL;
    tableResults.resize(total);

    // Порядок: Z0 → V0 → m → d → l (зовнішній → внутрішній)
    for (size_t i = 0; i < total; i++) {
      file >> tableResults[i].fallTime >> tableResults[i].fallDistance;
    }
  }
  else {
    std::cerr << "Ballistic Table file read error!  " << std::endl;
    exit(1);
  }
  file.close();
}

float TableSolver::TableController::GetNearestFallTime(int massIndex, int dragIndex, int liftIndex, int velocityIndex, int heightIndex)
{
  int index = massIndex * dragIndex * liftIndex * velocityIndex * heightIndex;
  if (index < tableResults.size()) {
    return tableResults[index].fallTime;
  }
  else {
    std::cerr << "Searching fall time index bigger that tableResults.size !" << std::endl;
    std::exit(1);
  }
}

float TableSolver::TableController::GetNearestFallDistance(int massIndex, int dragIndex, int liftIndex, int velocityIndex, int heightIndex)
{
  int index = massIndex * dragIndex * liftIndex * velocityIndex * heightIndex;
  if (index < tableResults.size()) {
    return tableResults[index].fallDistance;
  }
  else {
    std::cerr << "Searching fall distance index bigger that tableResults.size !" << std::endl;
    std::exit(1);
  }
}

int TableSolver::TableController::GetNearestTableMassIndex(const float& realMass)
{
  return GetNearestTableIndex(realMass, table.tableMass);
}

int TableSolver::TableController::GetNearestTableDragIndex(const float& realDrag)
{
  return GetNearestTableIndex(realDrag, table.tableDrag);
}

int TableSolver::TableController::GetNearestTableLiftIndex(const float& realLift)
{
  return GetNearestTableIndex(realLift, table.tableLift);
}

int TableSolver::TableController::GetNearestTableHeightIndex(const float& realHeight)
{
  return GetNearestTableIndex(realHeight, table.tableHeight);
}

int TableSolver::TableController::GetNearestTableVelocityIndex(const float& realVelocity)
{
  return GetNearestTableIndex(realVelocity, table.tableVelocity);
}

int TableSolver::TableController::GetNearestTableFallTimeIndex(const float& realFallTime)
{
  int minDiffIndex;
  float minDiffence;
  for (size_t i = 0; i < tableResults.size(); ++i) {
    float tableValue = tableResults[i].fallTime;
    float difference = std::fabs(realFallTime - tableValue);

    if (i == 0) {
      minDiffIndex = i;
      minDiffence = difference;
      continue;
    }

    if (difference < minDiffence) {
      minDiffIndex = i;
      minDiffence = difference;
    }
  }

  return minDiffIndex;
}

int TableSolver::TableController::GetNearestTableIndex(float value, const std::vector<float>& table)
{
  int minDiffIndex;
  float minDiffence;
  for (size_t i = 0; i < table.size(); ++i) {
    float tableValue = table[i];
    float difference = std::fabs(value - tableValue);

    if (i == 0) {
      minDiffIndex = i;
      minDiffence = difference;
      continue;
    }

    if (difference < minDiffence) {
      minDiffIndex = i;
      minDiffence = difference;
    }
  }

  // array index starts from zero, but we cannot multiply on zero - we multiply on 1
  return minDiffIndex + 1;
}