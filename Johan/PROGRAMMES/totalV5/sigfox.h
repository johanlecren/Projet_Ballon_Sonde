#ifndef __SIGFOX__
#define __SIGFOX__

 /**
  * @file       sigfox.h
  * @brief      Définition de la classe Sigfox
  * @version    1.1
  * @author     Johan Le Cren
  * @date       20 avril 2020
*/



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
