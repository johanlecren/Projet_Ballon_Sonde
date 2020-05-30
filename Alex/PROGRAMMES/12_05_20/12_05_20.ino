/*Le test unitaire*/

/**
 * @file 12_05_20.ino
 * @brief Code principal
 * @version 1
 * @author Alex TAHIATA
 * @date 23/05/2020
 */

#include "AcquisitionMesures.h"
#include "RadiationWatch.h"

AcquisitionMesures bmeGps(16, 17); //deux broches du gps
RadiationWatch rad(32, 33);

typedef struct {
  unsigned char jour;
  unsigned char mois;
  int annee;
} typeDate;

typedef struct {
  unsigned char seconde;
  unsigned char minute;
  unsigned char heure;
} typeHeure;
  
typedef struct
{
  unsigned short altitude;
  float latitude;
  float longitude;
  char* temperature;
  unsigned char humidite;
  unsigned short pression;
  unsigned short impulsions;

  typeHeure heure;
  typeDate date;
} sauvegardeMSD;
sauvegardeMSD carteSD;

typedef struct
{
  float latitude;
  float longitude;
  unsigned short altitude;
  unsigned short  impulsions : 15;
  unsigned id : 1;
} positionRadiation;
positionRadiation trame1;

typedef struct
{
  float latitude;
  float longitude;
  char *temperature;
  unsigned char humidite ;
  unsigned short  pression : 15;
  unsigned id : 1;
} positionMesures;
positionMesures trame2;

typedef struct
{
  int indice;
  double impulsions;
  int a;
} lesIndices;
lesIndices indiceRad;


TaskHandle_t Tasksensor;
TaskHandle_t Taskmsd;
TaskHandle_t Tasksigfox;

QueueHandle_t queueMSD;
QueueHandle_t queueSigfox;

int queueSize = 128;

/**
 * @brief setup
 * @details Initialisation des capteurs, création des
 * 3 tâches et des 2 files de stockages
 */
void setup() {
  Serial.begin(115200);
  
  queueMSD = xQueueCreate(queueSize, sizeof(int)); // Création de la file sauvegarde
  queueSigfox = xQueueCreate(queueSize, sizeof(queueSigfox)); // Création de al file sigfox
  
  bmeGps.Initialisation(); // Init du BME

  rad.setup(); // Init du capteur rad
  rad.registerNoiseCallback(&onNoise);
  rad.registerRadiationCallback(&onRadiation);
  
  xTaskCreatePinnedToCore(
    tacheAcquisition,   /* La tâche */
    "tacheAcquisition",     /* Le nom de la tâche */
    10000,       /* Taille de la pile de la tâche */
    NULL,        /* Paramètre de la tâche */
    2,           /* Priorité de la tâche */
    &Tasksensor,      /* Task handle pour garder une trace de la tâche créée */
    0);          /* Processeur assigné à la tâche */

  xTaskCreatePinnedToCore(
    tacheReception,   /* La tâche */
    "tacheReception",     /* Le nom de la tâche */
    10000,       /* Taille de la pile de la tâche */
    NULL,        /* Paramètre de la tâche */
    1,           /* Priorité de la tâche */
    &Taskmsd,      /* Task handle pour garder une trace de la tâche créée */
    1);          /* Processeur assigné à la tâche */

  xTaskCreatePinnedToCore(
    tacheReception2,   /* La tâche */
    "tacheReception2",     /* Le nom de la tâche */
    10000,       /* Taille de la pile de la tâche */
    NULL,        /* Paramètre de la tâche */
    1,           /* Priorité de la tâche */
    &Tasksigfox,      /* Task handle pour garder une trace de la tâche créée */
    1);          /* Processeur assigné à la tâche */
}

//INITSTRUCTURE

/**
 * @brief onRadiation
 * @details Méthode nécessaire pour la récupération
 * des radiations
 */
void onRadiation()
{  
  if (indiceRad.indice == 0)
  {
    indiceRad.impulsions = rad.cpm();
  }
  indiceRad.indice = 0;
}

/**
 * @brief onNoise
 * @details Méthode nécessaire pour reconnaître une
 * radiation érronée
 */
void onNoise()
{ 
  indiceRad.indice = indiceRad.indice + 1;
}

/**
 * @brief tacheAcquisition
 * @details Méthode exécuté de manière répétitive
 * concernant l'acquisition des valeurs des 3
 * différents capteurs
 */
void tacheAcquisition( void * pvParameters ) {
  trame1.id = 1;
  while (1) {

    /******************************************************************                 CARTESD                  ***********************************************************************/
    
    bmeGps.LireBme(); // MAJ du BME
    bmeGps.TraitementGps(); // MAJ du GPS
    rad.loop();
    
    if (bmeGps.Seconde() % 15 == 1)
    {
      indiceRad.a = 1;
      carteSD.temperature = bmeGps.Temperature();
      carteSD.humidite = bmeGps.Humidite();
      carteSD.pression = bmeGps.Pression();

      carteSD.impulsions = indiceRad.impulsions;

      carteSD.altitude = bmeGps.Altitude();
      carteSD.longitude = bmeGps.Longitude();
      carteSD.latitude = bmeGps.Latitude();

      carteSD.heure.seconde = bmeGps.Seconde();
      carteSD.heure.minute = bmeGps.Minute();
      carteSD.heure.heure = bmeGps.Heure();

      carteSD.date.jour = bmeGps.Jour();
      carteSD.date.mois = bmeGps.Mois();
      carteSD.date.annee = bmeGps.Annee();
    
      xQueueSend(queueMSD, &carteSD, portMAX_DELAY); // envoi dans la file queueMSD
    }

    /******************************************************************                 SIGFOX trame 1                 ***********************************************************************/

    if (carteSD.heure.seconde % 60 == 1)
    {
      if (trame1.id == 1)
      {
        trame1.impulsions = indiceRad.impulsions;

        trame1.altitude = bmeGps.Altitude();
        trame1.longitude = bmeGps.Longitude();
        trame1.latitude = bmeGps.Latitude();
    
        xQueueSend(queueSigfox, &trame1, portMAX_DELAY); // envoi via la file queueSigfox
        trame1.id = 0;
      } else {
        trame2.temperature = bmeGps.Temperature();
        trame2.humidite = bmeGps.Humidite();
        trame2.pression = bmeGps.Pression();

        trame2.longitude = bmeGps.Longitude();
        trame2.latitude = bmeGps.Latitude();
    
        xQueueSend(queueSigfox, &trame2, portMAX_DELAY); // envoi via la file queueSigfox
        trame1.id = 1;
      }
    }
  }
}

/**
 * @brief tacheReception
 * @details Méthode exécuté de manière répétitive
 * concernant la réception des valeurs des 3
 * différents capteurs pour la sauvegarde des données
 * dans la carte SD
 */
void tacheReception( void * pvParameters ) {
  while (1) {
    xQueueReceive(queueMSD, &carteSD, portMAX_DELAY); // reception via la file queueMSD

    if (indiceRad.a == 1)
    {
      Serial.print("Date : "); Serial.print(String(carteSD.date.jour) + "/" + String(carteSD.date.mois) + "/" + String(carteSD.date.annee));

      Serial.print("       Heure : "); Serial.println(String(carteSD.heure.heure + 2) + ":" + String(carteSD.heure.minute) + ":" + String(carteSD.heure.seconde));

      Serial.println();

      delay(50);
    
      Serial.print("1Température : "); Serial.println(carteSD.temperature);
    
      Serial.print("1Humidité : "); Serial.println(carteSD.humidite);
    
      Serial.print("1Pression : "); Serial.println(carteSD.pression);

      Serial.println();

      delay(50);

      Serial.print("1Radiation : "); Serial.println(carteSD.impulsions);

      Serial.println();

      delay(50);

      Serial.print("1Altitude : "); Serial.println(carteSD.altitude);

      Serial.print("1Longitude : "); Serial.println(carteSD.longitude);

      Serial.print("1Latitude : "); Serial.println(carteSD.latitude);

      Serial.println();

      delay(50);
      indiceRad.a = 0;
    }
  }
}

/**
 * @brief tacheReception2
 * @details Méthode exécuté de manière répétitive
 * concernant la réception des valeurs des 3
 * différents capteurs pour l'envoi des données
 * via Sigfox
 */
void tacheReception2( void * pvParameters ) {
  //envoiSigfox trame; //structure
  while (1) {
    xQueueReceive(queueSigfox, &trame1, portMAX_DELAY); // reception trame 1
    xQueueReceive(queueSigfox, &trame2, portMAX_DELAY); // reception trame 2
    
    /*Serial.print("2Température : "); Serial.println(trame.temperature);
    
    Serial.print("2Humidité : "); Serial.println(trame.humidite);
    
    Serial.print("2Pression : "); Serial.println(trame.pression);

    Serial.println();

    delay(50);

    Serial.print("2Radiation : "); Serial.println(trame.impulsions);

    Serial.println();

    delay(50);

    Serial.print("2Longitude : "); Serial.println(trame.longitude);

    Serial.print("2Latitude : "); Serial.println(trame.latitude);

    Serial.println();

    delay(50);*/
  }
}

/**
 * @brief tacheReception
 * @details Méthode exécuté de manière répétitive
 * concernant la réception des valeurs des 3
 * différents capteurs pour la sauvegarde des données
 * dans la carte SD
 */
void loop() {

}
