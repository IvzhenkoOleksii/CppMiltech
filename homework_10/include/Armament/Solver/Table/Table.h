#include <vector>

namespace TableSolver {

struct Table {
  std::vector<float> tableHeight;
  std::vector<float> tableVelocity;
  std::vector<float> tableMass;
  std::vector<float> tableDrag;
  std::vector<float> tableLift;
};

struct Interp {
  int index;
  float fraction;

  bool IsEqaul(Interp value)
  {
    if (index != value.index || fraction != value.fraction) {
      return false;
    }
    return true;
  }
};

}  // namespace TableSolver
