#include "TTGO_Config.h"
#include "display.h"
#ifdef ENABLE_SPI_SD

void listDir(fs::FS &fs, const char *dirname, uint8_t levels, Display *dsp)
{
    dsp->println_P("Listing directory: " + String(dirname));

    File root = fs.open(dirname);
    if (!root)
    {
        dsp->println_P("Failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        dsp->println_P("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            dsp->println_P("[D]" + String(file.name()));
            if (levels)
            {
                listDir(fs, file.name(), levels - 1, dsp);
            }
        }
        else
        {
            dsp->println_P("[F]" + String(file.name()));
        }
        file = root.openNextFile();
    }
}
#endif