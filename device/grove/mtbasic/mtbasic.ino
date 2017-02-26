/*
  TOYOSHIKI Tiny BASIC for Arduino
 (C)2012 Tetsuya Suzuki
 */
#include <Wire.h>

void setupWiFi();
void setupCamera();
void loopWiFi();
void loopCamera();
void basic(void);

extern const char leds[] = {14, -1};
extern const char sws[] = {0, -1};

void setup(void){
  Serial.begin(115200);
  randomSeed(analogRead(0));
  Wire.begin();
//  setupCamera();

  for (int i = 0; leds[i] != (const char)-1; i++) {
    pinMode(leds[i], OUTPUT);
  }
  for (int i = 0; sws[i] != (const char)-1; i++) {
    pinMode(sws[i], INPUT);
  }
  setupWiFi();
  setupCamera();
}

void loop(void){
//  loopWiFi();
//  loopCamera();
  basic();
}
