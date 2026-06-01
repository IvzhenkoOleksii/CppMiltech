#include "Files/Input/IInputFile.h"
#include "Armament/Solver/IArmamentSolver.h"

class MissionFactory {
public:
  IInputFile* CreateInputFile(const std::string& typeOfFile);
  IArmamentSolver* CreateArmamentSolver(const std::string& typeOfSolver);

private:
  enum fileTypeCode { txt, json };
  fileTypeCode ConvertArgumentIntoFileTypeCode(const std::string& typeOfFile);

  enum armamentSolverTypeCode { analytical, tables };
  armamentSolverTypeCode ConvertArgumentIntoArmamentSolverTypeCode(const std::string& typeOfFile);
};