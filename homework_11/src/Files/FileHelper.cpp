#include "Files/FileHelper.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

std::string FileHelper::CreateProjectRootFilePath(const std::string& localFilePath)
{
  std::string inputFilePath = PROJECT_ROOT_DIR + std::string{"/"};
  std::string fullPath = inputFilePath + localFilePath;
  return fullPath;
}

std::string FileHelper::CreateSystemRootFilePath(const std::string& localFilePath)
{
  std::string systemRootPath = std::filesystem::current_path().root_path();
  std::string fullPath = systemRootPath + localFilePath;
  return fullPath;
}

// checked file path types, which can work:
// "DataFiles/json/Input.json"
// "./DroneCourseHomework/DataFiles/json/Ammo.json"
// "/home/rty6061944/CppMiltech/DroneCourseHomework/DataFiles/json/Input.json"

std::string FileHelper::CheckFilePath(const std::string& filePath)
{
  std::ifstream file(filePath);
  if (file.is_open()) {
    file.close();
    return filePath;
  }

  std::string projectRootPath = FileHelper::CreateProjectRootFilePath(filePath);
  file.open(projectRootPath);
  if (file.is_open()) {
    file.close();
    return projectRootPath;
  }

  std::string systemFullPath = FileHelper::CreateSystemRootFilePath(filePath);
  file.open(systemFullPath);
  if (file.is_open()) {
    file.close();
    return systemFullPath;
  }

  std::cerr << "There is no file at path: " << filePath << " nor at: " << projectRootPath << " nor at: " << systemFullPath << std::endl;
  std::exit(1);
}

std::string FileHelper::GetFileExtension(const std::string& filePath)
{
  // FirstSplit using '/' as the delimiter
  std::stringstream stream{filePath};
  std::string segment;
  std::vector<std::string> result;

  while (std::getline(stream, segment, '/')) {
    result.push_back(segment);
  }
  std::string fileName = result[result.size() - 1];
  result.clear();

  // After Split using '.' as the delimiter
  std::stringstream stream2{fileName};
  while (std::getline(stream2, segment, '.')) {
    result.push_back(segment);
  }
  std::string typeOfFile = result[result.size() - 1];
  return typeOfFile;
}