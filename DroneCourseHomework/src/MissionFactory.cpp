#include "MissionFactory.h"
#include <iostream>
#include <ostream>
#include "Files/Input/IInputFile.h"
#include "Files/Input/InputFileTxt.h"
#include "Files/Input/InputFileJson.h"

#include "Armament/Solver/IArmamentSolver.h"
#include "Armament/Solver/ArmamentAnalyticalSolver.h"
#include "Target/TargetController.h"

enum fileTypeCode { txt, json };
std::map<std::string, fileTypeCode> inputFileTypes = {
  {"txt", fileTypeCode::txt}, {"Txt", fileTypeCode::txt}, {"json", fileTypeCode::json}, {"Json", fileTypeCode::json}};

enum armamentSolverTypeCode { analytical, tables };
std::map<std::string, armamentSolverTypeCode> solverTypes = {{"Analytical", armamentSolverTypeCode::analytical},
                                                             {"Table", armamentSolverTypeCode::tables}};

IInputFile* MissionFactory::CreateInputFile(const std::string& typeOfFile)
{
  if (!inputFileTypes.contains(typeOfFile)) {
    std::cerr << "Unknown Type of File!  " << typeOfFile << std::endl;
    return nullptr;
  }

  fileTypeCode code = inputFileTypes[typeOfFile];
  switch (code) {
    case txt:
      return new InputFileTxt();
    case json:
      return new InputFileJson();
    default:
      return nullptr;
  }
}

IArmamentSolver* MissionFactory::CreateArmamentSolver(const std::string& typeOfSolver)
{
  if (!solverTypes.contains(typeOfSolver)) {
    std::cerr << "Unknown Type of Solver!  " << typeOfSolver << std::endl;
    return nullptr;
  }

  armamentSolverTypeCode code = solverTypes[typeOfSolver];
  switch (code) {
    case analytical:
      return new ArmamentAnalitycalSolver();
    default:
      return nullptr;
  }

  return new ArmamentAnalitycalSolver();
}

ITargetController* MissionFactory::CreateTargetController(const std::vector<DataStructs::Coord2D> positionsData, const float& stepTimeData)
{
  return new TargetController(positionsData, stepTimeData);
}