#include <SPI.h>
#include "RF24.h"

RF24 radio(9, 10); //CE, CSN
byte addresses = "12345";     //naslov radia

struct transmitData {
  char id;
  byte ch1;

};

transmitData mojiPodatki;

void setup() {
  Serial.begin (115200);
  radio.begin ();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe (addresses);

  mojiPodatki.id = 0;      //podatek kdo oddaja
  mojiPodatki.ch1 = 0;     //številčni podatek
}

void loop() {
  radio.write ( &mojiPodatki, sizeof(transmitData) );
  delay (1000);

}
