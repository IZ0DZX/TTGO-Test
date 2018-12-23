#include "TTGO_Config.h"
#include "FreeRTOS.h"
// #include "display.h"
#include "I2CBus.h"
#include "store.h"
#include "WiFi.h"
#include <Ticker.h>
#include <Button2.h>

#define BUTTON_CLASS Button2
typedef struct
{
    uint8_t code;
    // void *param;
    uint8_t btnNum;
    String str;
    uint32_t len;
} queue_t;

#define QUEUE_BUTTON_CODE 0
#define QUEUE_DISPLAY_CODE 1

#define XQUEUE_LENGTH 10
#define XQUEUE_TYPE queue_t
xQueueHandle xQuqeueHandler = NULL;
SPIClass *sdObj = nullptr;
Display *dispObj = nullptr;
Button2 *btn = nullptr;
Ticker timer;
Ticker loraTicker;

#ifdef ENABLE_GPS
TinyGPSPlus gps;
#endif

#ifdef ENABLE_SIM800
SIM800 sim800(SIM800_BAUDRATE, SIM800_RX, SIM800_TX, SIM800_POWER);
#endif

#ifdef ENABLE_WS2812B
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(WS2812B_NUMS, WS2812B_PIN, NEO_GRB + NEO_KHZ800);
Ticker rgbTicker;
#endif

#ifdef ENABLE_DS3231
RtcDS3231<TwoWire> rtc(Wire);
#endif

//========================================BUTTON
#ifdef ENABLE_BUTTON
byte btnPins[] = BUTTON_PINS_ARRAY;
#define BUTTON_TOTAL sizeof(btnPins) / sizeof(btnPins[0])
void buttonCallBack(BUTTON_CLASS &b)
{
    static XQUEUE_TYPE qParam;
    for (int i = 0; i < BUTTON_TOTAL; i++)
    {
        if (btn[i] == b)
        {
            qParam.code = QUEUE_BUTTON_CODE;
            qParam.btnNum = btnPins[i];
            xQueueSend(xQuqeueHandler, &qParam, portMAX_DELAY);
        }
    }
}
#endif

void button_test()
{
#ifdef ENABLE_BUTTON
    btn = new BUTTON_CLASS[BUTTON_TOTAL];
    for (int i = 0; i < BUTTON_TOTAL; i++)
    {
        btn[i] = BUTTON_CLASS(btnPins[i]);
        btn[i].setClickHandler(buttonCallBack);
    }
#endif
}

//========================================TFT
void tft_test()
{
    dispObj = new Display;
    dispObj->begin();
    // dispObj.begin();
}

//========================================SPI SD
void spisd_test()
{
#ifdef ENABLE_SPI_SD
    sdObj = new SPIClass(HSPI);
    sdObj->begin(SD_SPI_CLK, SD_SPI_MISO, SD_SPI_MOSI, SD_SPI_CS);
    dispObj->clear();
    if (!SD.begin(SD_SPI_CS, *sdObj))
    {
        dispObj->setTextColor(TFT_RED);
        dispObj->setTextSize(2);
        dispObj->show(dispObj->getWidth() / 2, dispObj->getHeight() / 2, "SD FAIL");
        return;
    }
    else
    {
        dispObj->setTextColor(TFT_GREEN);
        dispObj->setTextSize(1);
        uint64_t size = SD.cardSize();
        String info = "SIZE:" + String((uint32_t)size / 1024 / 1024) + "MB";
        dispObj->showPass(info);
    }
    listDir(SD, "/", 2, dispObj);
#endif
}

//========================================BUZZER
void playSound(void)
{
#ifdef ENABLE_BUZZER
#if (BUZZER_PASSIVE)
    ledcWriteTone(CHANNEL_0, 1000);
    delay(500);
    ledcWriteTone(CHANNEL_0, 0);
    delay(500);
#else
    digitalWrite(BUZZER_PIN, HIGH);
    delay(800);
    digitalWrite(BUZZER_PIN, LOW);
    delay(800);
#endif
#endif
}

void buzzer_test()
{
#ifdef ENABLE_BUZZER
#if (BUZZER_PASSIVE)
    ledcSetup(CHANNEL_0, 1000, 8);
    ledcAttachPin(BUZZER_PIN, CHANNEL_0);
#else
    pinMode(BUZZER_PIN, OUTPUT);
#endif
    playSound();
    playSound();
#endif
}

//========================================WIFI SCAN
void wifi_test()
{
#ifdef ENABLE_WIFI_SCAN
    dispObj->clear();
    dispObj->setCursor(0, 0);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    int n = WiFi.scanNetworks();
    if (n == 0)
    {
        dispObj->setTextColor(TFT_RED);
        dispObj->setTextSize(2);
        dispObj->show(dispObj->getWidth() / 2, dispObj->getHeight() / 2, "no networks found");
        return;
    }

    dispObj->setTextColor(TFT_GREEN);
    dispObj->setTextSize(1);
    for (int i = 0; i < n; ++i)
    {
        char buf[512];
        sprintf(buf,
                "[%d]:%s(%d)",
                i + 1,
                WiFi.SSID(i).c_str(),
                WiFi.RSSI(i));
        Serial.println(buf);
        dispObj->println_P(String(buf));
    }
#endif
}

//========================================CORE TASK
void coreTask(void *param)
{
    XQUEUE_TYPE qParam;
    for (;;)
    {
        xQueueReceive(xQuqeueHandler, &qParam, portMAX_DELAY);
        switch (qParam.code)
        {
        case QUEUE_BUTTON_CODE:
            // Serial.println("Recv");
            dispObj->setCursor(0, 0);
            dispObj->println_P(qParam.str);
            break;
        case QUEUE_DISPLAY_CODE:
            break;
        default:
            break;
        }
    }
}

//========================================setup
void runTask(void *param)
{
    for (;;)
    {
#ifdef ENABLE_GPS
        if (gps.location.isValid())
        {
            playSound();
        }
        while (Serial1.available())
            gps.encode(Serial1.read());
        delay(200);
#endif
    }
}

//========================================setup
void setup()
{
    Serial.begin(115200);

    while (!Serial)
        ;

    xQuqeueHandler = xQueueCreate(XQUEUE_LENGTH, sizeof(XQUEUE_TYPE));
    if (!xQuqeueHandler)
    {
        log_e("queue create fail");
        while (1)
            ;
    }

    tft_test();
    spisd_test();
    delay(5000);

    i2c.scanI2Cdevice(dispObj);
    delay(5000);

    buzzer_test();
    wifi_test();
    delay(5000);

    button_test();

    dispObj->clear();
    xTaskCreate(coreTask, "core", 4096, NULL, 1, NULL);
    xTaskCreate(runTask, "run", 4096, NULL, 1, NULL);
}

//========================================loop
void loop()
{
#ifdef ENABLE_BUTTON
    for (int i = 0; i < BUTTON_TOTAL; i++)
    {
        // btn[i].tick();
        btn[i].loop();
    }
#endif
}

//========================================I2C
void gps_test()
{
#ifdef ENABLE_GPS
    Serial1.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
#endif
}

//========================================LORA
#ifdef ENABLE_LOAR
#if !LORA_SENDER
void loar_task(void *prarm)
{
    static XQUEUE_TYPE qParam;
    for (;;)
    {
        if (LoRa.parsePacket())
        {
            String recv = "";
            while (LoRa.available())
            {
                recv += (char)LoRa.read();
            }
            recv += "RSSI" + String(LoRa.packetRssi());
            qParam.code = QUEUE_DISPLAY_CODE;
            qParam.str = recv;
            xQueueSend(xQuqeueHandler, &qParam, portMAX_DELAY);
        }
    }
}
#endif
#endif

void lora_test()
{
#ifdef ENABLE_LOAR
    SPI.begin(SCK, MISO, MOSI, SS);
    LoRa.setPins(SS, RST, DI0);
    if (!LoRa.begin(BAND))
    {
        dispObj->show(0, 0, "LoRa Started Failed");
        return;
    }
    else
    {
        dispObj->show(0, 0, "LoRa Started PASS");
    }

#if LORA_SENDER
    loraTicker.attach(LORA_SENDER_PERIOD, [] {
        static uint32_t i = 0;
        LoRa.beginPacket();
        LoRa.print("lora: ");
        LoRa.print(i++);
        LoRa.endPacket();
    });
#else
    xTaskCreatePinnedToCore(loar_task, "loar_task", 2048, NULL, 2, NULL, 1);
#endif
#endif
}

//======================================== WS2812B

void fixedColor(uint32_t color)
{
    for (uint16_t i = 0; i < pixels.numPixels(); i++)
    {
        pixels.setPixelColor(i, color);
        pixels.show();
    }
}

void ws2812b_test()
{
    pixels.setBrightness(60);
    pixels.begin();
    pixels.show();

#ifdef ENABLE_WS2812B
    rgbTicker.attach(2, [] {
        static int i = 0;
        static uint16_t colorMap[] = {RED, GREEN, BLUE, WHITE, BLACK};
        fixedColor(colorMap[i++]);
    });
#endif
}

//======================================== ATM90E26
void atm90e26_test()
{
#ifdef ENABLE_ATM90E26

#endif
}

//======================================== SIM800
void sim800_test()
{
#ifdef ENABLE_SIM800
    // Serial1.begin(BAUDRATE, SERIAL_8N1, UART_RX, UART_TX);
    if (!sim800.begin())
    {
        dispObj->show(0, 0, "sim800 Started Failed");
        return;
    }
    else
    {
        dispObj->show(0, 0, "sim800 Started PASS");
    }
#endif
}

//======================================== ATM90E26
void adc_test()
{
#ifdef ENABLE_ADC_TEST
    // pinMode(ADC_BATTERY_PIN, INPUT);
    // uint16_t batt = analogRead(ADC_BATTERY_PIN);
    // String battStr = "BATT:" + String(batt);
    // Serial.println(battStr);
#endif
}

//======================================== DS3231
void ds3231_test()
{
#ifdef ENABLE_DS3231
    rtc.Begin();
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    if (!rtc.IsDateTimeValid())
    {
        rtc.SetDateTime(compiled);
    }
    if (!rtc.GetIsRunning())
    {
        // Serial.println("RTC was not actively running, starting now");
        rtc.SetIsRunning(true);
    }
    RtcDateTime now = rtc.GetDateTime();
    if (now < compiled)
    {
        // Serial.println("RTC is older than compile time!  (Updating DateTime)");
        rtc.SetDateTime(compiled);
    }

    rtc.Enable32kHzPin(false);

    rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmOne);

    rtc.LatchAlarmsTriggeredFlags();

    RtcTemperature temp = rtc.GetTemperature();

    Serial.print("RTC Temp : ");
    temp.Print(Serial);
    Serial.println();

    now = rtc.GetDateTime();
    char datestring[20];
    Serial.print("RTC Date : ");
    snprintf_P(datestring,
               sizeof(datestring) / sizeof(datestring[0]),
               PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               now.Month(),
               now.Day(),
               now.Year(),
               now.Hour(),
               now.Minute(),
               now.Second());
    
    Serial.println(datestring);
#endif
}
