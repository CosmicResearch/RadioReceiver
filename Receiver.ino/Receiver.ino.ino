#include <RFM69.h>
#include <string.h>
#include <stdio.h>

#define NETWORKID 2
#define NODEID 1
#define RECEIVER 2

#define FREQUENCY RF69_433MHZ
#define ENCRIPTKEY "ENCRIPT_KEY"
#define IS_RFM69HCW true

#define SERIAL_BAUD   115200
 
#define RFM69_CS      10
#define RFM69_IRQ     2
#define RFM69_IRQN    0
#define RFM69_RST     9

RFM69 radio;

void setup() {
  while(!Serial);
  Serial.begin(SERIAL_BAUD);

  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, HIGH);
  delay(100);
  digitalWrite(RFM69_RST, LOW);

  radio = RFM69(RFM69_CS, RFM69_IRQ, IS_RFM69HCW, RFM69_IRQN);
  radio.initialize(FREQUENCY, NODEID, NETWORKID);
  radio.setHighPower();
  radio.encrypt(ENCRIPTKEY);  
}

void loop() {
  if (radio.receiveDone()) {
    Serial.println((char*)radio.DATA);
    if (radio.ACKRequested()) {
      radio.sendACK();
    }
  }
  if (Serial.available() > 0) {
    String received = Serial.readStringUntil("/n");
    const char* buff = received.c_str();
    if (not radio.sendWithRetry(RECEIVER, buff, strlen(buff))) {
      Serial.println("Could not send the packet!");
    }
    else {
      Serial.print("RSSI: ");
      Serial.println(radio.RSSI);
    }
    delay(250);
  }
}
