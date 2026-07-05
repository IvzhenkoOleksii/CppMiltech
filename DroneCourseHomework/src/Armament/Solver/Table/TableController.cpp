#include "Armament/Solver/Table/TableController.h"
#include "Armament/ArmamentDatabase.h"

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
  int diffIndex = 0;
  float diffFraction = 0;
  float minDiffence;
  for (size_t i = 0; i < table.size(); ++i) {
    float tableValue = table[i];
    float difference = std::fabs(value - tableValue);

    if (i == 0) {
      diffIndex = i;
      minDiffence = difference;
      continue;
    }

    if (difference < minDiffence) {
      diffIndex = i;
      minDiffence = difference;
    }
    else if (difference == minDiffence) {
      diffFraction = 0.5;
    }
  }

  return {diffIndex, diffFraction};
}

int TableSolver::TableController::GetResultsIndex(int heightIndex, int velocityIndex, int massIndex, int dragIndex, int liftIndex)
{
  return ((((size_t)heightIndex * table.tableVelocity.size() + velocityIndex) * table.tableMass.size() + massIndex) *
            table.tableDrag.size() +
          dragIndex) *
           table.tableLift.size() +
         liftIndex;
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