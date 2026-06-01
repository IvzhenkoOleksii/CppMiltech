#include <iostream>
#include <vector>

#include "InputFile.h"
#include "Structures.h"
#include "Calculator.h"

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "usage: ugv_odometry <input_path>\n";
    return 1;
  }

  std::string fileName = argv[1];
  InputFile file;
  std::vector<Structures::WheelsOdometrData> data = file.ReadTxtFile(fileName);

  Calculator calc;
  Structures::NrkState nrkState;

  std::cout << std::endl;
  for (int i = 0; i < data.size(); ++i) {
    if (i == 0) {
      // start data are zeros
      continue;
    }

    calc.UpdateState(data[i], data[i - 1], nrkState);

    std::cout << "timestamp:  " << data[i].TimeStamp << "    X: " << nrkState.X << "    Y: " << nrkState.Y
              << "    Theta: " << nrkState.Theta << std::endl;
  }

  return 0;
}