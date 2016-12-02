//  File SerialCamera_DemoCode_CJ-OV528.ino
//  8/8/2013 Jack Shao
//  Demo code for using seeeduino or Arduino board to cature jpg format
//  picture from seeed serial camera and save it into sd card. Push the
//  button to take the a picture .
//  For more details about the product please check http://www.seeedstudio.com/depot/

#include <arduino.h>
#include <ESP8266WiFi.h>

//#define PIC_PKT_LEN    128                  //data length of each read, dont set this too big because ram is limited
#define PIC_PKT_LEN    126                  //data length of each read, dont set this too big because ram is limited
#define PIC_FMT_VGA    7
#define PIC_FMT_CIF    5
#define PIC_FMT_OCIF   3
#define CAM_ADDR       0
#define CAM_SERIAL     Serial

#define PIC_FMT        PIC_FMT_OCIF


const byte cameraAddr = (CAM_ADDR << 5);  // addr
unsigned long picTotalLen = 0;            // picture length
int picNameNum = 0;

/*********************************************************************/

/*********************************************************************/
/*********************************************************************/
void clearRxBuf()
{
    while (Serial.available())
    {
        Serial.read();
    }
}
/*********************************************************************/
void sendCmd(char cmd[], int cmd_len)
{
    for (char i = 0; i < cmd_len; i++) Serial.print(cmd[i]);
}
/*********************************************************************/
void initialize()
{
    Serial.println("\nCamera initialization start.");
    char cmd[] = {0xaa,0x0d|cameraAddr,0x00,0x00,0x00,0x00} ;
    unsigned char resp[6];

    Serial.setTimeout(500);
    while (1)
    {
        //clearRxBuf();
        sendCmd(cmd,6);
        if (Serial.readBytes((char *)resp, 6) != 6)
        {
            continue;
        }
        if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x0d && resp[4] == 0 && resp[5] == 0)
        {
            if (Serial.readBytes((char *)resp, 6) != 6) continue;
            if (resp[0] == 0xaa && resp[1] == (0x0d | cameraAddr) && resp[2] == 0 && resp[3] == 0 && resp[4] == 0 && resp[5] == 0) break;
        }
    }
    cmd[1] = 0x0e | cameraAddr;
    cmd[2] = 0x0d;
    sendCmd(cmd, 6);
    Serial.println("\nCamera initialization done.");
}
/*********************************************************************/
void preCapture()
{
    char cmd[] = { 0xaa, 0x01 | cameraAddr, 0x00, 0x07, 0x00, PIC_FMT };
    unsigned char resp[6];

    Serial.setTimeout(100);
    while (1)
    {
        clearRxBuf();
        sendCmd(cmd, 6);
        if (Serial.readBytes((char *)resp, 6) != 6) continue;
        if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x01 && resp[4] == 0 && resp[5] == 0) break;
    }
}
void Capture()
{
    char cmd[] = { 0xaa, 0x06 | cameraAddr, 0x08, PIC_PKT_LEN & 0xff, (PIC_PKT_LEN>>8) & 0xff ,0};
    unsigned char resp[6];

    Serial.setTimeout(100);
    while (1)
    {
        clearRxBuf();
        sendCmd(cmd, 6);
        if (Serial.readBytes((char *)resp, 6) != 6) continue;
        if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x06 && resp[4] == 0 && resp[5] == 0) break;
    }
    cmd[1] = 0x05 | cameraAddr;
    cmd[2] = 0;
    cmd[3] = 0;
    cmd[4] = 0;
    cmd[5] = 0;
    while (1)
    {
        clearRxBuf();
        sendCmd(cmd, 6);
        if (Serial.readBytes((char *)resp, 6) != 6) continue;
        if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x05 && resp[4] == 0 && resp[5] == 0) break;
    }
    cmd[1] = 0x04 | cameraAddr;
    cmd[2] = 0x1;
    while (1)
    {
        clearRxBuf();
        sendCmd(cmd, 6);
        if (Serial.readBytes((char *)resp, 6) != 6) continue;
        if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x04 && resp[4] == 0 && resp[5] == 0)
        {
            Serial.setTimeout(1000);
            if (Serial.readBytes((char *)resp, 6) != 6)
            {
                continue;
            }
            if (resp[0] == 0xaa && resp[1] == (0x0a | cameraAddr) && resp[2] == 0x01)
            {
                picTotalLen = (resp[3]) | (resp[4] << 8) | (resp[5] << 16);
                Serial.print("picTotalLen:");
                Serial.println(picTotalLen);
                break;
            }
        }
    }

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
void GetData(char *host, char *url)
{
    WiFiClient wifiClient;
    if (!wifiClient.connect(host, 80)) {
        Serial.println("connection failed");
        return;
    }
    posthead(&wifiClient, host, url);
    unsigned int pktCnt = (picTotalLen) / (PIC_PKT_LEN - 6);
    if ((picTotalLen % (PIC_PKT_LEN-6)) != 0) pktCnt += 1;

    char cmd[] = { 0xaa, 0x0e | cameraAddr, 0x00, 0x00, 0x00, 0x00 };
    unsigned char pkt[PIC_PKT_LEN];

    Serial.setTimeout(1000);
    for (unsigned int i = 0; i < pktCnt; i++)
    {
        cmd[4] = i & 0xff;
        cmd[5] = (i >> 8) & 0xff;

        int retry_cnt = 0;
        retry:
        delay(10);
        clearRxBuf();
        sendCmd(cmd, 6);
        uint16_t cnt = Serial.readBytes((char *)pkt, PIC_PKT_LEN);

        unsigned char sum = 0;
        for (int y = 0; y < cnt - 2; y++)
        {
            sum += pkt[y];
        }
        if (sum != pkt[cnt-2])
        {
            if (++retry_cnt < 100) goto retry;
            else break;
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
    //Serial.println("3");
    //Serial.println("\" }");
    //Serial.println("0");
    //Serial.println("");
    //while(wifiClient.available()){
    //  String line = wifiClient.readStringUntil('\r');
    //  Serial.print(line);
    //}

  //wifiClient.stop();
}

void setupCamera()
{
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
void loopCamera()
{
  delay(5000);
  camera("sensortag.azure-mobile.net", "tables/ss2saito");
  delay(60000);
}

