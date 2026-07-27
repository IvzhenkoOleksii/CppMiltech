#include <vector>
#include "Armament/Solver/Table/Table.h"
#include "Armament/ArmamentDatabase.h"

namespace TableSolver {

class TableController {
public:
  TableController() = default;
  TableController(std::string pathToTable);

public:
  Table table;

  ArmamentDatabase::FallResult Lerp(const ArmamentDatabase::FallResult& first, const ArmamentDatabase::FallResult& second, float coeff);
  Interp GetNearestTableIndex(float value, const std::vector<float>& table);

  int GetResultsIndex(int heightIndex, int velocityIndex, int massIndex, int dragIndex, int liftIndex);
  ArmamentDatabase::FallResult GetNearestResult(int index);

private:
  std::vector<ArmamentDatabase::FallResult> tableResults;
};

}  // namespace TableSolver