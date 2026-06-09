#include <vector>

namespace TableSolver {

struct Table {
  std::vector<float> tableHeight;
  std::vector<float> tableVelocity;
  std::vector<float> tableMass;
  std::vector<float> tableDrag;
  std::vector<float> tableLift;
};

struct TableResult {
  float fallTime;      // time to fall
  float fallDistance;  // distance to fall
};

}  // namespace TableSolver
