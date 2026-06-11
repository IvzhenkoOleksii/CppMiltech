#include <vector>
#include "Armament/Solver/Table/Table.h"

namespace TableSolver {

class TableController {
public:
  TableController();

public:
  Table table;
  int GetNearestTableMassIndex(const float& realMass);
  int GetNearestTableDragIndex(const float& realDrag);
  int GetNearestTableLiftIndex(const float& realLift);
  int GetNearestTableHeightIndex(const float& realHeight);
  int GetNearestTableVelocityIndex(const float& realVelocity);
  int GetNearestTableFallTimeIndex(const float& realFallTime);

  TableResult Lerp(const TableResult& first, const TableResult& second, float coeff);
  Interp FindInterpolatedIndex(float value, const std::vector<float>& axis);

  int GetResultsIndex(int heightIndex, int velocityIndex, int massIndex, int dragIndex, int liftIndex);
  int GetResultsIndex(float heightIndex, float velocityIndex, float massIndex, float dragIndex, float liftIndex);

  float GetNearestFallTime(int index);
  float GetNearestFallDistance(int index);
  TableResult GetNearestResult(int index);

private:
  int GetNearestTableIndex(float value, const std::vector<float>& table);

private:
  std::vector<TableResult> tableResults;
};

}  // namespace TableSolver