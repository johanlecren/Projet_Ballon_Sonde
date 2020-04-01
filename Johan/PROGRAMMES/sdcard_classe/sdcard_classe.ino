/*
   test de la carte sd
   formatage pour ecrire une ligne d'un fichier csv
   rappel  sprintf
   http://www.cplusplus.com/reference/cstdio/sprintf/
*/

typedef struct {
  unsigned char jour;
  unsigned char mois;
  int annee;
}typeDate;

typedef struct {
  unsigned char seconde;
  unsigned char minute;
  unsigned char heure;
}typeHeure;


typedef struct
{
  float latitude;
  float longitude;
  unsigned short altitude;
  unsigned short pression;
  unsigned char humidite;
  float temperature;
  unsigned short impulsions;
  typeHeure heure;
  typeDate date;  
} sauvegardeMSD;



#include "msdcard.h"

Msdcard cartesd(14, 2, 15, 13); //SCK,MISO,MOSI,ss

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  bool erreurCarte;
  bool erreurWrite;
  bool erreurAppend;
  erreurCarte = cartesd.begin();
  if (erreurCarte == false) {
    erreurWrite = cartesd.writeFile(SD, "/telemesures.txt", "Hello ");
    if (erreurWrite == false) {
      Serial.println("données écrites");
    }
    erreurAppend = cartesd.appendFile(SD, "/telemesures.txt", "World!\n");
    if (erreurAppend == false) {
      Serial.println("données ajoutées");
    }
  }

  /*
    //heure UTC;latitude;longitude;altitude;pression;humidite;temperature;impulsions
    //08:13:34;47,6036;-2,3574;1200;850;45;5,3;50
    sauvegardeMSD carteSd;
  carteSd.latitude =  47.994961; 
  carteSd.longitude = 0.204593;
  carteSd.altitude = 50;
  carteSd.pression = 1020;
  carteSd.humidite = 70;
  carteSd.temperature = 25;
  carteSd.heure.seconde = 34;
  carteSd.heure.minute = 45;
  carteSd.heure.heure = 13;
  carteSd.date.jour = 18;
  carteSd.date.mois = 06;
  carteSd.date.annee = 2020;
  carteSd.impulsions = 1;

    char ligneCsv[70];
    //sprintf(ligneCsv, "%02d/%02d/%02d %02d:%02d:%02d ",carteSd.date.jour,carteSd.date.mois,carteSd.date.annee,carteSd.heure.heure,carteSd.heure.minute,carteSd.heure.seconde);
    sprintf(ligneCsv,"%d:%d:%d;%f;%f;%d;%d;%d;%2.1f;%d\n",telemesures.heure[0],telemesures.heure[1],telemesures.heure[2],telemesures.latitude,telemesures.longitude,telemesures.altitude,telemesures.pression,telemesures.humidite,telemesures.temperature,telemesures.impulsions);

    Serial.print(ligneCsv);
  */
}

void loop() {
  // put your main code here, to run repeatedly:

}
