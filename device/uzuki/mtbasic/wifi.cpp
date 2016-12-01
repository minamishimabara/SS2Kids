#include <ESP8266WiFi.h>

const char* ssid     = "uFi_F1298D";
const char* password = "7Kj5GA5652";

void setupWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);  
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loopWiFi() {
  const char* host = "canon.jp";
  delay(5000);
  Serial.print("connecting to ");
  Serial.println(host);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  // We now create a URI for the request
  String url = "/";  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }  
  Serial.println();
  Serial.println("closing connection");
}
void post(char *host, char *url, char *json) {
  WiFiClient wifiClient;
  if (!wifiClient.connect(host, 80)) {
    Serial.println("connection failed");
    return;
  }
  //Serial.println("http post...");
  wifiClient.println(String("POST /")  + url + " HTTP/1.1");
  wifiClient.println(String("Host: ") + host);
  wifiClient.println("Cache-Control: no-cache");
  wifiClient.println("Content-Type: application/json");
  wifiClient.print("Content-Length: ");
  wifiClient.println(strlen(json));
  wifiClient.println();
  wifiClient.println(json);
//  while(wifiClient.available()){
//    String line = wifiClient.readStringUntil('\r');
//    Serial.print(line);
//  }
}

// "data:image/jpeg;base64,"を先頭に付ける
// BASE64にするには3バイト単位でないと不都合
// なので、PIC_PKT_LENが128→126にする
// 画像の小さいPICFMT_OCIFにする

void cnv(char *p, int n) {
	char *w = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int i = 0, x = 0, l = 0;
	char buff[128 *8/6+1];
	for (int j = 0; j < n; j++, p++) {
		x = x << 8 | *p;
		for (l += 8; l >= 6; l -= 6) {
			buff[i++] = w[(x >> (l - 6)) & 0x3f];
		}
	}
	if (l > 0) {
		x <<= 6 - l;
		buff[i++] = w[x & 0x3f];
	}
	for (; i % 4;) {
		buff[i++] = '=';
	}
	buff[i++] = '\0';
	puts(buff);
}