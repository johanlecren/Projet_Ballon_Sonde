#include "msdcard.h"

Msdcard::Msdcard(uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin, uint8_t ssPin) {
  //SPI.begin(14, 2, 15, 13); //SCK,MISO,MOSI,ss
  SD_CS = ssPin;
  SPI.begin(sckPin, misoPin, mosiPin, ssPin); //SCK,MISO,MOSI,ss
  delay(10);
}

bool Msdcard::begin() {
  bool erreurCarte = false;

  if (!SD.begin(SD_CS, SPI, 40000000, "/sd")) {
    Serial.println("Card Mount Failed");
    erreurCarte = true;
  }
  else {
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
      Serial.println("No SD card attached");
      erreurCarte = true;
    }
    else {
      switch (cardType) {
        case CARD_MMC: Serial.println("MMC");
          break;
        case CARD_SD: Serial.println("SDSC");
          break;
        case CARD_SDHC:
          Serial.println("SDHC");
          break;
      }
      uint64_t cardSize = SD.cardSize() / (1024 * 1024);
      Serial.printf("SD Card Size: %lluMB\n", cardSize);
    }
  }
  return erreurCarte;
}


bool Msdcard::writeFile(fs::FS &fs, const char * path, const char * message) {
  bool erreurWrite = false;
  File file = fs.open(path, FILE_WRITE);
  Serial.printf("Ecriture du fichier: %s\n", path);
  if (!file) {
    Serial.println("Erreur d'ouverture en écriture");
    erreurWrite = true;
  }
  else {
    if (!file.print(message)) {
      erreurWrite = true;
    }
    file.close();
  }
  return erreurWrite;
}


bool Msdcard::appendFile(fs::FS & fs, const char * path, const char * message) {
  bool erreurAppend = false;
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    erreurAppend = true;
  }
  else {
    if (!file.print(message)) {
      erreurAppend = true;
    }
    file.close();
  }
  return erreurAppend;
}
