#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* mqtt_server = "quickstart.messaging.internetofthings.ibmcloud.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[256];
int value = 0;

void setupMqtt() {
  client.setServer(mqtt_server, 1883);
//  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("d:quickstart:arduino:C4346B51C61C")) {
      Serial.println("connected");
//      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void pub(char* url, char *msg) {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  client.publish(url, msg);
  Serial.print("Publish url: ");
  Serial.print(url);
  Serial.print(" message: ");
  Serial.println(msg);
}
void loopMqtt() {
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
//    sprintf(msg, "{\"d\":{\"hello\": \"%ld\"}}", value);
    sprintf(msg, "{\"d\":{\"item1\": \"%ld\",\"item2\": \"%ld\"}}", value, value+1);
    pub("iot-2/evt/status/fmt/json", msg);
  }
}

