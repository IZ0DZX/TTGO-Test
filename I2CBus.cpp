#include "I2CBus.h"
#include "Wire.h"

void I2CBus::scanI2Cdevice(Display *disp)
{
    String str;
    byte err, addr;
    int nDevices = 0;
    disp->clear();
    disp->setCursor(0, 0);

    for (addr = 1; addr < 127; addr++)
    {
        Wire.beginTransmission(addr);
        err = Wire.endTransmission();
        if (err == 0)
        {
            disp->setTextColor(TFT_GREEN);
            disp->setTextSize(1);
            str = "I2C found:0x";
            if (addr < 16)
                str += "0";
            str += String(addr);
            disp->println_P(str);
            nDevices++;
        }
        else if (err == 4)
        {
            disp->setTextColor(TFT_RED);
            disp->setTextSize(1);
            str = "Unknow addr 0x";
            if (addr < 16)
                str += "0";
            str += String(addr);
            disp->println_P(str);
        }
    }
    if (nDevices == 0)
    {
        disp->setTextColor(TFT_RED);
        disp->setTextSize(2);
        disp->println_P("No I2C devices found");
    }
    else
        disp->println_P("I2C Scan Done");
}

bool I2CBus::readBytes(uint8_t addr, uint8_t reg, uint8_t nbytes, uint8_t *data)
{
    // Set register address
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();

    // Read Nbytes
    Wire.requestFrom(addr, nbytes);
    uint8_t index = 0;
    while (Wire.available())
        data[index++] = Wire.read();
}

bool I2CBus::writeBytes(uint8_t addr, uint8_t reg, uint8_t data)
{
    // Set register address
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
}

I2CBus i2c = I2CBus();
