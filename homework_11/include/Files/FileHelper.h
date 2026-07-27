#pragma once

#include <string>

class FileHelper {
public:
  static std::string CreateGlobalFilePath(const std::string& localFilePath);
  static std::string CheckFilePath(const std::string& filePath);
  static std::string GetFileExtension(const std::string& filePath);
};