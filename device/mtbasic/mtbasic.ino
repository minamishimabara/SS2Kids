/*
  TOYOSHIKI Tiny BASIC for Arduino
 (C)2012 Tetsuya Suzuki
 */

void setupWiFi();
void loopWiFi();
void setupMqtt();
void loopMqtt();
void setupDht11();
void loopDht11();
//void setupSakura();
void basic(void);

extern const char leds[] = {14, -1};
extern const char sws[] = {0, -1};

void setup(void){
  Serial.begin(115200);
  randomSeed(analogRead(0));
  for (int i = 0; leds[i] != (const char)-1; i++) {
    pinMode(leds[i], OUTPUT);
  }
  for (int i = 0; sws[i] != (const char)-1; i++) {
    pinMode(sws[i], INPUT);
  }
  setupWiFi();
  setupMqtt();
  setupDht11();
//  setupSakura();
}

void loop(void) {
//  loopWiFi();
//  loopMqtt();
//  loopDht11();
  basic();
}

/*
10 if sw() = 0 led 14 off
20 if sw() # 0 led 14 on
30 PRINT "HUMIDITY:",HUMIDITY()
40 PRINT "TEMPERATURE:",TEMPERATURE()
50 rem mqtt "iot-2/evt/status/fmt/json","tmp",TEMPERATURE(),"hum",HUMIDITY()
60 rem http "tables/ss2ito","tmp",TEMPERATURE(),"hum",HUMIDITY()
65 http "trigger/iphone/with/key/dpj0Wn5_i58qaygVwM8I7I","value1","aaaa","value2","bbbb", "value3", "cccc"
70 http "","",
80 sleep 2000
90 goto 10
*/

