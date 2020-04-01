#ifndef __SIGFOX__
#define __SIGFOX__


#include "Arduino.h"
#include <HardwareSerial.h>

class Sigfox
{
  public:
    Sigfox(uint8_t rxPin, uint8_t txPin, bool debugEn);
    bool emission(const void* trame,  uint8_t size);
  private:
    uint8_t rx, tx;
    HardwareSerial *serialSigfox;
    bool testOK();
    String conversionBinaireHexa(byte octet);
    bool debug;

};

#endif
