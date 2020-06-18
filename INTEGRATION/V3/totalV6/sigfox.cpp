/**
  * @file       sigfox.cpp
  * @brief      Implémentation de la classe Sigfox
  *             Assure la conversion des octets en chaine de caractères hexadécimal
  *             Réalise l'émission radio vers le modem Sigfox en commande AT
  *             Vérifie que la trame est bien envoyée
  * @version    1.1
  * @author     Johan Le Cren
  * @date       20 avril 2020
  */


#include "sigfox.h"

/**
 * @brief   Sigfox::Sigfox
 * @details Constructeur de la classe Sigfox, initialise la liaison série
 * @param   rxPin broche de réception série rxPin par défaut 27
 * @param   txPin broche d'émission série  txPin par défaut 26
 * @param   debugEn à true validation de l'affichage du débuggage
  */

Sigfox::Sigfox (uint8_t rxPin = 27 , uint8_t txPin = 26, bool debugEn = true) {
  serialSigfox = new HardwareSerial(1);
  rx = rxPin;
  tx = txPin;
  debug = debugEn;
  serialSigfox->begin(9600, SERIAL_8N1, rx, tx);
}

/**
 * @brief   Sigfox::conversionBinaireHexa
 * @details conversion d'un octet en chaine de caractère hexadécimal
 * @param   octet a convertir
 * @return  retourne une chaine de caractère (octet en hexadécimal)
 */

String Sigfox::conversionBinaireHexa(byte octet)
{
  //48   49
  char table[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'}; //initialisation d'un tableau contenant toutes les possibilités hexadécimales
  String resultat;
  byte msb = octet / 16;    //récupére les 4 bits de poids fort
  byte lsb = octet & 0x0F;  //récupére les 4 bits de poids faible
  //Serial.println(table[msb]);
  //Serial.println(table[lsb]);
  resultat = String(table[msb]) + String(table[lsb]);   //concatenation des deux caractères
  if (debug)
  {
    Serial.print(resultat);    
  }
  return resultat;
}

/**
 * @brief   Sigfox::emission
 * @details Méthode de la classe Sigfox, émet la trame Sigfox contenant les différentes données des télémesures
 * @param   *trame pointeur sur la mémoire des données à transmettre
 * @param   size nombre d'octets a transmettre
 * @return  retourne vrai si la trame a été transmise
 */

bool Sigfox::emission(const void* trame,  uint8_t size)
{
  uint8_t* bytes = (uint8_t*)trame;
  byte octet;
  int i;
  serialSigfox->print("AT$SF=");
  for (i = 0; i < size; i++) {    //balaie tous les octets de la trame
    octet = bytes[i];
    serialSigfox->print(conversionBinaireHexa(octet)); // conversion d'octet en chaine et envoi dans la ligne serie
  }
  serialSigfox->print("\n");    //la trame est terminée fin de ligne
  return testOK();              //vérifie l'accusé de réception du modem OK
}

/**
 * @brief   Sigfox::testOK
 * @details Méthode de la classe Sigfox, vérifie si le répond ok suite a l'envoi d'une commande
 * @return  retourne vrai si l'on recoit bien OK sur la ligne Série en provenance du modem Sigfox
 */

bool Sigfox::testOK()
{
  bool retour = false;
  String acq = "";
  char caractere;
  int compteur = 0;
  while (!serialSigfox->available() && compteur < 100) {    // test si le port serie Sigfox n'est pas disponible et si le compteur est inférieur à 100
    delay(100);                                             // boucle non bloquante timeout de 10 secondes
    compteur++;     //incrémentation du compteur
    //Serial.println(compteur);
  }
  while (serialSigfox->available()) {
    caractere = serialSigfox->read();
    if ((caractere != 0x0A) && (caractere != 0x0D)) {      //0x0A Line feed | 0x0D Carriage return
      acq += caractere;                                   //concatene la chaine avec les caractères recus de la ligne serie
    }
    delay(10);
  }
  if (acq.indexOf("OK") >= 0) {       // test si il y a bien ok dans la chaine
    retour = true;
  }
  return retour;
}
