class UartController {
public:
  int fileD;
  int OpenUart(const char* dev);
  void ReadFrame();
};