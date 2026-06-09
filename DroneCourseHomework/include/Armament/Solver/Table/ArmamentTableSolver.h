#include "Armament/Solver/IArmamentSolver.h"
#include "Armament/Solver/Table/TableController.h"
#include "Armament/ArmamentDatabase.h"

using namespace TableSolver;

class ArmamentTableSolver : public IArmamentSolver {
public:
  ArmamentTableSolver();
  ~ArmamentTableSolver();
  float CalculateFallTime(const ArmamentDatabase::Data& armData, float droneAttackSpeed, float droneZPosition) override;
  float CalculateFallDistance(const ArmamentDatabase::Data&, float droneAttackSpeed, float fallTime) override;

private:
  TableController tableController;
};