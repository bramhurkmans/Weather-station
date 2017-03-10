/* Made by Bram, Ilse, Iain en Rowan
  IC15AO-A/B
  2017
*/

#include <math.h>
#include "Seeed_BMP280.h"
#include <HMC5883L.h>
// Reference the I2C Library
#include <Wire.h>
// Reference the HMC5883L Compass Library
#include <HMC5883L.h>
//Include de LCD
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 255;

// Store our compass as a variable.
HMC5883L compass;
// Record any errors that may occur in the compass.
int error = 0;

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
  if (!bmp280.init()) {
    Serial.println("Device error!");
  }

  lcd.begin(16, 2); //Constateer de grootte en breedte van de lcd

  lcd.setRGB(colorR, colorG, colorB); //Zet de kleur van de LCD

  //Voor het kompas
  Wire.begin(); // Start the I2C interface.

  Serial.println("Constructing new HMC5883L");

  Serial.println("Setting scale to +/- 1.3 Ga");
  error = compass.setScale(1.3); // Set the scale of the compass.
  if (error != 0) // If there is an error, print it out.
    Serial.println(compass.getErrorText(error));

  Serial.println("Setting measurement mode to continous.");
  error = compass.setMeasurementMode(MEASUREMENT_CONTINUOUS); // Set the measurement mode to Continuous
  if (error != 0) // If there is an error, print it out.
    Serial.println(compass.getErrorText(error));

  //Eind kompas
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

  // De kompas LOOP

  // Retrive the raw values from the compass (not scaled).
  MagnetometerRaw raw = compass.readRawAxis();
  // Retrived the scaled values from the compass (scaled to the configured scale).
  MagnetometerScaled scaled = compass.readScaledAxis();

  // Values are accessed like so:
  int MilliGauss_OnThe_XAxis = scaled.XAxis;// (or YAxis, or ZAxis)

  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(scaled.YAxis, scaled.XAxis);

  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: -2��37' which is -2.617 Degrees, or (which we need) -0.0456752665 radians, I will use -0.0457
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = -0.0457;
  heading += declinationAngle;

  // Correct for when signs are reversed.
  if (heading < 0)
    heading += 2 * PI;

  // Check for wrap due to addition of declination.
  if (heading > 2 * PI)
    heading -= 2 * PI;

  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180 / M_PI;

  // Output the data via the serial port.
  Output(raw, scaled, heading, headingDegrees);

  // Normally we would delay the application by 66ms to allow the loop
  // to run at 15Hz (default bandwidth for the HMC5883L).
  // However since we have a long serial out (104ms at 9600) we will let
  // it run at its natural speed.
  delay(66);//of course it can be delayed longer.
  //return compassValBiri;
}

void Output(MagnetometerRaw raw, MagnetometerScaled scaled, float heading, float headingDegrees)
{ //Zet alles van de kompas in de serial.
  Serial.print("Raw:\t");
  Serial.print(raw.XAxis);
  Serial.print("   ");
  Serial.print(raw.YAxis);
  Serial.print("   ");
  Serial.print(raw.ZAxis);
  Serial.print("   \tScaled:\t");

  Serial.print(scaled.XAxis);
  Serial.print("   ");
  Serial.print(scaled.YAxis);
  Serial.print("   ");
  Serial.print(scaled.ZAxis);

  Serial.print("   \tHeading:\t");
  Serial.print(heading);
  Serial.print(" Radians   \t");
  Serial.print(headingDegrees);
  Serial.println(" Degrees   \t");
}

void LCD 
{
    lcd.setCursor(0, 0);
  lcd.setRGB(255, 0, 255);
  lcd.print("Send Nudes");
  delay(200);
  lcd.setCursor(0, 0);
  lcd.setRGB(0,255,255);
  lcd.print("          ");
  delay(200);
}

