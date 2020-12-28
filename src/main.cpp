#include <SPI.h>
#include <Arduino.h>
// define pins and other variables
const int latchPin = 5;
const int clockPin = 18;
const int dataPin  = 23;
const int enablePin = 22;
const uint FREQUENCY = 24000;
const int NUMBER_OF_CONNECTED_LEDS = 8;
byte leds[] = {0,0,0,0,0,0,0,0};


void turnOnLed(int ledNumber) {
   
SPI.transfer(1<<ledNumber);
digitalWrite(latchPin, HIGH);
   digitalWrite(latchPin, LOW);
}

void clearLeds() {
  for(int i=0;i<15;i++) {
    
    if(i==0) {
turnOnLed(0);

    }
    else if(i==1 || i==2) {
      turnOnLed(1);
    }
    else if(i>=3 && i<=6) {
      turnOnLed(2);
    }
    else if(i>7) {
      turnOnLed(3);
    }

  }

/*
  for(int i=0;i<14;i++){
SPI.transfer(32);
  digitalWrite(latchPin, HIGH);

  digitalWrite(latchPin, LOW);
  SPI.transfer(16);
  digitalWrite(latchPin, HIGH);

  digitalWrite(latchPin, LOW);
  }

  for(int i=0;i<14;i++){
SPI.transfer(32);
  digitalWrite(latchPin, HIGH);

  digitalWrite(latchPin, LOW);
  SPI.transfer(16);
  }
*/
  
}



void refresh() {
  for(int cycle=0;cycle<15;cycle++) {
    for(int ledNumber=0;ledNumber<NUMBER_OF_CONNECTED_LEDS;ledNumber++) {
      byte value = leds[ledNumber];
      
      if(cycle==0 ) {
        if(bitRead(value, 0)) {
          printf("cycle %d turnon\n", cycle);
          turnOnLed(ledNumber);
        }
      }
      else if(cycle==1 || cycle == 2) {
        if(bitRead(value, 1)) {
          printf("cycle %d turnon\n", cycle);
          turnOnLed(ledNumber);
        }
      }
      else if(cycle>=3 && cycle<=6) {
        if(bitRead(value, 2)) {
          printf("cycle %d turnon\n", cycle);
          turnOnLed(ledNumber);
        }
      }
      else if(cycle>=7) {
        if(bitRead(value, 3)) {
          printf("cycle %d turnon\n", cycle);
          turnOnLed(ledNumber);
        }
      }
    } 
  }
}

void setLed(int ledNumber, byte value) {
  leds[ledNumber] = value;
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
 clearLeds();
}
// loop to draw animations
void loop() {
  clearLeds();
}