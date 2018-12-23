
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
#ifdef ENABLE_SSD1306
    display = new SSD1306(SSD1306_ADDRESS, I2C_SDA, I2C_SCL, SSD1306_TYPE);
    if (!display->init())
    {
        log_e("SSD1306 Init Fail");
        return;
    }
    // display->flipScreenVertically();
    display->setFont(ArialMT_Plain_16);
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->clear();
    // delay(50);
    display->drawString(display->getWidth() / 2, display->getHeight() / 2, "TTGO");
    display->display();

    _height = display->height();
    _width = display->width();
#endif

    //========================================
#ifdef ENABLE_SSD1331
    display = new Adafruit_SSD1331(SSD1331_CS, SSD1331_DC, SSD1331_MOSI, SSD1331_CLK, SSD1331_RST);
    display->begin();
    display->fillScreen(0);
    display->setCursor(0, 0);
    // display->println("SSD1331 RGB OLED");
#endif

    //========================================
}
void Display::showFail(String str)
{
    //========================================
#ifdef ENABLE_TFT
    _tft->setTextFont(2);
    _tft->setTextColor(TFT_RED, TFT_BLACK);
    _tft->println(str);
#endif
//========================================
#ifdef ENABLE_SSD1306
#endif
//========================================
#ifdef ENABLE_SSD1331
#endif
}

void Display::showPass(String str)
{
    //========================================
#ifdef ENABLE_TFT
    _tft->setTextFont(2);
    _tft->setTextColor(TFT_GREEN, TFT_BLACK);
    _tft->println(str);
#endif
//========================================
#ifdef ENABLE_SSD1306
#endif
//========================================
#ifdef ENABLE_SSD1331
#endif
}

void Display::show(int x, int y, const char *str)
{
    //========================================
#ifdef ENABLE_TFT
    _tft->drawString(str, x, y);
#endif
//========================================
#ifdef ENABLE_SSD1306
#endif
//========================================
#ifdef ENABLE_SSD1331
#endif
}
//========================================
void Display::clear()
{
    //========================================
#ifdef ENABLE_TFT
    _tft->fillScreen(0);
#endif
//========================================
#ifdef ENABLE_SSD1306
#endif
//========================================
#ifdef ENABLE_SSD1331
#endif
}

void Display::print_P(String str)
{
#ifdef ENABLE_TFT
    _tft->print(str);
#endif
//========================================
#ifdef ENABLE_SSD1306
#endif
//========================================
#ifdef ENABLE_SSD1331
#endif
}

void Display::println_P(String str)
{
#ifdef ENABLE_TFT
    _tft->println(str);
#endif
//========================================
#ifdef ENABLE_SSD1306
#endif
//========================================
#ifdef ENABLE_SSD1331
#endif
}

void Display::println_P(const char *str)
{
#ifdef ENABLE_TFT
    _tft->println(str);
#endif
//========================================
#ifdef ENABLE_SSD1306
#endif
//========================================
#ifdef ENABLE_SSD1331
#endif
}

void Display::setTextSize(uint8_t s)
{
#ifdef ENABLE_TFT
    _tft->setTextSize(s);
#endif
//========================================
#ifdef ENABLE_SSD1306
#endif
//========================================
#ifdef ENABLE_SSD1331
#endif
}

void Display::setTextColor(uint16_t c)
{
#ifdef ENABLE_TFT
    _tft->setTextColor(c);
#endif
//========================================
#ifdef ENABLE_SSD1306
#endif
//========================================
#ifdef ENABLE_SSD1331
#endif
}

void Display::setCursor(int16_t x, int16_t y)
{
#ifdef ENABLE_TFT
    _tft->setCursor(x, y);
#endif
//========================================
#ifdef ENABLE_SSD1306
#endif
//========================================
#ifdef ENABLE_SSD1331
#endif
}