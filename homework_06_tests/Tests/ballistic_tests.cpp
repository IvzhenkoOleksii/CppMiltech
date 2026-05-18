#include <gtest/gtest.h>

#include "InputFile.h"
#include "DataStructs.h"

TEST(ReadFile, ReadJson)
{
    InputFile inputFile;
    DataStructs::InputData inputData;

    EXPECT_EXIT({
        inputFile.ReadJsonFile("Input.json");
    }, ::testing::ExitedWithCode(1), ".*");
    std::cout << "[ReadJson] Ended Test 1" << std::endl;


    EXPECT_EXIT({
        inputFile.ReadJsonFile("./homework_06_tests/DataFiles/json/non_existed_file.json");
    }, ::testing::ExitedWithCode(1), ".*");
    std::cout << "[ReadJson] Ended Test 2" << std::endl;


    EXPECT_NO_FATAL_FAILURE({
        inputData = inputFile.ReadJsonFile("./homework_06_tests/DataFiles/json/Input.json");
    });
    std::cout << "[ReadJson] Ended Test 3" << std::endl;


    EXPECT_NO_FATAL_FAILURE({
        inputData.CheckData();
    });
    std::cout << "[ReadJson] Ended Test 4" << std::endl;


    EXPECT_NO_FATAL_FAILURE({
        inputData = inputFile.ReadJsonFile("./homework_06_tests/Tests/Data/TestInput.json");
    });
    std::cout << "[ReadJson] Ended Test 5" << std::endl;


    EXPECT_EXIT({
        inputData.CheckData();
    }, ::testing::ExitedWithCode(1), ".*");
    std::cout << "[ReadJson] Ended Test 6" << std::endl;


    ASSERT_EXIT({
        inputData.CheckData();
        std::exit(0); 
    }, ::testing::ExitedWithCode(0), ".*");
}