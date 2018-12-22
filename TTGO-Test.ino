#include "TTGO_Config.h"
#include "FreeRTOS.h"
#include "display.h"
#include "store.h"
#include "WiFi.h"

#define XQUEUE_LENGTH 10
#define XQUEUE_TYPE uint8_t
xQueueHandle xQuqeueHandler = NULL;
SPIClass *sdObj = nullptr;
Display *dispObj = nullptr;
// Display dispObj;

//========================================TFT
void tft_test()
{
    dispObj = new Display;
    dispObj->begin();
    // dispObj.begin();
}

//========================================SP SD
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
    dispObj->setCursor(0,0);
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
    uint8_t pVBuff;
    for (;;)
    {
        xQueueReceive(xQuqeueHandler, &pVBuff, portMAX_DELAY);
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
    buzzer_test();
    wifi_test();
    delay(5000);
    xTaskCreate(coreTask, "core", 4096, NULL, 1, NULL);
}

//========================================loop
void loop()
{
}