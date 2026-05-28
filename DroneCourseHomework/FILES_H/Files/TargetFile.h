#pragma once
#include "DataStructs.h"

#include <vector>

class TargetFile {
public:
  DataStructs::TargetData ReadJsonFile();
};