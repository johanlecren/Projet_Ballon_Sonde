/**
    @file       simulation.cpp
    @brief      Implémentation de la classe Simulation
                Simule le vol complet du ballon du décollage jusqu'a l'atterrissage
                variation linéaire des paramètres physique
                ne tient pas compte des modèles de courbes réelles
                le simulateur pret de vérifier le déroulement de l'ensemble:
                gestion des files (queues):
                Sauvegarde dans la carte SD;
                Emission télémesures Sigfox.
    @version    1.1
    @author     Johan Le Cren
    @date       20 avril 2020
*/

#include "simulation.h"

/**
   @brief   Simulation::Simulation
   @details Constructeur de la classe Simulation, initialise chaque attribut du constructeur
            calcul les intervalles de déplacement(pas), pression , température, humidité, impulsion
            en fonction de l'altitude d'éclatement et de l'intervalle entre chaque mesure
            inititialise la position du baloon la date et l'heure dans la structure dédié a la carte sd
   @param   _annee  la date de lancement (reste fixe)
   @param   _mois
   @param   _jour
   @param   _heure  heure de lancement
   @param   _minute
   @param   _seconde  toujours à zéro
   @param   _latitude latitude du ballon au départ
   @param   _longitude latitude du ballon au départ
   @param   _altitude  altitude du ballon sur l'aire de lancement
   @param   _pression  pression au sol
   @param   _temperature  température au sol
   @param   _humidite   humidité au sol
   @param   _impulsion  nombre de cpm au sol
   @param   _eclatement  altitude d'éclatement
   @param   _vitesse_asc  vitesse ascensionnelle
   @param   _vitesse_sol  vitesse de déplacement par rapport au sol
   @param   _intervalle   intervalle entre chaque mesures (secondes)
*/

Simulation::Simulation(unsigned int _annee, char _mois, char _jour, char _heure, char _minute, float _latitude, float _longitude, short _altitude, short _pression, float _temperature, char _humidite, short _impulsion, int _eclatement, int _vitesse_asc, int _vitesse_sol, int _intervalle)
{
  pression = _pression;
  humidite = _humidite;
  temperature = _temperature;
  impulsions = _impulsion;
  intervalle = _intervalle;
  vitesse_asc = _vitesse_asc;
  eclatement = _eclatement;
  int nbMesuresMontee;
  nbMesuresMontee = (eclatement - altitudeMin) / (vitesse_asc * intervalle); //calcul du nombre de mesures
  pas_deplacement = (float) (_vitesse_sol * intervalle) / (60 * 1852); // calcul du pas de déplacement
  pas_pression = (float) 1020/ nbMesuresMontee; //calcul du pas de la pression
  pas_temperature = (temperature + 10) / nbMesuresMontee; //calcul de pas de la température
  pas_humidite = (float) humidite / nbMesuresMontee; //calcul du pas de l'humidité
  pas_impulsion = (float) (300 - impulsions) / nbMesuresMontee; //calcul du pas des implusions
  direction_ballon = MONTE; // indique la direction du ballon
  carteSD.latitude = _latitude;
  carteSD.longitude = _longitude;
  carteSD.altitude = _altitude;
  carteSD.heure.heure = _heure;
  carteSD.heure.minute = _minute;
  carteSD.heure.seconde = 0;
  carteSD.date.jour = _jour;
  carteSD.date.mois = _mois;
  carteSD.date.annee = _annee;
  altitudeMin = _altitude;
  pr.id = 0;
  pm.id = 1;
}
/**
   @brief   Simulation::gestion
   @details Méthode de la classe Simulation,
            gère l'incrémentation de heure, la variation de la pression, humidité, température
            le déplacement du ballon (pas défaut toujours au nord ouest)
            test l'attitude max et inverse le sens de déplacement (descente)
*/

void Simulation::gestion()
{

  //gestion de heure
  delay(intervalle * 1000);
  carteSD.heure.seconde = carteSD.heure.seconde + intervalle;
  if (carteSD.heure.seconde == 60)
  {
    carteSD.heure.minute = carteSD.heure.minute + 1;
    carteSD.heure.seconde = 0;
    if (carteSD.heure.minute == 60)
    {
      carteSD.heure.heure = carteSD.heure.heure + 1 ;
      carteSD.heure.minute = 0;
      if (carteSD.heure.heure == 24)
      {
        carteSD.heure.heure = 0;
      }
    }
  }
  //cas ou le ballon monte
  if (direction_ballon == MONTE )
  {
    carteSD.latitude += pas_deplacement;
    carteSD.longitude +=  pas_deplacement;
    carteSD.altitude += (vitesse_asc * intervalle);
    pression = pression - pas_pression;
    temperature = temperature - pas_temperature;
    humidite = humidite - pas_humidite;
    impulsions = impulsions + pas_impulsion;
    if (carteSD.altitude > eclatement) //le ballon a atteint son altitude d'éclatement, inversion du sens
    {
      direction_ballon = DESCENDS;
    }
  }
  else if (direction_ballon == DESCENDS) //le ballon descend
  {
    carteSD.latitude += pas_deplacement;
    carteSD.longitude +=  pas_deplacement;
    carteSD.altitude -= (vitesse_asc * intervalle);
    pression = pression + pas_pression;
    temperature = temperature + pas_temperature;
    humidite = humidite + pas_humidite;
    impulsions = impulsions - pas_impulsion;
    if (carteSD.altitude < altitudeMin) //le ballon a atteint le sol arret de l'évolution des mesures.
    {
      direction_ballon = ARRET;
    }
  }
}

/**
   @brief   Simulation::getTelemesures
   @details Méthode de la classe Simulation, récupère les données des télémesures pour la trame Sigfox
            dans le cas ou on envoie la position/ radiation
            et dans le cas ou on envoie la position/température/humidité/pression
            inversion entre la position/ radiation et position/température/humidité/pression
   @param   *trame pointeur sur une mémoire de 12 octets
*/

void Simulation::getTelemesures(byte *trame) {
  // met a jour la structure  pm et pr a partir de carte SD
  if (alt == RADIATION) //cas position/ radiation
  {
    pr.latitude = carteSD.latitude; //mise a jour
    pr.longitude = carteSD.longitude; //mise a jour
    pr.altitude = carteSD.altitude; //mise a jour
    pr.impulsions = impulsions; //mise a jour
    alt = MESURES;  //inversion pour le prochain appel de la méthode
    memcpy(trame, &pr, sizeof(positionRadiation));  //copie la structure dans la mémoire trame
    Serial.println("****************************"); //affichage de vérification
    Serial.println("Structure trame position radiation");
    Serial.print("latitude: ");
    Serial.println(pr.latitude, 6);
    Serial.print("longitude: ");
    Serial.println(pr.longitude, 6);
    Serial.print("altitude: ");
    Serial.println(pr.altitude);
    Serial.print("impulsion: ");
    Serial.println(pr.impulsions);
  }
  else {
    if (alt == MESURES) //cas position/température/humidité/pression
    {
      pm.latitude = carteSD.latitude; //mise a jour
      pm.longitude = carteSD.longitude; //mise a jour
      pm.temperature = (char) temperature; //mise a jour
      pm.humidite = humidite; //mise a jour
      pm.pression = pression; //mise a jour
      alt = RADIATION;  //inversion pour le prochain appel de la méthode
      memcpy(trame, &pm, sizeof(positionMesures)); //copie la structure dans la mémoire trame
      Serial.println("-------------------------------"); //affichage de vérification
      Serial.println("Structure trame position télémesures");
      Serial.print("latitude: ");
      Serial.println(pm.latitude, 6);
      Serial.print("longitude: ");
      Serial.println(pm.longitude, 6);
      Serial.print("température: ");
      Serial.println((int)pm.temperature);
      Serial.print("humidité: ");
      Serial.println(pm.humidite);
      Serial.print("pression: ");
      Serial.println(pm.pression);
    }
  }
}
/**
   @brief   Simulation::getSauvegarde
   @details Méthode de la classe Simulation, récupère les données des télémesures pour la carte Sd
   @param   *carte
*/

void Simulation::getSauvegarde(byte *carte) {
  //met a jour la structure  carte sd
  carteSD.impulsions = (unsigned short) impulsions; //mise a jour de la structure
  carteSD.temperature = temperature; //mise a jour de la structure
  carteSD.humidite = (unsigned char) humidite; //mise a jour de la structure
  carteSD.pression = (unsigned short) pression; //mise a jour de la structure
  memcpy(carte, &carteSD, sizeof(sauvegardeMSD)); //copie la structure carteSD dans la mémoire carte
  //affiche le contenu de la structure
  Serial.println("++++++++++++++++++++++++++++++"); //affichage de vérification
  Serial.println("Structure carte SD");
  Serial.print("jour: ");
  Serial.println(carteSD.date.jour);
  Serial.print("mois: ");
  Serial.println(carteSD.date.mois);
  Serial.print("année: ");
  Serial.println(carteSD.date.annee);
  Serial.print("heure: ");
  Serial.println(carteSD.heure.heure);
  Serial.print("minute: ");
  Serial.println(carteSD.heure.minute);
  Serial.print("seconde: ");
  Serial.println(carteSD.heure.seconde);
  Serial.print("latitude: ");
  Serial.println(carteSD.latitude, 6);
  Serial.print("longitude: ");
  Serial.println(carteSD.longitude, 6);
  Serial.print("altitude: ");
  Serial.println(carteSD.altitude);
  Serial.print("impulsion: ");
  Serial.println(carteSD.impulsions);
  Serial.print("température: ");
  Serial.println(carteSD.temperature);
  Serial.print("humidité: ");
  Serial.println(carteSD.humidite);
  Serial.print("pression: ");
  Serial.println(carteSD.pression);
}

/**
   @brief   Simulation::afficheTrame
   @details Méthode de la classe Simulation, affiche la trame en hexadécimal
            pour vérification avant envoi dans la file (cohérence des données)
   @param   *trame
   @param   taille
*/
void Simulation::afficheTrame(byte *trame, int taille) {
  byte octet;
  for (int i = 0; i < taille; i++) {
    octet = *((byte*)trame + i);
    Serial.print(octet, HEX); //affiche dans le moniteur série la trame en hexa
    Serial.print(','); // chaque octet séparé par un virgule 
  }
  Serial.println();
}
