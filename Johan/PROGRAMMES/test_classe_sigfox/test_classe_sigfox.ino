//Simulation d'envoi de trame avec
//2 trames alternées positionRadiation et positionMesures

#define TEMPO 120 // 2 minutes

#include "sigfox.h"

typedef struct
{
  float latitude;
  float longitude;
  unsigned short altitude;
  unsigned short  impulsions : 15;
  unsigned id : 1;
} positionRadiation;

typedef struct
{
  float latitude;
  float longitude;
  char temperature ;
  unsigned char humidite ;
  unsigned short  pression : 15;
  unsigned id : 1;
} positionMesures ;


Sigfox modem(27, 26, true);


void setup() {
  Serial.begin(115200);
}

void loop() {
  positionRadiation pr;
  positionMesures pm;
  pr.latitude =   47.994961;
  pr.longitude =  0.204593;
  pr.altitude =  100;
  pr.impulsions = 50;
  pr.id = 0;
  pm.latitude = 47.994961;
  pm.longitude = 0.204593;
  pm.pression = 1020;
  pm.humidite = 70;
  pm.temperature = 25;
  pm.id = 1;

  bool retour;
  int nMessage;
  boolean id = false;

  Serial.print(sizeof(pr));

  for (nMessage = 0; nMessage < 140; nMessage++) {
    if (id == false) {
      retour = modem.emission(&pr, sizeof(pr));
      id = true;
    }
    else
    {
      retour = modem.emission(&pm, sizeof(pm));
      id = false;
    }
    if (retour == true)
    {
      Serial.print("Message ");
      Serial.print(nMessage);
      Serial.print(" envoyé avec id :");
      Serial.println(id);
    } else
    {
      Serial.println("Erreur de connexion avec le modem");
    }
    Serial.print("Attente 2 minutes");
    delay(TEMPO * 1000L);
  }
  Serial.println("140 messages envoyés");
  while (1) {}
}
