#pragma once

#include <string>
#include <filesystem>

class Misc {
public:
  void FindAndRemoveAllZoneIdentifiers();

private:
  bool HasFolderInPath(const std::filesystem::path& filePath, const std::string& folderName);
};
