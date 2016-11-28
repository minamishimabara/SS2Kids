#include <Arduino.h>
#include <SakuraIO.h>

SakuraIO_I2C sakuraio;

void setupSakura() {
  Serial.print("Waiting to come online");
  for(;;){
    if( (sakuraio.getConnectionStatus() & 0x80) == 0x80 ) break;
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
}

void sakura(int n, int *val) {
  for (int i = 0; i < n; i++) {
    if (sakuraio.enqueueTx(i, *val++) != CMD_ERROR_NONE) {
      Serial.println("[ERR] enqueue error");
    }
  }
  sakuraio.send();
  delay(1000);

  uint8_t available;
  uint8_t queued;
  if(sakuraio.getTxQueueLength(&available, &queued) != CMD_ERROR_NONE){
    Serial.println("[ERR] get tx queue length error");
  }

  Serial.print("Available :");
  Serial.print(available);
  Serial.print(" Queued :");
  Serial.println(queued);
}
