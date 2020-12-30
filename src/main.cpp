#include <Arduino.h>
volatile boolean turnDetected;

const int pinClock = 21;
const int pinData = 22;
const int pinSwitch = 23;

const int pinWarm1 = 13;
const int pinCool1 = 14;
const int pinWarm2 = 27;
const int pinCool2 = 26;
const int pinWarm3 = 25;
const int pinCool3 = 33;
const int pinWarm4 = 32;
const int pinCool4 = 2;

const int channels[9] = {0,1,2,3,4,5,6,7,8};

const int warmChannel1 = 0;
const int coolChannel1 = 1;
const int warmChannel2 = 2;
const int coolChannel2 = 3;
const int warmChannel3 = 4;
const int coolChannel3 = 5;
const int warmChannel4 = 6;
const int coolChannel4 = 7;

 int mode = 0; //brightness=0,temp = 1;
const int numLights = 8;
const int freq = 5000;
const int resolution = 8;
// Variables to debounce Rotary Encoder
long timeOfLastDebounce = 0;
int delayofDebounce = .01;

long timeOfLastDebounce2 = 0;

long lastSwitchDebounce = 0;
int switchDebounceDelay = 1000;

int previousClock;   
int previousData;

int previousClock2;   
int previousData2;


int warmCounter=0;
int coolCounter = 0;
RTC_DATA_ATTR int stepIndexes[8] = {0,0,0,0,0,0,0,0};
RTC_DATA_ATTR int lightValues[8] = {0,0,0,0,0,0,0,0};
RTC_DATA_ATTR int tempIndex= 5;

bool tempSteps[9][8] = {
  {1,0,1,0,1,0,1,0},
  {1,1,1,0,1,0,1,0},
  {1,1,1,1,1,0,1,0},
  {1,1,1,1,1,1,1,0},
  {1,1,1,1,1,1,1,1},
  {0,1,1,1,1,1,1,1},
  {0,1,0,1,1,1,1,1},
  {0,1,0,1,0,1,1,1},
  {0,1,0,1,0,1,0,1}
};
int steps[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05,
0x05, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x0A, 0x0A, 0x0B, 0x0B,
0x0C, 0x0C, 0x0D, 0x0D, 0x0E, 0x0F, 0x0F, 0x10, 0x11, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1F, 0x20, 0x21, 0x23, 0x24, 0x26, 0x27, 0x29, 0x2B, 0x2C,
0x2E, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3A, 0x3C, 0x3E, 0x40, 0x43, 0x45, 0x47, 0x4A, 0x4C, 0x4F,
0x51, 0x54, 0x57, 0x59, 0x5C, 0x5F, 0x62, 0x64, 0x67, 0x6A, 0x6D, 0x70, 0x73, 0x76, 0x79, 0x7C,
0x7F, 0x82, 0x85, 0x88, 0x8B, 0x8E, 0x91, 0x94, 0x97, 0x9A, 0x9C, 0x9F, 0xA2, 0xA5, 0xA7, 0xAA,
0xAD, 0xAF, 0xB2, 0xB4, 0xB7, 0xB9, 0xBB, 0xBE, 0xC0, 0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE,
0xD0, 0xD2, 0xD3, 0xD5, 0xD7, 0xD8, 0xDA, 0xDB, 0xDD, 0xDE, 0xDF, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5,
0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xED, 0xEE, 0xEF, 0xEF, 0xF0, 0xF1, 0xF1, 0xF2,
0xF2, 0xF3, 0xF3, 0xF4, 0xF4, 0xF5, 0xF5, 0xF6, 0xF6, 0xF6, 0xF7, 0xF7, 0xF7, 0xF8, 0xF8, 0xF8,
0xF9, 0xF9, 0xF9, 0xF9, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFC,
0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD,
0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF
};
RTC_DATA_ATTR int bootCount = 0;
#define Threshold 40
touch_pad_t touchPin;

void setup() {
    
    Serial.begin(9600);
pinMode(pinWarm1, OUTPUT);
pinMode(pinWarm2, OUTPUT);
pinMode(pinWarm3, OUTPUT);
pinMode(pinWarm4, OUTPUT);
pinMode(pinCool1, OUTPUT);
pinMode(pinCool2, OUTPUT);
pinMode(pinCool3, OUTPUT);
pinMode(pinCool4, OUTPUT);
    ledcSetup(warmChannel1, freq, resolution );
    ledcAttachPin(pinWarm1, warmChannel1);

    ledcSetup(coolChannel1, freq, resolution );
    ledcAttachPin(pinCool1, coolChannel1);

    ledcSetup(warmChannel2, freq, resolution );
    ledcAttachPin(pinWarm2, warmChannel2);

    ledcSetup(coolChannel2, freq, resolution );
    ledcAttachPin(pinCool2, coolChannel2);

    ledcSetup(warmChannel3, freq, resolution );
    ledcAttachPin(pinWarm3, warmChannel3);

    ledcSetup(coolChannel3, freq, resolution );
    ledcAttachPin(pinCool3, coolChannel3);

    ledcSetup(warmChannel4, freq, resolution );
    ledcAttachPin(pinWarm4, warmChannel4);

    ledcSetup(coolChannel4, freq, resolution );
    ledcAttachPin(pinCool4, coolChannel4);

    previousClock = digitalRead(pinClock);
    previousData = digitalRead(pinData);
    pinMode(pinSwitch, INPUT_PULLUP);

}

void increaseIntensity() {
  Serial.println("inc");
    for(int i=0;i<numLights;i++) {
int index = stepIndexes[i]; 
    if(index<255) {
        int newValue = index +4;
        stepIndexes[i] = newValue; 
        lightValues[i]= steps[newValue];
    }
    }
    Serial.println(lightValues[7]);
}

void decreaseIntensity() {
  Serial.println("dec");
for(int i=0;i<numLights;i++) {
int index = stepIndexes[i]; 
    if(index>0) {
        int newValue = index -4;
        stepIndexes[i] = newValue; 
        lightValues[i]= steps[newValue];
    }
}
}

void increaseTemp() {
  if(tempIndex<8) {
    tempIndex++;
    Serial.printf("new temp: %d\n", tempIndex);
  }
}

void decreaseTemp() {
if(tempIndex>0) {
    tempIndex--;
    Serial.printf("new temp: %d\n", tempIndex);
  }
}

void increase() {
  if(mode==1) {
    increaseTemp();
  }
  else {
    increaseIntensity();
  }
}

void decrease() {
if(mode==1) {
    decreaseTemp();
  }
  else {
    decreaseIntensity();
  }
}

void checkRotary(int previousClock3, int previousData3, int pinClock3, int pinData3) {
    if ((previousClock3 == 0) && (previousData3 == 1)) {
    if ((digitalRead(pinClock3) == 1) && (digitalRead(pinData3) == 0)) {
      increase();
      
    }
    if ((digitalRead(pinClock3) == 1) && (digitalRead(pinData3) == 1)) {
      decrease();
      
    }
  }

if ((previousClock3 == 1) && (previousData3 == 0)) {
    if ((digitalRead(pinClock3) == 0) && (digitalRead(pinData3) == 1)) {
      increase();
      
    }
    if ((digitalRead(pinClock) == 0) && (digitalRead(pinData) == 0)) {
      decrease();
      
    }
  }

if ((previousClock3 == 1) && (previousData3 == 1)) {
    if ((digitalRead(pinClock3) == 0) && (digitalRead(pinData3) == 1)) {
      increase();
      
    }
    if ((digitalRead(pinClock3) == 0) && (digitalRead(pinData3) == 0)) {
      decrease();
      
    }
  }  

if ((previousClock3 == 0) && (previousData3 == 0)) {
    if ((digitalRead(pinClock3) == 1) && (digitalRead(pinData3) == 0)) {
      increase();
      
    }
    if ((digitalRead(pinClock3) == 1) && (digitalRead(pinData3) == 1)) {
      decrease();
      
    }
  }    
}

void handleSwitch() {
  
if((millis() - lastSwitchDebounce) > switchDebounceDelay) {
            if(mode==1) {
              mode=0;
            }
            else{
              mode =1;
            }
            lastSwitchDebounce = millis();
            Serial.printf("new mode %d\n", mode);
        }
}


int buttonState = 0;
void loop() {
    
    if((millis() - timeOfLastDebounce) > delayofDebounce) {
        checkRotary(previousClock, previousData, pinClock, pinData);
        previousClock = digitalRead(pinClock);
        previousData = digitalRead(pinData);
        timeOfLastDebounce = millis();
    }

 
    for(int i=0;i<numLights;i++) {
      int isOn = tempSteps[tempIndex][i];
      int value = 0;
      if(isOn) {
        value = lightValues[i];
      }
ledcWrite(channels[i], value);
    }
    

    if(digitalRead(pinSwitch) == LOW) {
        handleSwitch();
    }
}
