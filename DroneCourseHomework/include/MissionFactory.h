#include "Files/Input/IInputFile.h"
#include "Armament/Solver/IArmamentSolver.h"

class MissionFactory {
public:
  std::unique_ptr<IInputFile> CreateInputFile(const std::string& filePath);
  std::unique_ptr<IArmamentSolver> CreateArmamentSolver(const std::string& typeOfSolver, const std::string& pathToFile);

private:
  enum fileTypeCode { txt, json };
  fileTypeCode ConvertArgumentIntoFileTypeCode(const std::string& fileName);

  enum armamentSolverTypeCode { analytical, tables };
  armamentSolverTypeCode ConvertArgumentIntoArmamentSolverTypeCode(const std::string& typeOfFile);
};