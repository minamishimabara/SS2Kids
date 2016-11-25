#include "DHT.h"          // 5v
#define DHTPIN 2          // IO2/D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setupDht11() {
  Serial.println("DHT11 test!");
  dht.begin();
}

void loopDht11() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
}

short getHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    return -9999;
  }
  return (short)h;
}

short getTemperature() {
  //Serial.print("temp : ");
  float t = dht.readTemperature();
  //Serial.print(t);
  //Serial.println(" *C ");
  if (isnan(t)) {
    return -9999;
  }
  return (short)t;
}

