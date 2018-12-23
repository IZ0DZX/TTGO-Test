#pragma onec

#include <Arduino.h>
#include "esp32-hal-log.h"
#include <SPI.h>

// #define ENABLE_I2C_SCAN
// #define ENABLE_MAX14070
// #define ENABLE_GPS
// #define ENABLE_MPU9250
// #define ENABLE_DS3231
// #define ENABLE_SDCARD
// #define ENABLE_WS2812B
// #define ENABLE_BUZZER
// #define ENABLE_LOAR
#define ENABLE_SSD1306
// #define ENABLE_SSD1331
// #define ENABLE_SLEEP_MODE
// #define ENABLE_WIFI_MODE
// #define ENABLE_TFT
// #define ENABLE_TFT_BACKLIGHT_CTL
// #define ENABLE_BUTTON
// #define ENABLE_SPI_SD
// #define ENABLE_ADC_TEST
// #define ENABLE_BLE_SCAN
// #define ENABLE_WIFI_SOFT
// #define ENABLE_WIFI_SCAN
// #define ENABLE_ATM90E26
#define ENABLE_SIM800

#ifdef ENABLE_SIM800
#define UART_RX     15
#define UART_TX     2
#define BAUDRATE 9600
#define SIM800_PWR_PIN  14
#define SIM800_RST_PIN  12
#endif

#ifdef ENABLE_ATM90E26
#include <energyic_UART.h>
#define UART_RX     15
#define UART_TX     2
#define MMODE_PIN   0
#endif 


#ifdef ENABLE_BLE_SCAN
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#endif

#ifdef ENABLE_ADC_TEST
#define ADC_BATTERY_PIN 35
#endif

#ifdef ENABLE_SSD1331
// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define SSD1331_MOSI 13
#define SSD1331_CLK 14
#define SSD1331_DC 16
#define SSD1331_RST 4
#define SSD1331_CS 15
#endif

#ifdef ENABLE_SPI_SD
#include <SD.h>
//** T4
#define SD_SPI_CS 13
#define SD_SPI_MISO 2
#define SD_SPI_MOSI 15
#define SD_SPI_CLK 14
//** T10_WIFI_WALL
// #define SD_SPI_CS 13   //5
// #define SD_SPI_MOSI 15 //23
// #define SD_SPI_MISO 2  //19
// #define SD_SPI_CLK 14  //18
#endif

/****************************************************************************/
// 按键
#ifdef ENABLE_BUTTON
#define BUTTON_PINS_ARRAY   {37, 38, 39}
#endif

#ifdef ENABLE_WIFI_MODE
#define CONFIG_SSID "Xiaomi"
#define CONFIG_PASSWD "12345678"
#endif

#ifdef ENABLE_TFT
#define TFT_BL 17 // LED back-light
#define TFT_BK_ON_LEVEL HIGH
#define TFT_BK_OFF_LEVEL LOW
#define TFT_DIR 0
#endif
// sudo echo "deb https://repos.loraserver.io/${DISTRIB_ID,,} ${DISTRIB_CODENAME} testing" | sudo tee /etc/apt/sources.list.d/loraserver.list

//ssd1306 oled显示屏
#ifdef ENABLE_SSD1306
// #define GEOMETRY_128_64 0
// #define GEOMETRY_128_32 1
//*************************
#define SSD1306_TYPE     GEOMETRY_128_64
#define SSD1306_ADDRESS 0x3c
#define I2C_SDA 21
#define I2C_SCL 22
#endif

//lora模块
#ifdef ENABLE_LOAR
#include <LoRa.h>
//lora频率设置
#define LORA_PERIOD 868
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23 //14//
#define DI0 26
//设置为1则设定为lora 发送， 0 则为接收
#define LORA_SENDER     0
#define LORA_SENDER_PERIOD  3 //sec发送周期
#endif

//蜂鸣器
#ifdef ENABLE_BUZZER
#define BUZZER_PIN 25 //0//32
//无源蜂鸣器设置1 有源设置0
#define BUZZER_PASSIVE 1
#define CHANNEL_0 0
#endif

//WS2812B RGB 彩灯
#ifdef ENABLE_WS2812B
#include <Adafruit_NeoPixel.h>
#define WS2812B_PIN 33
#define WS2812B_NUMS 1
#endif

//GPS模块
#ifdef ENABLE_GPS
#include <TinyGPS++.h>
#define GPS_BANUD_RATE 9600
#define GPS_RX_PIN 16
#define GPS_TX_PIN 17
#endif

#ifdef ENABLE_SIM800
#include <sim800.h>
#define SIM800_TX           17
#define SIM800_RX           16
#define SIM800_POWER        9
#define SIM800_RST          12
#define SIM800_BAUDRATE         9600


#endif

//MPU9250 九轴姿态
#ifdef ENABLE_MPU9250
#define MPU9250_ADDRESS 0x69
#endif

//RTC 时钟
#ifdef ENABLE_DS3231
#include <RtcDS3231.h>
// #define DS3231_ADDRESS 0x68
#endif

//MAX14070 电量检测
#ifdef ENABLE_MAX14070
#define MAX14070_ADDRESS 0x36
#endif

#ifdef ENABLE_LOAR
#if LORA_PERIOD == 433
#define BAND 433E6
#elif LORA_PERIOD == 868
#define BAND 868E6
#elif LORA_PERIOD = 915
#define BAND 915E6
#else
#define BAND 433E6
#endif
#endif

