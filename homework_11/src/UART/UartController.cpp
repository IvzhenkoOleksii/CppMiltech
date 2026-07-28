#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>

#include "Drone/drone_link.h"
#include <UART/UartController.h>

int UartController::OpenUart(const char* dev)
{
  int fd = open(dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (fd < 0) {
    std::perror("open");
    return -1;
  }
  termios tio{};
  tcgetattr(fd, &tio);
  cfmakeraw(&tio);  // 8N1, без обробки символів
  cfsetispeed(&tio, B115200);
  cfsetospeed(&tio, B115200);  // швидкість з обох боків однакова!
  tio.c_cflag |= (CLOCAL | CREAD);
  tcsetattr(fd, TCSANOW, &tio);
  return fd;
}