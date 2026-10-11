#include <Arduino.h>
#include <SPI.h>
#include "RF24.h"

#define LED_PIN  8  
#define PIN_SCK  4
#define PIN_MISO 5
#define PIN_MOSI 6
#define PIN_CE   3
#define PIN_CSN  7

SPIClass spiBus(FSPI);
RF24 radio(PIN_CE, PIN_CSN, 16000000); 

const uint8_t jamChannels[] = {
  12, 17, 22, 27, 32, 37, 42, 47, 52, 57, 62, 67, 72, 
  2, 10, 20, 30, 40, 50, 60, 70, 79, 80               
};
const uint8_t totalChannels = sizeof(jamChannels) / sizeof(jamChannels[0]);

void setup() {

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); 


  spiBus.begin(PIN_SCK, PIN_MISO, PIN_MOSI);
  delay(100);


  if (radio.begin(&spiBus)) {
    radio.setAutoAck(false);
    radio.stopListening();
    radio.setRetries(0, 0);
    radio.setPALevel(RF24_PA_MAX, true);
    radio.setDataRate(RF24_2MBPS);
    radio.setCRCLength(RF24_CRC_DISABLED);
    radio.startConstCarrier(RF24_PA_MAX, 40);
    

    digitalWrite(LED_PIN, LOW); 
  } else {

    while(1) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      delay(100);
    }
  }
}

void loop() {
  uint8_t ch;
  if (random(0, 2) == 0) {
    ch = jamChannels[random(0, totalChannels)];
  } else {
    ch = random(2, 81);
  }

  radio.setChannel(ch);
  delayMicroseconds(25);
}
