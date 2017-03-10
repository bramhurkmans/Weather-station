/* Made by Bram, Ilse, Iain en Rowan
  IC15AO-A/B
  2017
*/

#include <math.h>
#include "Seeed_BMP280.h"
#include <HMC5883L.h>
#include <Wire.h>

/* TEMPERATURE VARS */
int aTemp;
float temperatureTemp;
int B = 3975; //B value of the thermistor
float resistance;


/*BAROMETER VARS*/
BMP280 bmp280;

  float pressure;

void setup() {
  Serial.begin(9600);
  if(!bmp280.init()){
    Serial.println("Device error!");
  }
}

void loop() {
  baroMeter();

  tempSensor();

  compassSensor();

  delay(1000);
}

void baroMeter() {
  //get and print temperatures
  Serial.print("Temp: ");
  Serial.print(bmp280.getTemperature());
  Serial.println("C"); // The unit for  Celsius because original arduino don't support speical symbols
  
  //get and print atmospheric pressure data
  Serial.print("Pressure: ");
  Serial.print(pressure = bmp280.getPressure());
  Serial.println("Pa");
  
  //get and print altitude data
  Serial.print("Altitude: ");
  Serial.print(bmp280.calcAltitude(pressure));
  Serial.println("m");
  
  Serial.println("\n");//add a line between output of different times.
}

void tempSensor() { //PORT A0
  aTemp = analogRead(0);
  resistance = (float)(1023 - aTemp) * 10000 / aTemp; //get the resistance of the sensor;
  temperatureTemp = 1 / (log(resistance / 10000) / B + 1 / 298.15) - 273.15; //convert to temperature via datasheet ;
  //delay(1000);
  Serial.print("Current temperature is ");
  Serial.println(temperatureTemp);

  return temperatureTemp;
}

void compassSensor() {

  //return compassValBiri;
}

