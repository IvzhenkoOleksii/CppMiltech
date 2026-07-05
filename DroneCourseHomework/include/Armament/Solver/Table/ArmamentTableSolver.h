#include <string>
#include "Armament/Solver/IArmamentSolver.h"
#include "Armament/Solver/Table/TableController.h"
#include "Armament/ArmamentDatabase.h"

using namespace TableSolver;

class ArmamentTableSolver : public IArmamentSolver {
public:
  ArmamentTableSolver(const std::string& pathToTable);
  ~ArmamentTableSolver();

  ArmamentDatabase::FallResult Calculate(const ArmamentDatabase::Data& armData, float droneAttackSpeed, float droneZPosition) override;

private:
  TableController tableController;
};