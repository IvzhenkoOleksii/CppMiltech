#include <gtest/gtest.h>

#include "InputFile.h"
#include "DataStructs.h"
#include "ArmamentDatabase.h"
#include "ArmamentFallCalculator.h"

TEST(ReadFile, ReadJson)
{
  InputFile inputFile;
  DataStructs::InputData inputData;

  EXPECT_EXIT({ inputFile.ReadJsonFile("Input.json"); }, ::testing::ExitedWithCode(1), ".*");
  std::cout << "[ReadJson] Ended Test 1" << '\n';

  EXPECT_EXIT({ inputFile.ReadJsonFile("./homework_06_tests/DataFiles/json/non_existed_file.json"); }, ::testing::ExitedWithCode(1), ".*");
  std::cout << "[ReadJson] Ended Test 2" << '\n';

  EXPECT_NO_FATAL_FAILURE({ inputData = inputFile.ReadJsonFile("./homework_06_tests/DataFiles/json/Input.json"); });
  std::cout << "[ReadJson] Ended Test 3" << '\n';

  EXPECT_NO_FATAL_FAILURE({ inputData.CheckData(); });
  std::cout << "[ReadJson] Ended Test 4" << '\n';

  EXPECT_NO_FATAL_FAILURE({ inputData = inputFile.ReadJsonFile("./homework_06_tests/Tests/Data/TestInput.json"); });
  std::cout << "[ReadJson] Ended Test 5" << '\n';

  EXPECT_EXIT({ inputData.CheckData(); }, ::testing::ExitedWithCode(1), ".*");
  std::cout << "[ReadJson] Ended Test 6" << '\n';

  EXPECT_EXIT(
    {
      inputData = inputFile.ReadJsonFile("./homework_06_tests/Tests/Data/WrongAmmo.json");
      ArmamentDatabase::Data armData = ArmamentDatabase::GetArmament(inputData.DroneData.AmmoType);
    },
    ::testing::ExitedWithCode(1),
    ".*");
  std::cout << "[ReadJson] Ended Test 7" << '\n';
}

TEST(MathTests, Math)
{
  ArmamentDatabase::Data testData = {0, 0, 0, 0};

  ArmamentFallCalculator calculator;
  calculator.CalculateFallDistance(testData, 0, 0);
}