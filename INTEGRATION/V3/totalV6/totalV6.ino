/**
    @file       totalV4.cpp
    @brief      programme principal, gestion des taches, des files et du serveur WEB
    @version    1.1
    @author     Johan Le Cren
    @date       18 juin 2020
*/

#include "structure.h"
#include "sigfox.h"
#include "msdcard.h"
#include "index.h"
#include "WiFi.h"
#include <WebServer.h>
#include "AcquisitionMesures.h"
#include "RadiationWatch.h"

const char *ssid = "ESP32AP";
const char *password = "totototo";   //doit être de 8 caractère minimum sinon le ssid n'est pas pris en compte


//#define SIGFOX_MODEM_ON   //active l'émission radio Sigfox
//#define USE_SD_CARD_ON    //active la sauvegarde dans la carte mSD
#define ALTITUDE_MAX_WIFI 2000

#define RAPPORT_SD_SIGFOX 8  // 8 memorisations dans la carte sd pour 1 envoi telemesures Sigfox (2 minutes = 8*15 secondes)

#define RAPPORT_USVH 53.032
#define queueSize 10   //nb d'éléments max dans les files

#pragma execution_character_set("utf-8")
#define NOM_FICHIER "/telemesures.csv"


Sigfox modem(27, 26, true);
Msdcard cartesd(14, 2, 15, 13); //SCK,MISO,MOSI,ss
AcquisitionMesures bmeGps(16, 17); //deux broches du gps
RadiationWatch rad(32, 33);
lesIndices indiceRad;

TaskHandle_t Tasksensor;  //déclaration de la tâche sensor
TaskHandle_t Tasksigfox;  //déclaration de la tâche sigfox
TaskHandle_t Taskmsd;  //déclaration de la tâche msd

QueueHandle_t queueSigfox;  //déclarationde la file sigfox
QueueHandle_t queueMSD;  //déclaration de la file msd
QueueHandle_t queueEnvoiBouton; //déclaration de la file EnvoiBouton

sauvegardeMSD carteSd;  // données carteSd

int messages = 0;      //Variable de comptage de message

WebServer server(80);  //instanciation du serveur WEB

void setup() {
  Serial.begin(115200);
  //initialisation du serveur web

  WiFi.softAP(ssid, password);  //l'ESP32 est en point d'accès

  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  server.on ( "/",  pageIndex );        //lien entre les pages web et les fonctions
  server.on("/Value", pageMesures);
  server.on("/Sigfox", commandeSigfox);
  server.onNotFound(handle_NotFound);
  server.begin();

  bmeGps.Initialisation(); // Init du BME

  rad.setup(); // Init du capteur rad
  rad.registerNoiseCallback(&onNoise);
  rad.registerRadiationCallback(&onRadiation);

  queueSigfox = xQueueCreate( queueSize, sizeof(positionRadiation)); //creation de la file Sigfox
  queueMSD = xQueueCreate( queueSize, sizeof( sauvegardeMSD ));   // création de la file MSD
  queueEnvoiBouton = xQueueCreate( queueSize, sizeof( boolean ));   // création de la file EnvoiBouton

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
    sensorTask,   /* fonction de tâche */
    "sensorTask",     /* nom de la tâche. */
    10000,       /* taille de la pile */
    NULL,        /* paramètres de la tâche */
    1,           /* priorité de la tâche */
    &Tasksensor,      /* Task handle */
    0);          /* tâche dans le coeur 0 */
  delay(500);
  Serial.println("Tâche sensorTask créée...");

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
    sigfoxTask,   /* fonction de tâche */
    "sigfoxTask",     /* nom de la tâche. */
    10000,       /* taille de la pile */
    NULL,        /* paramètres de la tâche */
    1,           /* priorité de la tâche */
    &Tasksigfox,      /* Task handle */
    1);          /* tâche dans le coeur 1 */
  Serial.println("Tâche sigfoxTask créée...");
  delay(500);
  xTaskCreatePinnedToCore(
    msdTask,   /* fonction de tâche */
    "msdTask",     /* nom de la tâche. */
    10000,       /* taille de la pile  */
    NULL,        /* paramètres de la tâche */
    2,           /* priorité de la tâche */
    &Taskmsd,      /* Task handle  */
    1);          /* tâche dans le coeur 1 */
  Serial.println("Tâche msd Task créée...");
  delay(500);

}

/*
  Gestion de la page Web
  4 handles sont utilisé afin d'envoyer la page WEB au client,
  gérer la mise à jour des mesures,
  gérer la commande manuelle (forçage d'emission),
  et envoi d'un message en cas d'URL érronée
*/
void pageIndex() {
  Serial.println("requete GET / page index");
  server.send ( 200, "text/html",  MAIN_page);
}

void pageMesures() {
  Serial.println("requete GET / mesures");
  String position = String (carteSd.latitude, 6) + ";" + String (carteSd.longitude, 6);
  //envoi des données au serveur
  server.send ( 200, "text/plain", String(carteSd.temperature) + ";" + String(carteSd.pression) + ";" + String(carteSd.humidite) + ";" + String(carteSd.impulsions / RAPPORT_USVH) + ";" + String(position) + ";" + String(carteSd.altitude)); // le serveur envoi les données
}

//gestion du bouton forcer envoi de trames
void commandeSigfox() {
  boolean commande = 1;
  Serial.println("requete GET / commande sigfox");
  server.send ( 200, "text/plain", String(messages));      //lors de la commande manuelle, un retour du nombre de message est effectué
  xQueueSend(queueEnvoiBouton, &commande, portMAX_DELAY);  // envoi dans la file pour commander manuellement l'emission sigfox
  messages++;
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

/**
   @brief onRadiation
   @details Méthode nécessaire pour la récupération
   des radiations
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
   @brief onNoise
   @details Méthode nécessaire pour reconnaître une
   radiation érronée
*/
void onNoise()
{
  indiceRad.indice = indiceRad.indice + 1;
}


//Aquisition des mesures physiques
void sensorTask( void * pvParameters ) {
  boolean commande = false;
  Serial.print("Tache sensor execute sur le coeur N° ");
  Serial.println(xPortGetCoreID());

  positionRadiation trame1;
  positionMesures trame2;
  sauvegardeMSD carteSD;
  typeTrame alt = RADIATION;    //drapeau d'alternance des trames RADIATION ET MESURES
  trame1.id = 0;                //initialise les id des 2 trames
  trame2.id = 1;
  unsigned char minute, seconde, secondePrec;
  secondePrec = 0;
  while (1) {
    bmeGps.LireBme(); // MAJ du BME    
    rad.loop();
    if (bmeGps.TraitementGps() == true) {   //les données GPS sont-elles valides ?
      seconde = bmeGps.Seconde();
      minute = bmeGps.Minute();
      if (seconde != secondePrec) {         //changement de valeurs dans les secondes
        Serial.print(minute);
        Serial.print(':');
        Serial.println(seconde);
        if (seconde % 15 == 0)              //mémorisation carte mSD
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

          Serial.println("Envoi file pour Carte mSd.");
          xQueueSend(queueMSD, &carteSD, portMAX_DELAY); // envoi dans la file queueMSD
        }
        if (( minute % 2 == 0) && (seconde == 5)) {    //Envoi trame Sigfox toutes les 2 minutes et 5 secondes
          if (alt == RADIATION)
          {
            trame1.impulsions = indiceRad.impulsions;
            trame1.altitude = bmeGps.Altitude();
            trame1.longitude = bmeGps.Longitude();
            trame1.latitude = bmeGps.Latitude();
            Serial.println("Envoi file pour Sigfox données Radiation.");
            xQueueSend(queueSigfox, &trame1, portMAX_DELAY); // envoi via la file queueSigfox
            alt = MESURES;
          }
          else
          {
            trame2.temperature = bmeGps.Temperature();
            trame2.humidite = bmeGps.Humidite();
            trame2.pression = bmeGps.Pression();
            trame2.longitude = bmeGps.Longitude();
            trame2.latitude = bmeGps.Latitude();
            Serial.println("Envoi file pour Sigfox données Temp Hum Pression.");
            xQueueSend(queueSigfox, &trame2, portMAX_DELAY); // envoi via la file queueSigfox
            alt = RADIATION;
          }
        }
      }
      secondePrec = seconde;
    }
    xQueueReceive(queueEnvoiBouton, &commande, 0); //reception de la file EnvoiBouton avec TickType_t à 0, pour ne pas bloquer la queue de reception
    if (commande == true)
    {
      trame1.impulsions = indiceRad.impulsions;
      trame1.altitude = bmeGps.Altitude();
      trame1.longitude = bmeGps.Longitude();
      trame1.latitude = bmeGps.Latitude();
      Serial.println("Envoi file pour Sigfox données via le bouton.");
      xQueueSend(queueSigfox, &trame1, portMAX_DELAY);  // envoi dans la file
      commande = false;
    }
  }
}


//récupère les données de la file et envoie la trame Sigfox
void  sigfoxTask( void * pvParameters ) {
  bool retour;
  int nMessage = 0;
  byte octet;
  Serial.print("Tache sigfox execute sur le coeur N° ");
  Serial.println(xPortGetCoreID());
  byte *trame = (byte*)malloc(1 * sizeof(positionRadiation)); //allocation mémoire (12 octets : taille de la structure)

  while (1) {
    xQueueReceive(queueSigfox, trame, portMAX_DELAY); //reception de la file Sigfox
    Serial.println("queueSigfox Réception de la file>");
    //test d 'affichage pour voir si bonne réception des données de la file
    for (int i = 0; i < sizeof(positionRadiation); i++) {
      octet = *((byte*)trame + i);
      Serial.print(octet, HEX);
    }
    Serial.println();
#ifdef SIGFOX_MODEM_ON
    retour = modem.emission(trame, sizeof(positionRadiation));    //emission de la trame
    if (retour == true)
    {
      Serial.print("Message envoyé N°:");
      Serial.println(nMessage);
      nMessage++;
    } else
    {
      Serial.println("Erreur de connexion avec le modem");
    }
#else
    Serial.println("Emetteur Sigfox désactivé");
#endif
    delay(50);
  }
}

//Sauvegarde des données dans la msd
void  msdTask( void * pvParameters ) {
  Serial.print("Tache mSd execute sur le coeur N°  ");
  Serial.println(xPortGetCoreID());

  bool erreurCarte;
  bool erreurWrite;
  bool erreurAppend;
  char ligneCsv[70];
  erreurCarte = cartesd.begin();
  if (erreurCarte == false) {
    if (cartesd.fichierPresent(SD, NOM_FICHIER) == false) { //vérification de la présence du fichier "telemesures.txt"
      erreurWrite = cartesd.writeFile(SD, NOM_FICHIER, "Date;Heure;Latitude;Longitude;Altitude;Pression;Humidité;Température;Impulsions\n"); // ecriture des données en tete dans le fichier "telemesures.txt"
      if (erreurWrite == false) { //test si il n'y a pas d'erreur d'écriture
        Serial.println("données écrites");    // affichage des données dans le moniteurs série
      }
    }
  }
  else {
    Serial.println("Pas de carte présente");
  }
  while (1) {
    xQueueReceive(queueMSD, &carteSd, portMAX_DELAY); //reception de la file MSD
    Serial.println("queueMSD Réception de la file>");

    char horaire[32];
    //formattage de la date et l'heure  affichage dans le moniteur série pour vérification
    sprintf(horaire, "%02d/%02d/%02d %02d:%02d:%02d ", carteSd.date.jour, carteSd.date.mois, carteSd.date.annee, carteSd.heure.heure, carteSd.heure.minute, carteSd.heure.seconde);
    Serial.println(horaire);
    Serial.println(carteSd.latitude, 6);
    Serial.println(carteSd.longitude, 6);
    Serial.println(carteSd.altitude);
    Serial.println(carteSd.pression);
    Serial.println(carteSd.humidite);
    Serial.println(carteSd.temperature);
    Serial.println(carteSd.impulsions);
    //formattage de la date et l'heure  affichage dans le moniteur série pour vérification
    sprintf(ligneCsv, "%02d/%02d/%02d;%02d:%02d:%02d;%f;%f;%d;%d;%d;%2.1f;%d\n", carteSd.date.jour, carteSd.date.mois, carteSd.date.annee, carteSd.heure.heure, carteSd.heure.minute, carteSd.heure.seconde, carteSd.latitude, carteSd.longitude, carteSd.altitude, carteSd.pression, carteSd.humidite, carteSd.temperature, carteSd.impulsions);
    cartesd.remplaceCaractere(ligneCsv, strlen(ligneCsv), '.', ','); // remplace les "." par les "," pour le tableur
    Serial.print(ligneCsv);
#ifdef USE_SD_CARD_ON
    if (erreurCarte == false) {
      erreurAppend = cartesd.appendFile(SD, NOM_FICHIER, ligneCsv); //ajout des télémesures dans la carte mSd
      if (erreurAppend == false) {
        Serial.println(" données ajoutées");
      }
    }
    else {
      Serial.println("Pas de carte présente");
    }
#else
    Serial.println("Enregistrement désactivé");
#endif
    delay(50);
  }
}

void loop() {
  //gestion du serveur web
  server.handleClient();
  if (carteSd.altitude > ALTITUDE_MAX_WIFI) // si l'altitude est supérieur à 2000 mètres, coupure du wifi et interruption du serveur web
  {
    //server.stop();
    //WiFi.mode(WIFI_OFF);
  }
}