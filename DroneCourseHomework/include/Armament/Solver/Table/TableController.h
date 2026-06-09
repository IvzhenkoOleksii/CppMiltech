#include <vector>
#include "Armament/Solver/Table/Table.h"

namespace TableSolver {

class TableController {
public:
  TableController();

public:
  int GetNearestTableMassIndex(const float& realMass);
  int GetNearestTableDragIndex(const float& realDrag);
  int GetNearestTableLiftIndex(const float& realLift);
  int GetNearestTableHeightIndex(const float& realHeight);
  int GetNearestTableVelocityIndex(const float& realVelocity);
  int GetNearestTableFallTimeIndex(const float& realFallTime);

  float GetNearestFallTime(int massIndex, int dragIndex, int liftIndex, int velocityIndex, int heightIndex);
  float GetNearestFallDistance(int massIndex, int dragIndex, int liftIndex, int velocityIndex, int heightIndex);

private:
  int GetNearestTableIndex(float value, const std::vector<float>& table);

private:
  Table table;
  std::vector<TableResult> tableResults;
};

}  // namespace TableSolver