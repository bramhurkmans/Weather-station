/* Made by Bram, Ilse, Iain en Rowan
  IC15AO-A/B
  2017
*/

const int buttonPinBIIR = 2;     // the number of the pushbutton pin
const int ledPinBIIR =  13;      // the number of the LED pin

void setup() 
{
  Serial.begin(9600);
  pinMode(ledPinBIIR, OUTPUT);
  pinMode(buttonPinBIIR, INPUT);
}

void loop() 
{
  ReadButtonBIIR();

}

void ReadButtonBIIR()
{
   boolean m_buttonStateBIIR = digitalRead(buttonPinBIIR);

  if (m_buttonStateBIIR == HIGH) {
    // turn LED on:
    digitalWrite(ledPinBIIR, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPinBIIR, LOW);
  }
}


