#include <SPI.h>
#include <Arduino.h>
// define pins and other variables
const int latchPin = 5;
const int clockPin = 18;
const int dataPin  = 23;
const int NUMBER_OF_CONNECTED_LEDS = 8;

// global array that keeps track of the LEDs brightnesses
bool brightnessMask[NUMBER_OF_CONNECTED_LEDS*4];

// setup the correct pins and initialize SPI library

// convert brightness to bitmasked brightness
void led(int ledNr, int brightness){
  // ensure 4-bit limited brightness
  brightness = constrain(brightness, 0, 15);
  
  // turn 4-bit brightness into brightness mask
  for (int i = 3; i >= 0; i--) {
    if (brightness - (1 << i) >= 0) {
      brightness -= (1 << i);
      brightnessMask[(ledNr*4)+i] = 1;
    }
    else{
      brightnessMask[(ledNr*4)+i] = 0;
    }
  }
}



// turn off all LEDs using 595's
void clearLeds() {
  digitalWrite(latchPin, HIGH);
  SPI.transfer(0);
  SPI.transfer(0);
  digitalWrite(latchPin, LOW);
}


// turn on correct LED using 595's
void turnOnLed(int ledNr) {
  digitalWrite(latchPin, HIGH);

  if (ledNr >= 8) {
    SPI.transfer(1<<ledNr-8);
    SPI.transfer(0);
  }
  else {
    SPI.transfer(0);
    SPI.transfer(1<<ledNr);
  }

  digitalWrite(latchPin, LOW);
}


// transform brighnesses to 4-bit BAM
void refresh(){
  // Loop over each LED
  for (int cycle = 0; cycle < 16; cycle++) {
    Serial.printf("%d\n", cycle);
    for (int currentLed = 0; currentLed < NUMBER_OF_CONNECTED_LEDS; currentLed++) {
      int maskPosition = currentLed * 4;
      if (cycle == 1 && brightnessMask[maskPosition]) {
        turnOnLed(currentLed);
      }
      else if ((cycle == 2 || cycle == 3) && brightnessMask[maskPosition+1]) {
        turnOnLed(currentLed);
      }
      else if (cycle >= 4 && cycle <= 7 && brightnessMask[maskPosition+2]) {
        turnOnLed(currentLed);
      }
      else if (cycle >= 8 && cycle <= 15 && brightnessMask[maskPosition+3]) {
        turnOnLed(currentLed);
      }
      else{
        clearLeds();
      }
    }
  }
  clearLeds();
}

void setup() {
  // setup pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin,  OUTPUT);
  
  // setup SPI framework
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.begin();

  // True random by seeding it a random analogRead signal
  randomSeed(analogRead(0));
  
  // turn all LEDs off in advance
  clearLeds();
}




///////////////////////
// ANIMATION METHODS //
///////////////////////

void leftToRight(){
  // fade in from left to right
  for(int cl=0; cl < NUMBER_OF_CONNECTED_LEDS; cl++){
    for(int b=0; b<15; b++){
      led(cl, b);
      refresh();
    }
  }

  // fade out from left to right
  for(int cl=0; cl < NUMBER_OF_CONNECTED_LEDS; cl++){
    for(int b=15; b>=0; b--){
      led(cl, b);
      refresh();
    }
  }
}
void rightToLeft(){
  // fade in from right to left
  for(int cl=NUMBER_OF_CONNECTED_LEDS; cl >= 0; cl--){
    for(int b=0; b<15; b++){
      led(cl, b);
      refresh();
    }
  }

  // fade out from right to left
  for(int cl=NUMBER_OF_CONNECTED_LEDS; cl >= 0; cl--){
    for(int b=15; b>=0; b--){
      led(cl, b);
      refresh();
    }
  }
}
void fromMiddle(){
  int led1 = NUMBER_OF_CONNECTED_LEDS/2;
  int led2 = NUMBER_OF_CONNECTED_LEDS/2 - 1;
  int diff =0;

  // fade in from middle
  for(int diff =0; diff < 8; diff++){
    for(int b = 0; b < 15; b++){
      led(led1 + diff, b);
      led(led2 - diff, b);
      refresh();
    }
  }

  // fade out from middle
  for(int diff =0; diff < 8; diff++){
    for(int b = 15; b >= 0; b--){
      led(led1 + diff, b);
      led(led2 - diff, b);
      refresh();
    }
  }
}
void toMiddle(){
  int led1 = 0;
  int led2 = NUMBER_OF_CONNECTED_LEDS;
  int diff = 0;

  // fade in to middle
  for(int diff =0; diff <= 8; diff++){
    for(int b = 0; b < 15; b++){
      led(led1 + diff, b);
      led(led2 - diff, b);
      refresh();
    }
  }

  // fade out to middle
  for(int diff =0; diff <= 8; diff++){
    for(int b = 15; b >= 0; b--){
      led(led1 + diff, b);
      led(led2 - diff, b);
      refresh();
    }
  }  
}
void fillRight(){
  for(int ml = NUMBER_OF_CONNECTED_LEDS; ml >= 0; ml--){
    for(int cl=0; cl < ml; cl++){
      for(int c=0; c < 10; c++){
        led(cl, 15);
        refresh();
      }
      if(cl+1 != ml){
        led(cl, 0);
        refresh();
      }
    }
  }
}
void fillLeft(){
 for(int ml=0; ml <= NUMBER_OF_CONNECTED_LEDS; ml++){
    for(int cl=NUMBER_OF_CONNECTED_LEDS; cl >=ml; cl--){
      for(int c=0; c < 10; c++){
        led(cl, 15);
        refresh();
      }
      if(cl != ml){
        led(cl, 0);
        refresh();
      }
    }
  } 
}

// turn off all the LEDs
void allOff(){
  for(int cl=0; cl < NUMBER_OF_CONNECTED_LEDS; cl++){
    led(cl, 0);
  }
}

// turn on all the LEDs
void allOn(){
  for(int cl =0; cl < NUMBER_OF_CONNECTED_LEDS; cl++){
    led(cl, 15);
  }
}


////////////////////////////////////////
// Internal methods to drive the LEDs //
////////////////////////////////////////



void flashAll(){
  for(int c =0; c < 5; c++){
    for(int on=0; on< 50; on++){
      allOn();
      refresh();
    }
    
    for(int on=0; on< 50; on++){
      allOff();  
      refresh();
    }
  }
}

void fade() {
      for(int i=0;i<16;i++) {
        led(0, i);
        refresh();  
      }
      
      
}

// loop to draw animations
void loop() {
      led(5, 15);
      led(6, 15);
  refresh();

}