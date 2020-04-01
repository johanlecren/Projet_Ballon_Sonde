#include "sigfox.h"

Sigfox::Sigfox (uint8_t rxPin = 27 , uint8_t txPin = 26, bool debugEn = true) {
  serialSigfox = new HardwareSerial(1);
  rx = rxPin;
  tx = txPin;
  debug = debugEn;
  serialSigfox->begin(9600, SERIAL_8N1, rx, tx);
}


String Sigfox::conversionBinaireHexa(byte octet)
{
  //48   49
  char table[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  String resultat;
  byte msb = octet / 16;
  byte lsb = octet & 0x0F;
  //Serial.println(table[msb]);
  //Serial.println(table[lsb]);
  resultat = String(table[msb]) + String(table[lsb]);
  if (debug)
  {
    Serial.print(resultat);    
  }
  return resultat;
}



bool Sigfox::emission(const void* trame,  uint8_t size)
{
  uint8_t* bytes = (uint8_t*)trame;
  byte octet;
  int i;
  serialSigfox->print("AT$SF=");
  for (i = 0; i < size; i++) {
    octet = bytes[i];
    serialSigfox->print(conversionBinaireHexa(octet));
  }
  serialSigfox->print("\n");
  return testOK();
}


bool Sigfox::testOK()
{
  bool retour = false;
  String acq = "";
  char caractere;
  int compteur = 0;
  while (!serialSigfox->available() && compteur < 100) {
    delay(100);
    compteur++;
    //Serial.println(compteur);
  }
  while (serialSigfox->available()) {
    caractere = serialSigfox->read();
    if ((caractere != 0x0A) && (caractere != 0x0D)) {      //0x0A Line feed | 0x0D Carriage return
      acq += caractere;
    }
    delay(10);
  }
  if (acq.indexOf("OK") >= 0) {
    retour = true;
  }
  return retour;
}
