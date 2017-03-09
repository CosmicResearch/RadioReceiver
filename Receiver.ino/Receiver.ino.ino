#include <RFM69.h>
#include <RFM69_ATC.h>
#include <RFM69_OTA.h>
#include <RFM69registers.h>

#include <SPIFlash.h>
#include <SPI.h>
#include <RFM69.h>
#include <string.h>
#include <stdio.h>

#define NETWORKID 2
#define NODEID 1
#define RECEIVER 2

#define FREQUENCY RF69_433MHZ
#define ENCRIPTKEY "HOLA"
#define IS_RFM69HCW true

#define SERIAL_BAUD   115200
 
#define RFM69_CS      10
#define RFM69_IRQ     2
#define RFM69_IRQN    0
#define RFM69_RST     9

RFM69 radio;
int i = 0;

void setup() {
  while(!Serial);
  Serial.begin(SERIAL_BAUD);
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, HIGH);
  delay(100);
  digitalWrite(RFM69_RST, LOW);
  delay(100);

  radio.initialize(RF69_433MHZ, 1, 2);
  radio.setHighPower();
  radio.setPowerLevel(31);
  radio.encrypt("1234567890123456");
  radio.receiveDone();
  delay(1000);
}

void loop() {
  if (radio.receiveDone()) {
    if (radio.ACKRequested()) {
      radio.sendACK();
    }
    Serial.println(radio.RSSI);
  }
  if (Serial.available() > 0) {
    String received = Serial.readStringUntil("\n");
    const char* buff = received.c_str();
    radio.sendWithRetry(RECEIVER, buff, strlen(buff));
    delay(250);
  }
  Serial.flush();
}
