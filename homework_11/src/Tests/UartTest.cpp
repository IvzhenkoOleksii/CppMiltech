#include "Tests/UartTest.h"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <cerrno>
#include <cstring>

void UartTest::Test()
{
  int fd = open("/tmp/ttyA", O_RDWR | O_NOCTTY);
  if (fd == -1) {
    std::perror("Failed to oper UART");
    std::cerr << " Ошибка открытия dev/ttyA UART: код ошибки: " << errno << ")" << std::endl;
    exit(1);
  }

  // або /tmp/ttyA від socat
  termios tio;
  tcgetattr(fd, &tio);
  cfmakeraw(&tio);
  // 8N1, без обробки
  cfsetispeed(&tio, B115200);
  cfsetospeed(&tio, B115200);
  tio.c_cflag |= (CLOCAL | CREAD);

  tio.c_cc[VMIN] = 0;
  tio.c_cc[VTIME] = 50;  // Таймаут 1.0 сек
  tcsetattr(fd, TCSANOW, &tio);

  const char *msg = "TEST PING\n";
  write(fd, msg, std::strlen(msg));

  char buf[64];
  int n = read(fd, buf, sizeof(buf));

  // блокується до даних
  (void)n;
  close(fd);
}