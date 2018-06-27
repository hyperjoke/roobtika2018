#include <SPI.h>
#include "RF24.h"

RF24 RadioModule(9, 10); //CE, CSN
byte addresses = "12345";
struct podatki_za_sprejem {
  unsigned short int ch[4];
};
podatki_za_sprejem cont_data;

void setup() {
  Serial.begin(115200);
  RadioModule.begin();
  RadioModule.setPALevel(RF24_PA_LOW);
  RadioModule.openReadingPipe(1, addresses);
  RadioModule.startListening();

  cont_data.ch[0]=0;
  cont_data.ch[1]=0;
  cont_data.ch[2]=0;
  cont_data.ch[3]=0;
}

void loop() {
  //byte num=44;
  //unsigned long got_time;
  if (RadioModule.available() ) {                            // While nothing is received
    RadioModule.read( &cont_data, sizeof(podatki_za_sprejem) );
    for (int i = 0; i < 4; i++)
    {
      Serial.print(cont_data.ch[i]);
      if (i < 3) Serial.print(",");
    }
    Serial.println("");
  }
}
