#ifndef __SIMULATION__
#define __SIMULATION__
 /**
  * @file       simulation.h
  * @brief      Définition de la classe Simulation
  * @version    1.1
  * @author     Johan Le Cren
  * @date       20 avril 2020
*/

#include "Arduino.h"
#include "structure.h"

/**
 * @brief      Facilite la machine à état gérant la simulation du ballon
 * @details    3 valeurs sont nécessaires afin d'indiquer la montée, la descente et l'arrêt du ballon
 */
enum etatBallon
{
   MONTE,   /*!< Le ballon en en phase de montée*/
   DESCENDS,  /*!< Le ballon en en phase de descente*/
   ARRET    /*!< Le ballon est posé au sol fin de vol*/
};

/**
 * @brief      Identifie le type de trame Sigfox positionRadiation ou positionMesures
 * @details    permet de savoir quel type de trame on doit envoyer
 */
enum typeTrame
{
   RADIATION, /*!< tramepositionRadiation sélectionnée*/
   MESURES /*!< positionMesures sélectionnée*/
};

class Simulation
{
  public:
    Simulation(unsigned int _annee, char _mois, char _jour, char _heure, char _minute, float _latitude, float _longitude, short _altitude, short _pression, float _temperature, char _humidite, short _impulsion, int _eclatement, int _vitesse_asc, int _vitesse_sol, int _intervalle);
    void gestion();
    void getTelemesures(byte *trame);
    void getSauvegarde(byte *carte);
    void afficheTrame(byte *trame,int taille);
  private:
    positionMesures pm;
    positionRadiation pr;
    sauvegardeMSD carteSD;
    float pas_deplacement;
    int intervalle;
    float pas_pression;
    float pas_humidite;
    float pas_temperature;
    float pas_impulsion;
    float pression;
    float humidite;
    float temperature;
    float impulsions;    
    int vitesse_asc;
    int eclatement;
    short altitudeMin;
    typeTrame alt;
    etatBallon direction_ballon;
};

#endif
