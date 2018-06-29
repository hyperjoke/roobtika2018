#include <SPI.h>
#include "RF24.h"
#include <Servo.h>

#define ST_KANALOV 4

RF24 RadioModule(9, 10); //CE, CSN


Servo Motor[ST_KANALOV];

byte addresses = "12345";
struct podatki_za_sprejem {
  unsigned short int ch[ST_KANALOV];
};


podatki_za_sprejem cont_data;

void setup() {
  Serial.begin(115200);
  Serial.println("RESTART=============");

  /*RF Komunikacija*/
  RadioModule.begin();
  RadioModule.setPALevel(RF24_PA_LOW);
  RadioModule.openReadingPipe(1, addresses);
  RadioModule.startListening();



  /*pritrdi servo motorje na pin D3,4,5,6*/
  for (int i; i < ST_KANALOV; i++) {
    Motor[i].attach(i + 3);
    //Serial.print("servo:"); Serial.println(i);
  }

  cont_data.ch[0] = 512; //
  cont_data.ch[1] = 512; //
  cont_data.ch[2] = 550; //throtle
  cont_data.ch[3] = 512; //
}


void loop() {
  //byte num=44;
  //unsigned long got_time;
  /*if (RadioModule.available() ) {                            // While nothing is received
    RadioModule.read( &cont_data, sizeof(podatki_za_sprejem) );
    for (int i = 0; i < ST_KANALOV; i++)
    {
      Serial.print(cont_data.ch[i]);
      if (i < (ST_KANALOV - 1)) Serial.print(",");
    }
    Serial.println("");

    }*/

  updateServo(cont_data.ch);

}




/*
    updateServo(chanel data)
    Mappira vse channele ter zapiše pozicije na serverje
*/
void updateServo(unsigned short int ch[ST_KANALOV]) {
  for (int i = 0; i < 4; i++) {
    //vse kar je izven 0..1023 je 0 ozirma 1023

    unsigned short int mapirano = ch[i] > 1023 ? 1023 : ch[i];
    mapirano = mapirano < 0 ? 0 : mapirano;


    if (i == 2) { // je brushless (nastavi thotle)
      //~97..19 min-tmax
      mapirano = mapBrushless(mapirano);
    } else {
      mapirano = map(mapirano, 0 , 1023, 5, 175); //mapiranje/5..175 stopin
    }


    Motor[i].write(omejitevServota(i));

    //Serial.print(mapirano);
    //Serial.print(",");
  }
  //Serial.println("DELA!");
}

unsigned short int omejitevServota(int ch_nr) {
  switch (ch_nr) {
    case 0:
      return constrain(ch_nr, 45, 130);
      break;

    case 1:
      return constrain(ch_nr, 0, 180); // TODO napiši omejitev
      break;

    case 2:
      return constrain(ch_nr, 60, 160); // diy
      break;

    case 3:
      return constrain(ch_nr, 0, 180); // TODO napiši omejitev
      break;
  }
  return 0;
}


unsigned short int mapBrushless(unsigned short int tomap) {
  return ((tomap < 545) ? 0 :  map(tomap, 545, 1023, 165, 160));
}

























