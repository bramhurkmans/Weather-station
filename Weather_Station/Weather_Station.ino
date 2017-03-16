/* Made by Bram, Ilse, Iain en Rowan
  IC15AO-A/B
  2017
*/

#include <Wire.h>
#include "rgb_lcd.h" //LCD library

#include "DHT.h"

#define DHTPIN A0     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 (the type we use)
DHT dht(DHTPIN, DHTTYPE);

const int buttonPinBIIR = 2;     // the number of the pushbutton pin
const int ledPinBIIR =  13;      // the number of the LED pin

int counterBIIR = 2;

boolean lastBtnStateBIIR;
boolean btnStateBIIR;

rgb_lcd lcd;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  dht.begin();
  pinMode(ledPinBIIR, OUTPUT);
  pinMode(buttonPinBIIR, INPUT);
}

void loop()
{

  readButtonBIIR();
  meassureInformationBIIR();

    if (Serial.available())
    {
      writeLcdBIIR();
    }

}

void readButtonBIIR()
{
  btnStateBIIR = digitalRead(buttonPinBIIR);

  if ( btnStateBIIR !=  lastBtnStateBIIR) {

    if ( btnStateBIIR == HIGH) {

      //   Serial.println("Button has been pressed");
      chooseInformationShown();

    } else {
      //  Serial.println("Button has been released");
    }
    delay(50);
  }
  lastBtnStateBIIR =  btnStateBIIR;
}

void writeLcdBIIR()
{
  delay(100);
  lcd.clear();
  while (Serial.available() > 0)
  {
    lcd.write(Serial.read());
  }
}

void chooseInformationShown()
{
  switch (counterBIIR)
  {
    case 1:    //Temperature & Humidity

      Serial.println("Temperature & Humidity");
      counterBIIR = 2;
      break;

    case 2:    //Barometer

      Serial.println("Barometer");
      counterBIIR = 3;
      break;

    case 3:    //Compass

      Serial.println("Compass");
      counterBIIR = 1;
      break;
  }
}

void meassureInformationBIIR()
{
  switch (counterBIIR)
  {
    case 2:    //Temperature & Humidity Void

      tempHumBIIR();

      break;

    case 3:    //Barometer Void


      break;

    case 1:    //Compass Void


      break;
  }
}

void tempHumBIIR()
{
  readButtonBIIR();
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h))
  {
    Serial.println("Failed to read from DHT");
  }
  else
  {
    if (counterBIIR == 2)
    {
      readButtonBIIR();
      Serial.print("Humidity: ");
      Serial.print(h);
      readButtonBIIR();
      Serial.print(" %\t");
      Serial.print("Temperature: ");
      readButtonBIIR();
      Serial.print(t);
      Serial.println(" *C");
    }
  }
}

