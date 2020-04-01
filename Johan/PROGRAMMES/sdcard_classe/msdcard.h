

#ifndef __MSDCARD__
#define __MSDCARD__

#include "Arduino.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"


class Msdcard
{
  public:
    Msdcard(uint8_t sckPin,uint8_t misoPin,uint8_t mosiPin,uint8_t ssPin); //SCK,MISO,MOSI,ss
    bool begin();
    bool appendFile(fs::FS &fs, const char * path, const char * message);
    bool writeFile(fs::FS &fs, const char * path, const char * message);
  private:
    uint8_t SD_CS;

};



#endif
