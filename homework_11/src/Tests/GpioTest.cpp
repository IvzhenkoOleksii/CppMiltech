#include "Tests/GpioTest.h"

#include <gpiod.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <cerrno>

void GpioTest::Test()
{
  // або gpiochip1 у симуляторі ?
  gpiod_chip *chip = gpiod_chip_open_by_name("gpiochip0");
  if (chip == nullptr) {
    std::cerr << " Ошибка открытия gpiochip0: код ошибки: " << errno << ")" << std::endl;
    exit(1);
  }

  gpiod_line *out = gpiod_chip_get_line(chip, 0);  // лінія-вихід
  gpiod_line *in = gpiod_chip_get_line(chip, 1);   // лінія-вхід

  int v1 = gpiod_line_get_value(in);
  std::cout << "Read in values after step 1:  " << v1 << std::endl;

  gpiod_line_request_output(out, "demo", 0);
  gpiod_line_request_input(in, "demo");

  std::cout << "Passed step 2" << std::endl;

  for (int i = 0; i < 6; ++i) {
    int setValue = i % 2;
    std::cout << "Set value as :  " << setValue << std::endl;
    gpiod_line_set_value(out, setValue);
    // блимаємо виходом
    int valueIn = gpiod_line_get_value(in);    // читаємо вхід
    int valueOut = gpiod_line_get_value(out);  // читаємо вхід
    std::cout << "Read valueIn:  " << valueIn << "   valueOut:  " << valueOut << std::endl;
    (void)valueIn;
    usleep(200000);
  }

  gpiod_line_release(out);
  gpiod_line_release(in);
  gpiod_chip_close(chip);
}