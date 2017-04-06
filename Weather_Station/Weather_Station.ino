/* Made by Bram, Ilse, Iain en Rowan
  IC15AO-A/B
  2017
*/

#include "Seeed_BMP280.h"
#include <Wire.h>
#include <LCD.h> //LCD library
#include <LiquidCrystal_I2C.h> // i²c library
#include <HMC5883L_Simple.h> // Compass heading library

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Addr, En, Rw, Rs, d4, d5, d6, d7, backlighpin, polarity

#include "DHT.h" //Temperature & Humidity sensor
#define address 0x1E //0011110b, I2C 7bit address of HMC5883
#define DHTPIN A0     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 (the type we use)

HMC5883L_Simple Compass; // Compass
DHT dht(DHTPIN, DHTTYPE); //Humidity + temperature
BMP280 bmp280; //Barometer

const int buttonPinBIIR = 2;     // the number of the pushbutton pin

int counterBIIR = 2;
int errorBIIR;

float oldHumBIIR;
float oldTempBIIR;

boolean lastBtnStateBIIR;
boolean btnStateBIIR;

boolean printBarometerBIIR = false;
boolean printCompassBIIR = false;

float oldXBIIR = 100;
float oldYBIIR = 100;
float oldZBIIR = 100;

float compassHeadingBIIR;
float oldCompassHeadingBIIR = 100;

float oldBarTempBIIR = 100;
float oldPressBIIR = 100;
float oldAltBIIR = 100;

float newBarTempBIIR;
float newBarPressBIIR;
float newBarAltBIIR;

int HeadingBIIR;

int compassXBIIR, compassYBIIR, compassZBIIR; //triple acompassXBIIRis data

byte NBIIR[8] = { //N
  B00000,
  B00100,
  B01110,
  B11111,
  B01110,
  B01110,
  B01110,
  B01110
};

byte NeBIIR[8] = { //N-E
  B00000,
  B00000,
  B00000,
  B00001,
  B00110,
  B01110,
  B11100,
  B11000
};

byte EBIIR[8] = {  //E
  B00000,
  B00000,
  B00010,
  B11111,
  B11111,
  B00010,
  B00000,
  B00000
};

byte SeBIIR[8] = {  //S-E
  B11000,
  B11100,
  B01110,
  B00110,
  B00001,
  B00000,
  B00000,
  B00000
};
byte SBIIR[8] = {  //S
  B01110,
  B01110,
  B01110,
  B01110,
  B11111,
  B01110,
  B00100,
  B00000
};

byte SwBIIR[8] = {   //S-W
  B00011,
  B00111,
  B01110,
  B01100,
  B10000,
  B00000,
  B00000,
  B00000
};

byte WBIIR[8] = {   //W
  B00000,
  B00000,
  B01000,
  B11111,
  B11111,
  B01000,
  B00000,
  B00000
};

byte NwBIIR[8] = {  //N-W
  B00000,
  B00000,
  B00000,
  B10000,
  B01100,
  B01110,
  B00111,
  B00011
};


void setup()
{
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.backlight();
  lcd.setCursor(0, 0);
  dht.begin();
  pinMode(buttonPinBIIR, INPUT);
  Wire.begin();

  lcd.createChar(1, NBIIR);
  lcd.createChar(2, NeBIIR);
  lcd.createChar(3, EBIIR);
  lcd.createChar(4, SeBIIR);
  lcd.createChar(5, SBIIR);
  lcd.createChar(6, SwBIIR);
  lcd.createChar(7, WBIIR);
  lcd.createChar(8, NwBIIR);

  //Setup compass heading
  Compass.SetDeclination(23, 35, 'E');
  Compass.SetSamplingMode(COMPASS_SINGLE);
  Compass.SetScale(COMPASS_SCALE_130);
  Compass.SetOrientation(COMPASS_HORIZONTAL_X_NORTH);
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
      oldXBIIR = 100;
      oldYBIIR = 100;
      oldZBIIR = 100;
      oldCompassHeadingBIIR = 100;

      Serial.println("Temperature & Humidity");

      oldHumBIIR = 0;
      oldTempBIIR = 0;
      lcd.clear();
      counterBIIR = 2;
      break;

    case 2:    //Barometer
      oldHumBIIR = 100;
      oldTempBIIR = 100;

      Serial.println("Barometer");

      lcd.clear();
      printBarometerBIIR = false;

      if (!bmp280.init()) {
        Serial.println("Device error!");
      }

      counterBIIR = 3;
      break;

    case 3:    //Compass

      oldBarTempBIIR = 100;
      oldPressBIIR = 100;
      oldAltBIIR = 100;
      printCompassBIIR = false;

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

      barometerBIIR();

      break;

    case 1:    //Compass Void

      compassBIIR();

      break;
  }
}

void tempHumBIIR()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float m_hBIIR = dht.readHumidity();
  float m_tBIIR = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(m_tBIIR) || isnan(m_hBIIR))
  {
    Serial.println("Failed to read from DHT");
    lcd.print("Failed to read");
  }
  else
  {
    if (!(m_hBIIR == oldHumBIIR && m_tBIIR == oldTempBIIR))
    {
      readButtonBIIR();

      Serial.print("Humidity: ");
      Serial.print(m_hBIIR);
      Serial.print(" %\t");
      readButtonBIIR();

      lcd.setCursor(0, 0);
      lcd.print("Vochtigheid: ");
      lcd.print(m_hBIIR);
      lcd.print(" %");
      readButtonBIIR();

      Serial.print("Temperature: ");
      Serial.print(m_tBIIR);
      Serial.println(" *C");
      readButtonBIIR();

      lcd.setCursor(0, 2);
      lcd.print("Temperatuur: ");
      lcd.print(m_tBIIR);
      lcd.print(" C");
      readButtonBIIR();

      oldHumBIIR = m_hBIIR;
      oldTempBIIR = m_tBIIR;
    }
  }
}

void barometerBIIR()
{

  if (printBarometerBIIR == false)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.setCursor(0, 1);
    lcd.print("Druk: ");
    lcd.setCursor(0, 2);
    lcd.print("Hoogte: ");

    printBarometerBIIR = true;
  }
  else
  {
    printBaroTempBIIR();
    printBaroPressBIIR();
    printBaroAltBIIR();
  }
}

void printBaroTempBIIR()
{

  newBarTempBIIR = bmp280.getTemperature();

  if ((newBarTempBIIR - oldBarTempBIIR) > 0.03  || (oldBarTempBIIR - newBarTempBIIR) > 0.03)
  {
    lcd.setCursor(10, 0);
    lcd.print(newBarTempBIIR);

    oldBarTempBIIR = newBarTempBIIR;
  }
}

void printBaroPressBIIR()
{

  newBarPressBIIR = bmp280.getPressure();

  if ((newBarPressBIIR - oldPressBIIR) > 5.00  || (oldPressBIIR - newBarPressBIIR) > 5.00)
  {
    lcd.setCursor(10, 1);
    lcd.print(newBarPressBIIR);

    oldPressBIIR = newBarPressBIIR;
  }
}

void printBaroAltBIIR()
{

  newBarAltBIIR = bmp280.calcAltitude(newBarPressBIIR);

  if ((newBarAltBIIR - oldAltBIIR) > 0.20      || (oldAltBIIR - newBarAltBIIR) > 0.20)
  {
    lcd.setCursor(10, 2);
    lcd.print(newBarAltBIIR);

    oldAltBIIR = newBarAltBIIR;
  }
}

void compassBIIR()         //Nog laten uitlezen LCD
{
  if (printCompassBIIR == false)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("R: ");
    lcd.setCursor(0, 1);
    lcd.print("X: ");
    lcd.setCursor(0, 2);
    lcd.print("Y: ");
    lcd.setCursor(0, 3);
    lcd.print("Z: ");
    lcd.setCursor(15, 1);
    delay(15);
    lcd.write("O");
    lcd.setCursor(0, 0);

    printCompassBIIR = true;
  }
  else
  {
    //Tell the HMC5883L where to begin reading data
    Wire.beginTransmission(address);
    Wire.write(0x03); //select register 3, compassXBIIR MSB register
    Wire.endTransmission();

    //Read data from each acompassXBIIRis, 2 registers per acompassXBIIRis
    Wire.requestFrom(address, 6);
    if (6 <= Wire.available()) {
      compassXBIIR = Wire.read() << 8; //compassXBIIR msb
      compassXBIIR |= Wire.read(); //compassXBIIR lsb
      compassZBIIR = Wire.read() << 8; //Z msb
      compassZBIIR |= Wire.read(); //Z lsb
      compassYBIIR = Wire.read() << 8; //Y msb
      compassYBIIR |= Wire.read(); //Y lsb

      printCompassHeadingBIIR();
      printCompassXBIIR();
      printCompassYBIIR();
      printCompassZBIIR();
    }
  }
}

void printCompassHeadingBIIR()
{
  compassHeadingBIIR = Compass.GetHeadingDegrees();

  if ((compassHeadingBIIR - oldCompassHeadingBIIR) > 00.50 || (oldCompassHeadingBIIR - compassHeadingBIIR) > 00.50)
  {
    lcd.setCursor(4, 0);
    lcd.print(compassHeadingBIIR);

    oldCompassHeadingBIIR = compassHeadingBIIR;
    printDirectionBIIR();
  }
}

void printDirectionBIIR()
{
  if (oldCompassHeadingBIIR > 340 || oldCompassHeadingBIIR < 20)
  {
    deleteOldHeading();

    lcd.setCursor(15, 0);
    lcd.write(byte(1));

    HeadingBIIR = 1;
  }
  if (oldCompassHeadingBIIR > 20 && oldCompassHeadingBIIR < 70)
  {
    deleteOldHeading();

    lcd.setCursor(16, 0);
    lcd.write(byte(2));

    HeadingBIIR = 2;
  }
  if (oldCompassHeadingBIIR > 70 && oldCompassHeadingBIIR < 110)
  {
    deleteOldHeading();

    lcd.setCursor(16, 1);
    lcd.write(byte(3));

    HeadingBIIR = 3;
  }
  if (oldCompassHeadingBIIR > 110 && oldCompassHeadingBIIR < 160)
  {
    deleteOldHeading();

    lcd.setCursor(16, 2);
    lcd.write(byte(4));

    HeadingBIIR = 4;
  }
  if (oldCompassHeadingBIIR > 160 && oldCompassHeadingBIIR < 200)
  {
    deleteOldHeading();

    lcd.setCursor(15, 2);
    lcd.write(byte(5));

    HeadingBIIR = 5;
  }
  if (oldCompassHeadingBIIR > 200 && oldCompassHeadingBIIR < 250)
  {
    deleteOldHeading();

    lcd.setCursor(14, 2);
    lcd.write(byte(6));

    HeadingBIIR = 6;
  }
  if (oldCompassHeadingBIIR > 250 && oldCompassHeadingBIIR < 290)
  {
    deleteOldHeading();

    lcd.setCursor(14, 1);
    lcd.write(byte(7));

    HeadingBIIR = 7;
  }
  if (oldCompassHeadingBIIR > 290 && oldCompassHeadingBIIR < 340)
  {
    deleteOldHeading();

    lcd.setCursor(14, 0);
    lcd.write(byte(8));

    HeadingBIIR = 8;
  }

}

void deleteOldHeading()
{
  switch (HeadingBIIR)
  {
    case 1:  //N

      lcd.setCursor(15, 0);
      lcd.print(" ");
      break;

    case 2:  //NE

      lcd.setCursor(16, 0);
      lcd.print(" ");
      break;

    case 3:  //E

      lcd.setCursor(16, 1);
      lcd.print(" ");
      break;

    case 4:  //SE

      lcd.setCursor(16, 2);
      lcd.print(" ");
      break;

    case 5:  //S

      lcd.setCursor(15, 2);
      lcd.print(" ");
      break;

    case 6:  //SW

      lcd.setCursor(14, 2);
      lcd.print(" ");
      break;

    case 7:  //W

      lcd.setCursor(14, 1);
      lcd.print(" ");
      break;

    case 8:  //NW

      lcd.setCursor(14, 0);
      lcd.print(" ");
      break;
  }
}

void printCompassXBIIR()
{
  if ((compassXBIIR - oldXBIIR) > 10 || (oldXBIIR - compassXBIIR) > 10)
  {
    lcd.setCursor(4, 1);
    lcd.print(compassXBIIR);

    oldXBIIR = compassXBIIR;
  }
}

void printCompassYBIIR()
{
  if ((compassYBIIR - oldYBIIR) > 10 || (oldYBIIR - compassYBIIR) > 10)
  {
    lcd.setCursor(4, 2);
    lcd.print(compassYBIIR);

    oldYBIIR = compassYBIIR;
  }
}

void printCompassZBIIR()
{
  if ((compassZBIIR - oldZBIIR) > 10 || (oldZBIIR - compassZBIIR) > 10)
  {
    lcd.setCursor(4, 3);
    lcd.print(compassZBIIR);

    oldZBIIR = compassZBIIR;
  }
}





