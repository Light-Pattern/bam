#include <SPI.h>
#include <Arduino.h>
// define pins and other variables
const int latchPin = 5;
const int clockPin = 18;
const int dataPin  = 23;
const int enablePin = 22;
const uint FREQUENCY = 100000;
const int NUMBER_OF_CONNECTED_LEDS = 8;
byte leds[] = {0,0,0,0,0,0,0,0};


void setBrightness(int ledNumber, byte brightness) {
  leds[ledNumber] = brightness;
}

void turnOnLed(int ledNumber) {
  SPI.transfer(1<<ledNumber);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
}

void clear() {
  SPI.transfer(0);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
}

void ledLoop() {
  for(int i=0;i<15;i++) {
    for(int j=0;j<NUMBER_OF_CONNECTED_LEDS;j++) {
      byte value = leds[j];
      if(i==0) {
        if(bitRead(value,0)) {
          Serial.println("turn on 0");
      turnOnLed(j);
        }
    }
    else if(i==1 || i==2) {
      if(bitRead(value,1)) {
        Serial.println("turn on 1");
        turnOnLed(j);
      }
    }
    else if(i>=3 && i<=6) {
      if(bitRead(value,2)) {
        Serial.println("turn on 2");
        turnOnLed(j);
      }
    }
    else if(i>7) {
      if(bitRead(value,3)) {
        Serial.println("turn on 3");
        turnOnLed(j);
      }
    }
    }
    

  }
}

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
 Serial.begin(9600);
 SPI.begin();
 SPI.setBitOrder(LSBFIRST);
 SPI.setClockDivider(SPI_CLOCK_DIV16);
 SPI.setDataMode(SPI_MODE0);
 SPI.setFrequency(FREQUENCY);
 setBrightness(0,0);
  clear();
}
// loop to draw animations
void loop() {
  setBrightness(0,1);
  ledLoop();
}