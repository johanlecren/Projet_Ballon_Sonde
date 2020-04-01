/*
  Gps + bme + radiation
*/

#include <HardwareSerial.h>
#include <TinyGPS.h>
#include <Wire.h>
#include "Adafruit_BME280.h"
#include "RadiationWatch.h"

HardwareSerial serialGps(2);
TinyGPS gps;
Adafruit_BME280 bme;
RadiationWatch radiationWatch(32, 33);

//structure pour la file sigfox
typedef struct
{
  float latitude;
  float longitude;
  unsigned short altitude;
  unsigned short  impulsions;
} sigfoxTrame;

//structure pour la file cartesd
typedef struct
{
  float latitude;
  float longitude;
  unsigned short altitude;
  unsigned short pression;
  unsigned char humidite;
  float temperature;
  byte heure[3];
  unsigned short impulsions;
} sauvegardeMSD;


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  serialGps.begin(4800, SERIAL_8N1, 16, 17);
  
  initBme();
    
  radiationWatch.setup();
  // Register the callbacks.
  radiationWatch.registerRadiationCallback(&onRadiation);
  radiationWatch.registerNoiseCallback(&onNoise);

}

// the loop function runs over and over again forever
void loop() {
  radiationWatch.loop();
  //traitementGps();
}

// BME-------------------------------------------------------------------

void printValuesBme() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");

  Serial.print("Pressure = ");

  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}


void initBme()
{
  Serial.println(F("BME280 test"));

  unsigned status;

  status = bme.begin(0x77);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1);
  }

}

// ¨GPS-------------------------------------------------------------------


void traitementGps()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  if (serialGps.available()) {
    char c = serialGps.read();
    if (gps.encode(c)) // Did a new valid sentence come in?
      newData = true;
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    int year;
    byte month, day, hour, minute, second, hundredths;
    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
    gps.f_get_position(&flat, &flon, &age);
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ", month, day, year, hour, minute, second);
    Serial.println(sz);
    Serial.print("Latitude=");
    Serial.println(flat, 6);
    Serial.print("Longitude=");
    Serial.println(flon, 6);
    Serial.print("Altitude=");
    Serial.println(gps.f_altitude());
    //Serial.println();
    printValuesBme();
  }

  gps.stats(&chars, &sentences, &failed);
  if (chars > 0 && sentences == 0) {
    Serial.println("gpsACQ Wait");
    //digitalWrite(LED, HIGH);
  }
  if (chars == 0) {
    Serial.println("** pas de trame nmea recue vérifier le cablage du gps **");
    //digitalWrite(LED, HIGH);
  }
}

// RADIATION-------------------------------------------------------------------

void onRadiation()
{

  Serial.println("Un rayon gamma est apparu");
  Serial.print(radiationWatch.uSvh());
  Serial.print(" uSv/h +/- ");
  Serial.println(radiationWatch.uSvhError());
  Serial.print(" CPM : ");
  Serial.println(radiationWatch.cpm());
}

void onNoise()
{
  Serial.println("Argh, bruit, SVP arreter de bouger");
}
