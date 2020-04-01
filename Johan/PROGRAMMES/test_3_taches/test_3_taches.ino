//version 3 taches avec 2 files (capteurs, sigfox, carte SD)

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



TaskHandle_t Tasksensor;
TaskHandle_t Tasksigfox;
TaskHandle_t Taskmsd;

QueueHandle_t queueSigfox;
QueueHandle_t queueMSD;

int queueSize = 10;

void setup() {
  Serial.begin(115200);
  queueSigfox = xQueueCreate( queueSize, sizeof(positionRadiation));
  queueMSD = xQueueCreate( queueSize, sizeof( sauvegardeMSD ));

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
    sensorTask,   /* Task function. */
    "sensorTask",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Tasksensor,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
  delay(500);
  Serial.println("Tâche sensorTask créée...");

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
    sigfoxTask,   /* Task function. */
    "sigfoxTask",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Tasksigfox,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */
  Serial.println("Tâche sigfoxTask créée...");
  delay(500);
  xTaskCreatePinnedToCore(
    msdTask,   /* Task function. */
    "msdTask",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    2,           /* priority of the task */
    &Taskmsd,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */
  Serial.println("Tâche msd Task créée...");
  delay(500);

}

//simule l'envoi des mesures dans les 2 files
void sensorTask( void * pvParameters ) {
  Serial.print("Task sensor running on core ");
  Serial.println(xPortGetCoreID());

  positionRadiation pr;
  positionMesures pm;
  sauvegardeMSD carteSd;

  carteSd.latitude =  47.994961;  //pour carte sd
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
  pr.latitude = 47.994961;
  pr.longitude = 0.204593;
  pr.altitude = 50;
  pr.impulsions = 1;
  pr.id = 0;
  pm.latitude = 47.994961;
  pm.longitude = 0.204593;
  pm.pression = 1020;
  pm.humidite = 70;
  pm.temperature = 25;
  pm.id = 1;

  while (1) {
    Serial.println("Envoi file pour carteSD");    
    xQueueSend(queueMSD, &carteSd, portMAX_DELAY);
    delay(1000);
    Serial.println("Envoi file pour Sigfox données position radiation ");    
    xQueueSend(queueSigfox, &pr, portMAX_DELAY);
    delay(1000);
    Serial.println("Envoi file pour Sigfox données position temperature/humidite/pression ");    
    xQueueSend(queueSigfox, &pm, portMAX_DELAY);
    delay(1000);
  }
}

//récupère les données de la file et envoie la trame Sigfox
void  sigfoxTask( void * pvParameters ) {
  Serial.print("Task sigfox running on core ");
  Serial.println(xPortGetCoreID());
  byte *trame = (byte*)malloc(1 * sizeof(positionRadiation));
 
  while (1) {
    xQueueReceive(queueSigfox, &trame, portMAX_DELAY);
    Serial.println("Tache sigfox trame recue");
    //test d 'affichage pour voir
    byte octet;
    for (int i = 0; i < sizeof(positionRadiation); i++) {
      octet = *((byte*)&trame + i);
      Serial.print(octet, HEX);
    }
    Serial.println();
    delay(50);
  }
}

//Sauvegarde des données dans la msd
void  msdTask( void * pvParameters ) {
  Serial.print("Task msd running on core ");
  Serial.println(xPortGetCoreID());
  sauvegardeMSD carteSd;
  while (1) {
    xQueueReceive(queueMSD, &carteSd, portMAX_DELAY);
    Serial.println("télémesures reçues");
    char horaire[32];
    sprintf(horaire, "%02d/%02d/%02d %02d:%02d:%02d ",carteSd.date.jour,carteSd.date.mois,carteSd.date.annee,carteSd.heure.heure,carteSd.heure.minute,carteSd.heure.seconde);
    Serial.println(horaire);
    Serial.println(carteSd.latitude, 6);
    Serial.println(carteSd.longitude, 6);
    Serial.println(carteSd.altitude);
    Serial.println(carteSd.pression);
    Serial.println(carteSd.humidite);
    Serial.println(carteSd.temperature);
    Serial.println(carteSd.impulsions);

    delay(50);
  }
}


void loop() {

}
