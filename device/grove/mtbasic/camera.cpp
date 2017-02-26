//  File SerialCamera_DemoCode_CJ-OV528.ino
//  8/8/2013 Jack Shao
//  Demo code for using seeeduino or Arduino board to cature jpg format
//  picture from seeed serial camera and save it into sd card. Push the
//  button to take the a picture .

//  For more details about the product please check http://www.seeedstudio.com/depot/

#include <arduino.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

//#define PIC_PKT_LEN    128        //data length of each read, dont set this too big because ram is limited
#define PIC_PKT_LEN    126        //data length of each read, dont set this too big because ram is limited
#define PIC_FMT_VGA    7
#define PIC_FMT_CIF    5
#define PIC_FMT_OCIF   3
#define CAM_ADDR       0
#define CAM_SERIAL     softSerial

#define PIC_FMT        PIC_FMT_OCIF

// うまくいかない！！
// read可能なピンはIO12(D12),IO13(D11),IO14(D13/D7)
// GROVEシールド上ではIO14(D7)しか使えない
// RXがIO14(D7)なのは、D7ソケットでD8がTXだがESPrではRSTに割り当たってる
//
// GROVEシールド使わなければ大丈夫
// IO13(D11)に黄色(SCL)、IO2(D4)に白(SDA)でsoftSerial(14, 2)
SoftwareSerial softSerial(13, 2);  //rx,tx (11-13 is used by sd shield)

const byte cameraAddr = (CAM_ADDR << 5);  // addr
unsigned long picTotalLen = 0;            // picture length

/*********************************************************************/
void clearRxBuf() {
  while (CAM_SERIAL.available())  {
    CAM_SERIAL.read(); 
  }
}
/*********************************************************************/
void sendCmd(char cmd[], int cmd_len) {
  Serial.print("*sendCmd ");
  for (char i = 0; i < cmd_len; i++) {
    Serial.print(cmd[i], HEX);
    Serial.print(" ");
    CAM_SERIAL.write(cmd[i]); 
  }
  Serial.println("");
}
/*********************************************************************/
int readBytes(char *dest, int len, unsigned int timeout) {
  Serial.print("*readBytes ");
  int read_len = 0;
  unsigned long t = millis();
  while (read_len < len) {
    while (CAM_SERIAL.available()<1) {
      if ((millis() - t) > timeout) {
        Serial.println("  ret:" + String(read_len) + " timeout");
        return read_len;
      }
    }
    *(dest+read_len) = CAM_SERIAL.read();
   read_len++;
  }
  Serial.println("  ret:" + String(read_len));
  return read_len;
}
/*********************************************************************/
void initialize() {   
  char cmd[] = {0xaa,0x0d|cameraAddr,0x00,0x00,0x00,0x00} ;  
  unsigned char resp[6];
  Serial.println("*initialize");
  while (1) {
    sendCmd(cmd,6);
    if (readBytes((char *)resp, 6,1000) != 6) {
      Serial.println("*initialize error 1");
      delay(100);
      continue;
    }
    if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x0d && resp[4] == 0 && resp[5] == 0) {
      if (readBytes((char *)resp, 6, 500) != 6) {
        Serial.println("*initializeerror 2");
        delay(100);
        continue; 
      }
      if (resp[0] == 0xaa && resp[1] == (0x0d | cameraAddr) && resp[2] == 0 && resp[3] == 0 && resp[4] == 0 && resp[5] == 0) {
        break; 
      }
    }
    Serial.println("*initialize error 3");
    delay(100);
  }  
  cmd[1] = 0x0e | cameraAddr;
  cmd[2] = 0x0d;
  sendCmd(cmd, 6); 
  Serial.println("*initialize end");
  delay(1000);
}


/*********************************************************************/
void preCapture() {
  Serial.println("*preCapture start");
  char cmd[] = { 0xaa, 0x01 | cameraAddr, 0x00, 0x07, 0x00, PIC_FMT };  
  unsigned char resp[6]; 
  
  while (1) {
    clearRxBuf();
    sendCmd(cmd, 6);
    if (readBytes((char *)resp, 6, 1000) != 6) {
      Serial.println("*preCapture error 1");
      delay(100);
      continue; 
    }
    if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x01 && resp[4] == 0 && resp[5] == 0) {
      break; 
    }
    Serial.println("*preCapture error 2");
    delay(100);
  }
  Serial.println("*preCapture end");
}
void Capture() {
  Serial.println("*Capture start");
  char cmd[] = { 0xaa, 0x06 | cameraAddr, 0x08, PIC_PKT_LEN & 0xff, (PIC_PKT_LEN>>8) & 0xff ,0}; 
  unsigned char resp[6];
  while (1) {
    clearRxBuf();
    sendCmd(cmd, 6);
    if (readBytes((char *)resp, 6, 100) != 6) {
      Serial.println("*Capture error 1");
      delay(100);
      continue;
    }
    if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x06 && resp[4] == 0 && resp[5] == 0) {
      break; 
    }
    Serial.println("*Capture error 2");
    delay(100);
  }
  cmd[1] = 0x05 | cameraAddr;
  cmd[2] = 0;
  cmd[3] = 0;
  cmd[4] = 0;
  cmd[5] = 0; 
  while (1) {
    clearRxBuf();
    sendCmd(cmd, 6);
    if (readBytes((char *)resp, 6, 100) != 6) {
      Serial.println("*Capture error 3");
      delay(100);
      continue;
    }
    if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x05 && resp[4] == 0 && resp[5] == 0) {
      break;
    }
    Serial.println("*Capture error 4");
    delay(100);
  }
  cmd[1] = 0x04 | cameraAddr;
  cmd[2] = 0x1;
  while (1)  {
    clearRxBuf();
    sendCmd(cmd, 6);
    if (readBytes((char *)resp, 6, 100) != 6) {
      Serial.println("*Capture error 5");
      delay(100);
      continue;
    }
    if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x04 && resp[4] == 0 && resp[5] == 0) {
      if (readBytes((char *)resp, 6, 1000) != 6) {
        Serial.println("*Capture error 6");
        delay(100);
        continue;
      }
      if (resp[0] == 0xaa && resp[1] == (0x0a | cameraAddr) && resp[2] == 0x01) {
        picTotalLen = (resp[3]) | (resp[4] << 8) | (resp[5] << 16); 
        Serial.print("*picTotalLen:");
        Serial.println(picTotalLen);
        break;
      }
    }
    Serial.println("*Capture error 27");
    delay(100);
  }
  Serial.println("*Capture end");
}
/*********************************************************************/
void posthead(WiFiClient *wifiClient, char *host, char *url) {
  //Serial.println("http post...");
  char *json = {"{ \"photo\": \"data:image/jpeg;base64,\"}"};
  wifiClient->println(String("POST /")  + url + " HTTP/1.1");
  wifiClient->println(String("Host: ") + host);
  wifiClient->println("Cache-Control: no-cache");
  wifiClient->println("Content-Type: application/json");
  wifiClient->println("Transfer-Encoding: chunked");
  //wifiClient->print("Content-Length: ");
  //wifiClient->println(strlen(json));
  wifiClient->println();
  //wifiClient->println(json);
  char *msg = "{ \"photo\": \"data:image/jpeg;base64,";
  char chunk[10];
  sprintf(chunk, "%x", strlen(msg));
  wifiClient->println(chunk);
  wifiClient->println(msg);
//  Serial.println(chunk);
//  Serial.println(msg);
}
// "data:image/jpeg;base64,"を先頭に付ける
// BASE64にするには3バイト単位でないと不都合
// なので、PIC_PKT_LENが128→126にする
// 画像の小さいPICFMT_OCIFにする

void postdata(WiFiClient *wifiClient, unsigned char *p, int n) {
//  Serial.println("cnv - " + String(n));
  char *w = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  int i = 0, x = 0, l = 0;
  char buff[128 *8/6+5];
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
  char chunk[10];
  sprintf(chunk, "%x", strlen(buff));
  wifiClient->println(chunk);
  wifiClient->println(buff);
//  Serial.println(chunk);
//  Serial.println(buff);
}
void GetData(char *host, char *url) {
  Serial.println("*GetData start");
  WiFiClient wifiClient;
  if (!wifiClient.connect(host, 80)) {
      Serial.println("*connection failed");
      return;
  }
  posthead(&wifiClient, host, url);
  unsigned int pktCnt = (picTotalLen) / (PIC_PKT_LEN - 6);
  if ((picTotalLen % (PIC_PKT_LEN-6)) != 0) {
    pktCnt += 1;
  }
  char cmd[] = { 0xaa, 0x0e | cameraAddr, 0x00, 0x00, 0x00, 0x00 };
  unsigned char pkt[PIC_PKT_LEN];
  for (unsigned int i = 0; i < pktCnt; i++) {
      cmd[4] = i & 0xff;
      cmd[5] = (i >> 8) & 0xff;
      int retry_cnt = 0;
     retry:
      delay(100);
      clearRxBuf();
      sendCmd(cmd, 6);
      uint16_t cnt = readBytes((char *)pkt, PIC_PKT_LEN, 200);

      unsigned char sum = 0;
      for (int y = 0; y < cnt - 2; y++) {
          sum += pkt[y];
      }
      if (sum != pkt[cnt-2]) {
        if (++retry_cnt < 100) {
          Serial.println("*GetData error 1");
          delay(100);
          goto retry;
        } else {
          break;
        }
      }
      postdata(&wifiClient, &pkt[4], cnt-6);
  }
  cmd[4] = 0xf0;
  cmd[5] = 0xf0;
  sendCmd(cmd, 6);
  wifiClient.println("3");
  wifiClient.println("\" }");
  wifiClient.println("0");
  wifiClient.println("");
  wifiClient.flush();
  Serial.println("*GetData end");
}
/*********************************************************************/
void loopCamera()
{
  delay(10000);
  preCapture();
  Capture();
}
void setupCamera()
{
  CAM_SERIAL.begin(9600);
  initialize();
}
void camera(char *host, char *url)
{
  //Serial.println("preCapture");
  preCapture();
  //Serial.println("Capture");
  Capture();
  //Serial.println("SendData");
  GetData(host, url);
  //Serial.println("SendData done");
}

