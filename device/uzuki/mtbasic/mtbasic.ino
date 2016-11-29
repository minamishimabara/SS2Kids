/*
  TOYOSHIKI Tiny BASIC for Arduino
 (C)2012 Tetsuya Suzuki
 */

void setupWiFi();
void loopWiFi();
void setupMqtt();
void loopMqtt();
void setupSi1145();
void loopSi1145();
void setupDht11();
void loopDht11();
//void setupSakura();
void basic(void);

extern const char leds[] = {14, -1};
extern const char sws[] = {0, -1};

void setup(void){
  Serial.begin(115200);
  pinMode(A0, INPUT);
  randomSeed(analogRead(0));
  for (int i = 0; leds[i] != (const char)-1; i++) {
    pinMode(leds[i], OUTPUT);
  }
  for (int i = 0; sws[i] != (const char)-1; i++) {
    pinMode(sws[i], INPUT);
  }
  setupWiFi();
  setupMqtt();
  setupSi1145();
  setupDht11();
//  setupSakura();
}

void loop(void) {
//  loopWiFi();
//  loopMqtt();
//  loopSi1145();
//  loopDht11();
  basic();
}

/*
10 if sw() : 0 led 14 off
20 if sw() # 0 led 14 on
30 h:HUMIDITY()
40 T:TEMPERATURE()
50 L:ANALOG()
55 PRINT "TMP:",T," HUM:",H," LUX:",L
60 rem mqtt "iot-2/evt/status/fmt/json","tmp",TEMPERATURE(),"hum",HUMIDITY()
70 rem http "http://sensortag.azure-mobile.net/tables/ss2ito","tmp",T,"hum",H,"lux",L
80 rem http "http://maker.ifttt.com/trigger/iphone/with/key/dpj0Wn5_i58qaygVwM8I7I","value1","aaaa","value2","bbbb", "value3", "cccc"
90 sleep 2000
100 goto 10
*/

