#pragma onec
#include "TTGO_Config.h"
#include <TFT_eSPI.h>
#include "Free_Fonts.h" // Include the header file attached to this sketch
#define TFT_GREY 0x5AEB
#define TFT_RED 0xF800

class Display
{
public:
  void begin();
  void show(int x, int y, const char *str);
  void clear();
  void print_P(String str);
  void println_P(const char *str);
  void println_P(String str);
  void showFail(String str);
  void showPass(String str);
  int getWidth() { return _width; }
  int getHeight() { return _height; }
  void setTextSize(uint8_t s),
      setTextColor(uint16_t c),
      setCursor(int16_t x, int16_t y);

private:
  int _fHeight, _fWidth;
  int _height, _width;
#ifdef ENABLE_TFT
  TFT_eSPI *_tft = nullptr;
  // TFT_eSPI _tft = TFT_eSPI();
#endif
};