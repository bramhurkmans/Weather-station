/* Made by Bram, Ilse, Iain en Rowan
  IC15AO-A/B
  2017
*/

#include <Wire.h>
#include <LCD.h> //LCD library
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Addr, En, Rw, Rs, d4, d5, d6, d7, backlighpin, polarity

#include "DHT.h" //Temperature & Humidity sensor
#include <HMC5883L.h>

#define DHTPIN A0     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 (the type we use)
DHT dht(DHTPIN, DHTTYPE);

const int buttonPinBIIR = 2;     // the number of the pushbutton pin

int counterBIIR = 2;
int errorBIIR;

float oldHumBIIR;
float oldTempBIIR;

boolean lastBtnStateBIIR;
boolean btnStateBIIR;

void setup()
{
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.backlight();
  lcd.setCursor(0, 0);
  dht.begin();
  pinMode(buttonPinBIIR, INPUT);
}

void loop()
{

  readButtonBIIR();
  meassureInformationBIIR();

}

void readButtonBIIR()
{
  btnStateBIIR = digitalRead(buttonPinBIIR);

  if ( btnStateBIIR !=  lastBtnStateBIIR) {

    if ( btnStateBIIR == HIGH) {

      //   Serial.println("Button has been pressed");
      chooseInformationShown();
      lcd.clear();

    } else {
      //  Serial.println("Button has been released");
    }
    delay(50);
  }
  lastBtnStateBIIR =  btnStateBIIR;
}

void chooseInformationShown()
{
  switch (counterBIIR)
  {
    case 1:    //Temperature & Humidity

      Serial.println("Temperature & Humidity");
      lcd.clear();
      counterBIIR = 2;
      break;

    case 2:    //Barometer

      Serial.println("Barometer");
      lcd.clear();
      counterBIIR = 3;
      break;

    case 3:    //Compass

      Serial.println("Compass");
      lcd.clear();
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

      compassBIIR();

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
    lcd.print("Failed to read");
  }
  else
  {
    if (!(h == oldHumBIIR && t == oldTempBIIR))
    {
      lcd.clear();
      readButtonBIIR();

      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.print(" %\t");
      readButtonBIIR();

      lcd.print("Humidity: ");
      lcd.print(h);
      lcd.print(" %");
      readButtonBIIR();

      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.println(" *C");
      readButtonBIIR();

      lcd.setCursor(0, 2);
      lcd.print("Temperature: ");
      lcd.print(t);
      lcd.print(" C");
      readButtonBIIR();

      oldHumBIIR = h;
      oldTempBIIR = t;
    }
  }
}

void compassBIIR()
{

}

