#include "MissionFactory.h"
#include "Files/Input/IInputFile.h"
#include "Files/Input/InputFileTxt.h"
#include "Files/Input/InputFileJson.h"

#include "Armament/Solver/IArmamentSolver.h"
#include "Armament/Solver/ArmamentAnalyticalSolver.h"

#include <memory>

std::unique_ptr<IInputFile> MissionFactory::CreateInputFile(const std::string& typeOfFile)
{
  fileTypeCode code = ConvertArgumentIntoFileTypeCode(typeOfFile);
  switch (code) {
    case txt:
      return std::make_unique<InputFileTxt>();
    case json:
    default:
      return std::make_unique<InputFileJson>();
  }
}

std::unique_ptr<IArmamentSolver> MissionFactory::CreateArmamentSolver(const std::string& typeOfSolver)
{
  armamentSolverTypeCode code = ConvertArgumentIntoArmamentSolverTypeCode(typeOfSolver);
  switch (code) {
    case analytical:
      return std::make_unique<ArmamentAnalitycalSolver>();
    default:
      return std::make_unique<ArmamentAnalitycalSolver>();
  }

  return std::make_unique<ArmamentAnalitycalSolver>();
}

MissionFactory::fileTypeCode MissionFactory::ConvertArgumentIntoFileTypeCode(const std::string& typeOfFile)
{
  if (typeOfFile == "Txt") {
    return MissionFactory::txt;
  }
  else if (typeOfFile == "Json") {
    return MissionFactory::json;
  }

  // by default
  return MissionFactory::json;
}

MissionFactory::armamentSolverTypeCode MissionFactory::ConvertArgumentIntoArmamentSolverTypeCode(const std::string& typeOfFile)
{
  if (typeOfFile == "Analytical") {
    return MissionFactory::analytical;
  }

  // by default
  return MissionFactory::analytical;
}