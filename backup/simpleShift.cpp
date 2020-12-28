#include <Arduino.h>

// for ESP32 Microcontrollers
int latchPin = 21;
int clockPin = 22;
int dataPin = 23;
 
byte leds = 0;
 

void updateShiftRegister()
{
   digitalWrite(latchPin, HIGH);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, LOW);
}

void setup() 
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);

  Serial.begin(9600);
}
 
void loop() 
{
  leds = 0;
  updateShiftRegister();
  delay(1000);
  for (int i = 0; i < 8; i++)
  {
    bitSet(leds, i);
    updateShiftRegister();

    for (int i = 7; i >= 0; i--)
    {
        bool b = bitRead(leds, i);
        Serial.print(b);
    }
    
    delay(1000);
    Serial.println(" ");
  }
}
 