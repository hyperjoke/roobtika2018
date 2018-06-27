#include <SPI.h>
#include "RF24.h"

RF24 radio(7, 8); //CE, CSN
byte addresses = "12345";
struct podatki_za_sprejem {
  char id[5];
  byte ch1;
};
podatki_za_sprejem sprejem;

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1, addresses);
  radio.startListening();

  sprejem.id[0]="D";
  sprejem.id[1]="a";
  sprejem.id[2]="v";
  sprejem.id[3]="i";
  sprejem.id[4]="d";
  sprejem.ch1=0;
}

void loop() {
  //byte num=44;
  //unsigned long got_time;
  if (radio.available() ) {                            // While nothing is received
    radio.read( &sprejem, sizeof(podatki_za_sprejem) );
    Serial.print(sprejem.id[0]);
    Serial.print("=>");
    Serial.println(sprejem.ch1);
  }
}
