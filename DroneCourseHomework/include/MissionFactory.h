#include "Files/Input/IInputFile.h"
#include "Armament/Solver/IArmamentSolver.h"

#include <memory>

class MissionFactory {
public:
  std::unique_ptr<IInputFile> CreateInputFile(const std::string& typeOfFile);
  std::unique_ptr<IArmamentSolver> CreateArmamentSolver(const std::string& typeOfSolver);

private:
  enum fileTypeCode { txt, json };
  fileTypeCode ConvertArgumentIntoFileTypeCode(const std::string& typeOfFile);

  enum armamentSolverTypeCode { analytical, tables };
  armamentSolverTypeCode ConvertArgumentIntoArmamentSolverTypeCode(const std::string& typeOfFile);
};