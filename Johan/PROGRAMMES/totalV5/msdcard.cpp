/**
  * @file       msdcard.cpp
  * @brief      Implémentation de la classe Msdcard
  *             Détection de la présence d'une carte mSd
  *             opération de création et d'écriture dans un fichier texte
  *             opération d'ajout de données dans un fichier texte
  * @version    1.1
  * @author     Johan Le Cren
  * @date       20 avril 2020
  */

#include "msdcard.h"

/**
 * @brief   Msdcard::Msdcard
 * @details Constructeur de la classe Msdcard, initialise chaque attribut du constructeur
 * @param   sckPin  broche d'horloge sck (sortie)
 * @param   misoPin  broche master input slave output (entrée)
 * @param   mosiPin  broche master output slave input (sortie)
 * @param   ssPin  broche de sélection de la carte SD (sortie)
 */


Msdcard::Msdcard(uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin, uint8_t ssPin) {
  //SPI.begin(14, 2, 15, 13); //SCK,MISO,MOSI,ss
  SD_CS = ssPin;
  SPI.begin(sckPin, misoPin, mosiPin, ssPin); //SCK,MISO,MOSI,ss
  delay(10);
}

/**
 * @brief   Msdcard::begin
 * @details Méthode de la classe Msdcard, initialise les paramètres de la liaison SPI
 *          vérifie la présence de la carte mSD
 *          Affiche le type de carte mSD présente
 *          Affiche la taille de la carte mémoire
 * @return  retourne un booleen à vrai si erreur (pas de carte mSD)
 */

bool Msdcard::begin() {
  bool erreurCarte = false;

  if (!SD.begin(SD_CS, SPI, 40000000, "/sd")) {   //initialise l'accès à la carte mSd
    Serial.println("échec du montage de la carte SD");
    erreurCarte = true;
  }
  else {
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
      Serial.println("Pas de carte SD branchée");
      erreurCarte = true;
    }
    else {
      switch (cardType) {                       //identifie le type de carte
        case CARD_MMC: Serial.println("MMC");
          break;
        case CARD_SD: Serial.println("SDSC");
          break;
        case CARD_SDHC:
          Serial.println("SDHC");
          break;
      }
      uint64_t cardSize = SD.cardSize() / (1024 * 1024);      //affiche la taille en Mo
      Serial.printf("Taille de la carte SD: %lluMB\n", cardSize);
    }
  }
  return erreurCarte;
}


/**
 * @brief   Msdcard::fichierPresent
 * @details Méthode de la classe Msdcard, vérifie si le fichier de sauvegarde est présent ou non
 * @param   &fs adresse de la classe file system
 * @param   *path chemin et nom du fichier à tester
 * @return  retourne un booleen à vrai si le fichier existe sur la carte SD
 */

bool Msdcard::fichierPresent(fs::FS &fs, const char * path) {
  bool retour;
  File file = fs.open(path);
  if (!file) {
    Serial.println("pas de fichier");
    retour = false;
  }
  else {
    Serial.println("fichier existe");
    retour = true;
    file.close();
  }

  return retour;
}

/**
 * @brief   Msdcard::writeFile
 * @details Méthode de la classe Msdcard, écrase les données existantes dans le fichier et les remplaces
 * @param   &fs adresse de la classe file system
 * @param   *path chemin et nom du fichier en écriture
 * @param   *message chaine de caratères à écrire dans le fichier
 * @return  retourne un booleen à vrai si erreur d'écriture
 */
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

/**
 * @brief   Msdcard::appendFile
 * @details Méthode de la classe Msdcard, ajoute des données dans le fichier à la suite sans écraser les données précédentes
 * @param   &fs adresse de la classe file system
 * @param   *path chemin et nom du fichier en ajout de données
 * @param   *message chaine de caratères à ajouter dans le fichier
 * @return  retourne un booleen à vrai si erreur d'écriture
 */
bool Msdcard::appendFile(fs::FS & fs, const char * path, const char * message) {
  bool erreurAppend = false;
  Serial.printf("Ajouter au fichier: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Echec de l'ouverture du fichier pour ajout de données");
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

/**
 * @brief   Msdcard::remplaceCaractere
 * @details Méthode de la classe Msdcard, remplace un caractére par un autre (point par une virgule)
 * @param   ligneCsv[]  chaine de caratères traiter
 * @param   longueur    longueur de la chaine
 * @param   carSource   caractère à rechercher dans la chaine
 * @param   carDest     caractère à remplacer dans la chaine
 */
void Msdcard::remplaceCaractere(char ligneCsv[], int longueur, char carSource, char carDest)
{
  int n;
  for (n = 0; n < longueur; n++) 
  {
    if (ligneCsv[n] == carSource) //cherche les "."
    {
      ligneCsv[n] = carDest;  // remplace par les ","
    }
  }

}
