#include "Misc.h"

#include <iostream>
#include <ostream>
#include <vector>
#include <filesystem>

void Misc::FindAndRemoveAllZoneIdentifiers()
{
  std::filesystem::path currentFilePath(__FILE__);

  // Получаем родительскую папку, в которой лежит этот файл кода
  std::filesystem::path sourceFolder = currentFilePath.parent_path();
  std::filesystem::path cppMiltechPath = sourceFolder.parent_path().parent_path();

  // auto iteratorOptions = std::filesystem::directory_options::skip_permission_denied;
  auto iteratorOptions = std::filesystem::directory_options::none;

  for (const auto& entry : std::filesystem::recursive_directory_iterator(cppMiltechPath, iteratorOptions)) {
    if (HasFolderInPath(entry.path(), "build")) {
      continue;
    }

    if (HasFolderInPath(entry.path(), ".git")) {
      continue;
    }

    if (HasFolderInPath(entry.path(), ".cache")) {
      continue;
    }

    if (entry.is_regular_file()) {
      if (entry.path().extension() == ".Identifier") {
        std::filesystem::remove(entry.path());
      }
    }
  }
}

bool Misc::HasFolderInPath(const std::filesystem::path& filePath, const std::string& folderName)
{
  // Проходим по каждому компоненту пути
  for (const auto& component : filePath) {
    // Сравниваем компонент с искомым именем папки
    if (component == folderName) {
      return true;
    }
  }
  return false;
}