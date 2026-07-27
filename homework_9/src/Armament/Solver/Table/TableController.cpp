#include "Armament/Solver/Table/TableController.h"
#include "Armament/ArmamentDatabase.h"

#include <cstddef>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

TableSolver::TableController::TableController(std::string pathToTable)
{
  std::ifstream file(pathToTable);

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
      file >> tableResults[i].Time >> tableResults[i].Distance;
    }
  }
  else {
    std::cerr << "Ballistic Table file read error!  " << std::endl;
    exit(1);
  }
  file.close();
}

ArmamentDatabase::FallResult TableSolver::TableController::GetNearestResult(int index)
{
  if (index < tableResults.size()) {
    return tableResults[index];
  }
  else {
    std::cerr << "Searching fall result index bigger that tableResults.size !" << std::endl;
    std::exit(1);
  }
}

TableSolver::Interp TableSolver::TableController::GetNearestTableIndex(float value, const std::vector<float>& table)
{
  if (value <= table[0]) {
    return {0, 0};
  }

  size_t tableSize = table.size();
  if (value >= table[tableSize - 1]) {
    int prevToSizeIndex = (int)(tableSize - 2);
    return {prevToSizeIndex, 1};
  }

  for (size_t i = 0; i < tableSize; ++i) {
    float tableValue = table[i];

    if (value < tableValue) {
      int diffIndex = i - 1;
      float prevValue = table[diffIndex];
      float length = tableValue - prevValue;
      float difference = value - prevValue;
      float diffFraction = difference / length;
      return {diffIndex, diffFraction};
    }
  }

  // must be unreachable
  std::cerr << "This is unreachable part!" << std::endl;
  return {-1, -1};
}

int TableSolver::TableController::GetResultsIndex(int heightIndex, int velocityIndex, int massIndex, int dragIndex, int liftIndex)
{
  size_t heightSize_t = (size_t)heightIndex;
  size_t velocityTableSize = table.tableVelocity.size();
  size_t massTableSize = table.tableMass.size();
  size_t dragTableSize = table.tableDrag.size();
  size_t liftTableSize = table.tableLift.size();

  size_t firstCoeff = heightSize_t * velocityTableSize + velocityIndex;
  size_t secondCoeff = firstCoeff * massTableSize + massIndex;
  size_t thirdCoeff = secondCoeff * dragTableSize + dragIndex;
  size_t fourthCoeff = thirdCoeff * liftTableSize + liftIndex;

  return fourthCoeff;
}

ArmamentDatabase::FallResult TableSolver::TableController::Lerp(const ArmamentDatabase::FallResult& first,
                                                                const ArmamentDatabase::FallResult& second,
                                                                float coeff)
{
  float time = first.Time + (second.Time - first.Time) * coeff;
  float distance = first.Distance + (second.Distance - first.Distance) * coeff;

  ArmamentDatabase::FallResult result;
  result.Time = time;
  result.Distance = distance;

  return result;
}