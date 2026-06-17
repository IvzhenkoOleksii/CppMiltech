#include "Files/Input/IInputFile.h"
#include "Armament/Solver/IArmamentSolver.h"
#include "Target/ITargetController.h"

class MissionFactory {
public:
  IInputFile* CreateInputFile(const std::string& typeOfFile);
  IArmamentSolver* CreateArmamentSolver(const std::string& typeOfSolver);
  ITargetController* CreateTargetController(const std::vector<DataStructs::Coord2D> positionsData, const float& stepTimeData);
};