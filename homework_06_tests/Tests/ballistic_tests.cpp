#include <gtest/gtest.h>
#include <string>

#include "InputFile.h"
#include "DataStructs.h"
#include "ArmamentDatabase.h"
#include "ArmamentFallCalculator.h"

TEST(ReadFile, ReadJson)
{
  std::string initialString = PROJECT_ROOT_DIR;
  std::string jsonFolderPath = initialString + "/DataFiles/json/";
  std::string testJsonFolderPath = initialString + "/Tests/Data/";

  InputFile inputFile;
  DataStructs::InputData inputData;

  EXPECT_EXIT({ inputFile.ReadJsonFile("Input.json"); }, ::testing::ExitedWithCode(1), ".*");
  std::cout << "[ReadJson] Ended Test 1" << '\n' << '\n';

  EXPECT_EXIT({ inputFile.ReadJsonFile(jsonFolderPath.append("non_existed_file.json")); }, ::testing::ExitedWithCode(1), ".*");
  std::cout << "[ReadJson] Ended Test 2" << '\n' << '\n';

  EXPECT_NO_FATAL_FAILURE({ inputData = inputFile.ReadJsonFile(jsonFolderPath.append("Input.json")); });
  std::cout << "[ReadJson] Ended Test 3" << '\n' << '\n';

  EXPECT_NO_FATAL_FAILURE({ inputData.CheckData(); });
  std::cout << "[ReadJson] Ended Test 4" << '\n' << '\n';

  EXPECT_NO_FATAL_FAILURE({ inputData = inputFile.ReadJsonFile(testJsonFolderPath.append("TestInput.json")); });
  std::cout << "[ReadJson] Ended Test 5" << '\n' << '\n';

  EXPECT_EXIT({ inputData.CheckData(); }, ::testing::ExitedWithCode(1), ".*");
  std::cout << "[ReadJson] Ended Test 6" << '\n' << '\n';

  EXPECT_EXIT(
    {
      inputData = inputFile.ReadJsonFile(testJsonFolderPath.append("WrongAmmo.json"));
      ArmamentDatabase::Data armData = ArmamentDatabase::GetArmament(inputData.DroneData.AmmoType);
    },
    ::testing::ExitedWithCode(1),
    ".*");
  std::cout << "[ReadJson] Ended Test 7" << '\n' << '\n';
}

TEST(MathTests, Math)
{
  ArmamentDatabase::Data testData = {0, 0, 0, 0};

  ArmamentFallCalculator calculator;
  calculator.CalculateFallDistance(testData, 0, 0);
}