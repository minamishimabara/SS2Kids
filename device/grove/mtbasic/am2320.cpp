#include <Arduino.h>
#include <Wire.h> //by dd647

void setupAm2320()
{
  Wire.begin();
}
void am2320(int *t, int *h)
{
  //Serial.println("Start");
  Wire.beginTransmission(0x5c);     // address(0x5c)  sensor(AM2320)
  Wire.write(0x03); //Arduino read senser
  Wire.write(0x00); //address of Humidity
  Wire.write(0x04); //The number of address
  //(Humidity1,Humidity2,Temperature1,Temperature2)
  Wire.endTransmission();//
  delay(10);
  int ans = Wire.requestFrom(0x5c, 6); // request 6 bytes from sensor(AM2320)
  //Serial.println("ans:" + String(ans));
  while (true) {
    int available = Wire.available();
    //Serial.println("available:" + String(available));
    if (available == 0) {
      break;
    }
    int H1, H2, T1, T2 = 0;
    for (int i = 1; i <  ans + 1; i++) {
      int c = Wire.read();
      //Serial.println("i:" + String(i) + " -> " + String(c));
      switch (i) {
        case 5:
          T1 = c;
          break;
        case 6:
          T2 = c;
          break;
        case 3:
          H1 = c;
          break;
        case 4:
          H2 = c;
          break;
        default:
          break;
      }
    }
    *h = (H1 * 256 + H2) / 10;
    *t = (T1 * 256 + T2) / 10;
  }
}
void loopAm2320()
{
  int t = 0;
  int h = 0;
  while (t == 0 && h == 0) {
    am2320(&t, &h);
  }
  Serial.print("  Humidity=");
  Serial.print(h);
  Serial.print("   ");
  Serial.print("  Temperature=");
  Serial.println(t);
  delay(1000);
}
short getHumidity() {
  int t = 0;
  int h = 0;
  for (int i = 0; i < 10; i++ ) {
    am2320(&t, &h);
    if (t != 0 || h != 0) {
      break;
    }
  }
  return (short)h;
}
short getTemperature() {
  int t = 0;
  int h = 0;
  for (int i = 0; i < 10; i++ ) {
    am2320(&t, &h);
    if (t != 0 || h != 0) {
      break;
    }
  }
  return (short)t;
}

