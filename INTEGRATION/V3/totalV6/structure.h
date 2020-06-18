#ifndef __STRUCTURE__
#define __STRUCTURE__

/**
 * @brief      Structure de données positionRadiation compatible Sigfox (12 octets) 
 * @details    Envoyer la position(longitude latitude), l'altitude et la radiation (cpm)
 */
typedef struct
{
  float latitude;   /*!< latitude en degrés décimaux*/
  float longitude;  /*!< longitude en degrés décimaux*/
  unsigned short altitude;  /*!< altitude en metres*/
  unsigned id : 1;  /*!< identifiant de la trame toujours égal à 0*/
  unsigned short  impulsions : 15;  /*!< impusions image de la radiation en cpm (coups par minutes)*/
} positionRadiation;

/**
 * @brief      Structure de données positionMesures compatible Sigfox (12 octets)
 * @details    Envoyer la position(longitude latitude), l'altitude et la radiation (cpm)
 */
typedef struct
{
  float latitude;   /*!< latitude en degrés décimaux*/
  float longitude;  /*!< longitude en degrés décimaux*/
  char temperature ; /*!< température en °C*/
  unsigned char humidite ; /*!< humidité en %HR*/
  unsigned id : 1;  /*!< identifiant de la trame toujours égal à 1*/
  unsigned short  pression : 15; /*!< pression en hPa*/
} positionMesures;

/**
 * @brief      Structure de données de la date
 * @details    jour/mois/année
 */

typedef struct {
  unsigned char jour; /*!< jour de 1 à 31*/
  unsigned char mois; /*!< mois de 1 à 12*/
  unsigned int annee; /*!< année format 20xx*/
} typeDate;

/**
 * @brief      Structure de données des heures
 * @details    heure/minutes/secondes
 */
typedef struct {
  unsigned char seconde; /*!< secondes de 0 à 59*/
  unsigned char minute; /*!< minutes de 0 à 59*/
  unsigned char heure;  /*!< heures de 0 à 23*/
} typeHeure;

/**
 * @brief      Structure de données sauvegardeMSD
 * @details    Liste toutes les données à enregister dans la carte mSd pendant le vol
 */
typedef struct
{
  float latitude;  /*!< latitude en degrés décimaux*/
  float longitude; /*!< longitude en degrés décimaux*/
  unsigned short altitude; /*!< altitude en metres*/
  unsigned short pression; /*!< pression en hPa*/
  unsigned char humidite; /*!< humidité en %HR*/
  float temperature; /*!< température en °C*/
  unsigned short impulsions; /*!< impusions image de la radiation en cpm (coups par minutes)*/
  typeHeure heure; /*!< heure */
  typeDate date; /*!< date */
} sauvegardeMSD;

typedef struct
{
  int indice;
  double impulsions;
  int a;
} lesIndices;

enum typeTrame
{
   RADIATION,
   MESURES 
};


#endif
