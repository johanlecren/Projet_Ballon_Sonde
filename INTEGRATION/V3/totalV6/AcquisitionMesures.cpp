/**
 * @file AcquisitionMesures.cpp
 * @brief Implémentation de la classe AcquisitionMesures
 * @version 1
 * @author Alex TAHIATA
 * @date 23/05/2020
 */

#include "AcquisitionMesures.h"

/**
 * @brief AcquisitionMesures::AcquisitionMesures
 * @details Constructeur de la classe AcquisitionMesures,
 * permet la liaison série avec le GPS
 * @param _b1 broche du GPS
 * @param _b2 broche du GPS
 */
AcquisitionMesures::AcquisitionMesures(uint8_t _b1, uint8_t _b2)
{
  //INITIALISATION GPS
  hs = new HardwareSerial(2);
  hs->begin(4800, SERIAL_8N1, _b1, _b2);

  gpsValide = false;
}

/**
 * @brief AcquisitionMesures::Initialisation
 * @details Méthode qui permet la liaison série via i2c
 * avec le capteur BME280
 */
void AcquisitionMesures::Initialisation()
{
  // BME LIE PAR SERIE I2C
  if (!bme.begin(0x77))
  {
    initBme = 0;
  } else {
    initBme = 1;
  }
}

/**
 * @brief AcquisitionMesures::TraitementGps
 * @details Méthode qui permet de lire les données
 * provenant du GPS et de récupérer la date, l'altitude
 * et la position
 */
boolean AcquisitionMesures::TraitementGps()
{
  if (hs->available())
  {
    char c = hs->read();
    if (tGps.encode(c))
    {
      gpsValide = true;
    }
  }
  if (gpsValide)
  {
    int year;
    unsigned long age;
    byte hundredths;

    tGps.crack_datetime(&year, &mois, &jour, &heure, &minute, &seconde, &hundredths, &age);
    annee = year % 100;
    
    tGps.f_get_position(&latitude, &longitude, &age);
    //Serial.println(latitude);
    //Serial.println(longitude);
    //Serial.println(seconde);
    altitude = tGps.f_altitude();
  }
  /*unsigned long chars;
  unsigned short sentences, failed;
  tGps.stats(&chars, &sentences, &failed);
  if (chars > 0 && sentences == 0)
  {
    Serial.println("gpsACQ Wait");
  }
  if (chars == 0)
  {
    Serial.println("** pas de trame nmea recue vérifier le cablage du gps **");
  }*/
  return gpsValide;
}

/**
 * @brief AcquisitionMesures::LireBme
 * @details Méthode qui permet de récupérer les
 * données de température, pression et humidité
 * du capteur BME280
 */
void AcquisitionMesures::LireBme()
{
  if (initBme == 1)
  {
    temperature = bme.readTemperature();
    humidite = bme.readHumidity();
    pression = bme.readPressure() / 100.0F;
  } else if (initBme == 0) {
    temperature = 0;
    humidite = 0;
    pression = 0;
  }
}

/**
 * @brief AcquisitionMesures::Temperature
 * @details Méthode qui renvoi la valeur de
 * la température
 */
float AcquisitionMesures::Temperature()
{
  return temperature;
}

/**
 * @brief AcquisitionMesures::Humidite
 * @details Méthode qui renvoi la valeur de
 * l'humidite
 */
char AcquisitionMesures::Humidite()
{
  return humidite;
}

/**
 * @brief AcquisitionMesures::Pression
 * @details Méthode qui renvoi la valeur de
 * la pression
 */
short AcquisitionMesures::Pression()
{
  return pression;
}

/**
 * @brief AcquisitionMesures::Altitude
 * @details Méthode qui renvoi la valeur de
 * l'altitude
 */
short AcquisitionMesures::Altitude()
{
  return altitude;
}

/**
 * @brief AcquisitionMesures::Latitude
 * @details Méthode qui renvoi la valeur de
 * la latitude
 */
float AcquisitionMesures::Latitude()
{
  return latitude;
}

/**
 * @brief AcquisitionMesures::Longitude
 * @details Méthode qui renvoi la valeur de
 * la longitude
 */
float AcquisitionMesures::Longitude()
{
  return longitude;
}


String AcquisitionMesures::Ladate()
{
  return String(jour) + "/" + String(mois) + "/" + String(annee);
}

String AcquisitionMesures::Lheure()
{
  return String(heure) + ":" + String(minute) + ":" + String(seconde);
}

/**
 * @brief AcquisitionMesures::Seconde
 * @details Méthode qui renvoi les secondes
 */
unsigned char AcquisitionMesures::Seconde()
{
  return seconde;
}

/**
 * @brief AcquisitionMesures::Minute
 * @details Méthode qui renvoi les minutes
 */
char AcquisitionMesures::Minute()
{
  return minute;
}

/**
 * @brief AcquisitionMesures::Heure
 * @details Méthode qui renvoi l'heure
 */
char AcquisitionMesures::Heure()
{
  return heure;
}

/**
 * @brief AcquisitionMesures::Jour
 * @details Méthode qui renvoi le jour
 */
char AcquisitionMesures::Jour()
{
  return jour;
}

/**
 * @brief AcquisitionMesures::Mois
 * @details Méthode qui renvoi le mois
 */
char AcquisitionMesures::Mois()
{
  return mois;
}

/**
 * @brief AcquisitionMesures::Annee
 * @details Méthode qui renvoi l'année
 */
unsigned int AcquisitionMesures::Annee()
{
  return annee;
}
