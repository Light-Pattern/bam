#include <Arduino.h>
#include <SPI.h>
volatile int interruptCounter;
int totalInterruptCounter;
 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

byte leds[] = {0,0,0,0,0,0,0,0};

const int latchPin = 5;
const int clockPin = 18;
const int dataPin  = 23;

void setBrightness(int ledNumber, byte brightness) {
  leds[ledNumber] = brightness;
}

void turnOnLed(int ledNumber) {
  SPI.transfer(1<<ledNumber);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
}


void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}
 
void setup() {
 
  Serial.begin(9600);
 
  timer = timerBegin(0, 80, true); //esp32 prob runs at 80mhz, so prescale it
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
 
}
 
void loop() {
 
  if (interruptCounter > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
 
    totalInterruptCounter++;
 
    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);
 
  }
}



