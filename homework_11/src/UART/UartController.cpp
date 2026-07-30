#include <fcntl.h>
#include <termios.h>
#include <cstdio>
#include <iostream>
#include <unistd.h>

#include "UART/UartController.h"
#include "Drone/drone_link.h"

int UartController::OpenUart(const char* dev)
{
  fileD = open(dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (fileD < 0) {
    std::perror("open");
    return -1;
  }
  termios tio{};
  tcgetattr(fileD, &tio);
  cfmakeraw(&tio);  // 8N1, без обробки символів
  cfsetispeed(&tio, B115200);
  cfsetospeed(&tio, B115200);  // швидкість з обох боків однакова!
  tio.c_cflag |= (CLOCAL | CREAD);
  tcsetattr(fileD, TCSANOW, &tio);
  return fileD;
}

void UartController::ReadFrame()
{
  dlink::Parser parser;  // тримає стан між викликами
  uint8_t buf[256];
  int n = read(fileD, buf, sizeof(buf));  // прочитати доступні байти

  if (n > 0) {
    std::cout << "ReadFrame started. Bytes:    " << n << "  fileD :  " << fileD << std::endl;
  }

  uint8_t type, len, payload[260];
  for (int i = 0; i < n; i++)
    if (parser.feed(buf[i], type, payload, len)) {
      std::cout << "Frame assembled with type: " << type << std::endl;  // зібрався цілий кадр
      if (type == dlink::PKT_TELEMETRY) {
        dlink::Telemetry telemetry;
        memcpy(&telemetry, payload, sizeof telemetry);
        std::cout << "PKT_TELEMETRY assembled" << std::endl;
      }
      else if (type == dlink::PKT_TARGET) {
        dlink::TargetPos targetPos;
        memcpy(&targetPos, payload, sizeof targetPos);
        std::cout << "PKT_TARGET assembled" << std::endl;
      }
      else if (type == dlink::PKT_AMMO) {
        dlink::AmmoCfg ammoCfg;
        memcpy(&ammoCfg, payload, sizeof ammoCfg);
        std::cout << "PKT_AMMO assembled" << std::endl;
      }
    }
}