#include "MissionFactory.h"
#include "Files/FileHelper.h"
#include "Files/Input/IInputFile.h"
#include "Files/Input/InputFileTxt.h"
#include "Files/Input/InputFileJson.h"

#include "Armament/Solver/IArmamentSolver.h"
#include "Armament/Solver/ArmamentAnalyticalSolver.h"
#include "Armament/Solver/Table/ArmamentTableSolver.h"

<<<<<<<< HEAD:homework_9/src/MissionFactory.cpp
#include <string>

========
>>>>>>>> Homework10:homework_10/src/MissionFactory.cpp
std::unique_ptr<IInputFile> MissionFactory::CreateInputFile(const std::string& filePath)
{
  fileTypeCode code = ConvertArgumentIntoFileTypeCode(filePath);
  switch (code) {
    case txt:
      return std::make_unique<InputFileTxt>(filePath);
    case json:
    default:
      return std::make_unique<InputFileJson>(filePath);
  }
}

std::unique_ptr<IArmamentSolver> MissionFactory::CreateArmamentSolver(const std::string& typeOfSolver, const std::string& pathToFile)
{
  armamentSolverTypeCode code = ConvertArgumentIntoArmamentSolverTypeCode(typeOfSolver);
  switch (code) {
    case analytical:
    default:
      return std::make_unique<ArmamentAnalitycalSolver>();
    case tables:
      return std::make_unique<ArmamentTableSolver>(pathToFile);
  }
}

MissionFactory::fileTypeCode MissionFactory::ConvertArgumentIntoFileTypeCode(const std::string& filePath)
{
  std::string typeOfFile = FileHelper::GetFileExtension(filePath);
  if (typeOfFile == "txt") {
    return MissionFactory::txt;
  }
  else if (typeOfFile == "json") {
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
  else if (typeOfFile == "Table") {
    return MissionFactory::tables;
  }

  // by default
  return MissionFactory::analytical;
}