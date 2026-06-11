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

float TableSolver::TableController::GetNearestFallTime(int index)
{
  if (index < tableResults.size()) {
    return tableResults[index].fallTime;
  }
  else {
    std::cerr << "Searching fall time index bigger that tableResults.size !" << std::endl;
    std::exit(1);
  }
}

float TableSolver::TableController::GetNearestFallDistance(int index)
{
  if (index < tableResults.size()) {
    return tableResults[index].fallDistance;
  }
  else {
    std::cerr << "Searching fall distance index bigger that tableResults.size !" << std::endl;
    std::exit(1);
  }
}

TableSolver::TableResult TableSolver::TableController::GetNearestResult(int index)
{
  if (index < tableResults.size()) {
    return tableResults[index];
  }
  else {
    std::cerr << "Searching fall result index bigger that tableResults.size !" << std::endl;
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

    if (difference <= minDiffence) {
      minDiffIndex = i;
      minDiffence = difference;
    }
  }

  return minDiffIndex;
}

int TableSolver::TableController::GetResultsIndex(int heightIndex, int velocityIndex, int massIndex, int dragIndex, int liftIndex)
{
  return ((((size_t)heightIndex * table.tableVelocity.size() + velocityIndex) * table.tableMass.size() + massIndex) *
            table.tableDrag.size() +
          dragIndex) *
           table.tableLift.size() +
         liftIndex;
}

int TableSolver::TableController::GetResultsIndex(float heightIndex, float velocityIndex, float massIndex, float dragIndex, float liftIndex)
{
  return ((((size_t)heightIndex * table.tableVelocity.size() + velocityIndex) * table.tableMass.size() + massIndex) *
            table.tableDrag.size() +
          dragIndex) *
           table.tableLift.size() +
         liftIndex;
}

TableSolver::Interp TableSolver::TableController::FindInterpolatedIndex(float value, const std::vector<float>& axis)
{
  if (value <= axis.front())
    return {0, 0.0f};
  if (value >= axis.back())
    return {(int)axis.size() - 2, 1.0f};

  auto it = std::lower_bound(axis.begin(), axis.end(), value);
  int i = (int)(it - axis.begin()) - 1;
  if (i < 0)
    i = 0;

  float frac = (value - axis[i]) / (axis[i + 1] - axis[i]);
  return {i, frac};
}

TableSolver::TableResult TableSolver::TableController::Lerp(const TableResult& first, const TableResult& second, float coeff)
{
  float time = first.fallTime + (second.fallTime - first.fallTime) * coeff;
  float distance = first.fallDistance + (second.fallDistance - first.fallDistance) * coeff;

  TableResult result;
  result.fallTime = time;
  result.fallDistance = distance;

  return result;
}