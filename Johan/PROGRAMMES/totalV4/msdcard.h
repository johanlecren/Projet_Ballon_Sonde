 /**
  * @file       msdcard.h
  * @brief      Définition de la classe Msdcard
  * @version    1.1
  * @author     Johan Le Cren
  * @date       20 avril 2020
*/

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
    bool fichierPresent(fs::FS &fs, const char * path);
    void remplaceCaractere(char ligneCsv[],int longueur,char carSource,char carDest);
  private:
    uint8_t SD_CS;

};



#endif
