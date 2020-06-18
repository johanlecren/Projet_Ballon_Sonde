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
    boolean TraitementGps();
    void LireBme();
    
    float Temperature();
    char Humidite();
    short Pression();

    short Altitude();
    float Latitude();
    float Longitude();

    String Ladate();
    String Lheure();

    unsigned char Seconde();
    char Minute();
    char Heure();

    char Jour();
    char Mois();
    unsigned int Annee();
    
  private:
    bool gpsValide;
    Adafruit_BME280 bme;
    HardwareSerial *hs;
    TinyGPS tGps;
    bool initBme;

    //BME
    float temperature;
    char humidite;
    short pression;

    //GPS
    short altitude;
    float latitude;
    float longitude;

    //HEURE
    unsigned char seconde;
    unsigned char minute;
    unsigned char heure;
    
    
    //DATE
    unsigned char jour;
    unsigned char mois;
    unsigned int annee;
};

#endif
