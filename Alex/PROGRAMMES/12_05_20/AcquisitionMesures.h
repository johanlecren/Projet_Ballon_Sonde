/**
 * @file AcquisitionMesures.h
 * @brief Définition de la classe AcquisitionMesures
 * @version 1
 * @author Alex TAHIATA
 * @date 23/05/2020
 */

#ifndef AcquisitionMesures_h
#define AcquisitionMesures_h

#include <Arduino.h>
#include <Adafruit_BME280.h>
#include <HardwareSerial.h>
#include <TinyGPS.h>

class AcquisitionMesures
{
  public:
    AcquisitionMesures(uint8_t _brocheGps1, uint8_t _brocheGps2);
    void Initialisation();
    void TraitementGps();
    void LireBme();
    
    char* Temperature();
    unsigned char Humidite();
    unsigned short Pression();

    unsigned short Altitude();
    float Latitude();
    float Longitude();

    /*String Ladate();
    String Lheure();*/

    unsigned char Seconde();
    unsigned char Minute();
    unsigned char Heure();

    unsigned char Jour();
    unsigned char Mois();
    int Annee();
    
  private:
    bool gpsValide;
    Adafruit_BME280 bme;
    HardwareSerial *hs;
    TinyGPS tGps;
    bool initBme;

    //BME
    char* temperature;
    unsigned char humidite;
    unsigned short pression;

    //GPS
    unsigned short altitude;
    float latitude;
    float longitude;

    //HEURE
    unsigned char seconde;
    unsigned char minute;
    unsigned char heure;

    //DATE
    unsigned char jour;
    unsigned char mois;
    int annee;
};

#endif
