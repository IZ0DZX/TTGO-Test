
#include "display.h"

//========================================
void Display::begin()
{
    //========================================
#ifdef ENABLE_TFT
    _tft = new TFT_eSPI();
    _tft->init();
    _tft->setRotation(TFT_DIR);

#ifdef ENABLE_TFT_BACKLIGHT_CTL
    pinMode(TFT_BL, OUTPUT);
    //turn on backlight
    digitalWrite(TFT_BL, TFT_BK_ON_LEVEL);
#endif
    _tft->setTextFont(2);
    _tft->fillScreen(TFT_BLACK);
    _tft->setTextSize(1);
    _tft->setCursor(0, 0, 2);

    _height = _tft->height();
    _width = _tft->width();
#endif
    //========================================

    //========================================

    //========================================
}
void Display::showFail(String str)
{
#ifdef ENABLE_TFT
    _tft->setTextFont(2);
    _tft->setTextColor(TFT_RED, TFT_BLACK);
    _tft->println(str);
#endif
}

void Display::showPass(String str)
{
#ifdef ENABLE_TFT
    _tft->setTextFont(2);
    _tft->setTextColor(TFT_GREEN, TFT_BLACK);
    _tft->println(str);
#endif
}

void Display::show(int x, int y, const char *str)
{
    //========================================
#ifdef ENABLE_TFT
    _tft->drawString(str, x, y);
#endif
    //========================================

    //========================================
}
//========================================
void Display::clear()
{
    //========================================
#ifdef ENABLE_TFT
    _tft->fillScreen(0);
#endif
    //========================================

    //========================================
}

void Display::print_P(String str)
{
#ifdef ENABLE_TFT
    _tft->print(str);
#endif
}

void Display::println_P(String str)
{
#ifdef ENABLE_TFT
    _tft->println(str);
#endif
}

void Display::println_P(const char *str)
{
#ifdef ENABLE_TFT
    _tft->println(str);
#endif
}

void Display::setTextSize(uint8_t s)
{
#ifdef ENABLE_TFT
    _tft->setTextSize(s);
#endif
}

void Display::setTextColor(uint16_t c)
{
#ifdef ENABLE_TFT
    _tft->setTextColor(c);
#endif
}

void Display::setCursor(int16_t x, int16_t y)
{
#ifdef ENABLE_TFT
    _tft->setCursor(x, y);
#endif
}