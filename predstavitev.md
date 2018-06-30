# Predstaviev RC Letala
![Slika letala](https://github.com/hyperjoke/roobtika2018/blob/master/IMG_20180630_093824.jpg?raw=true) //todo

#### Cilj projekta
Namen projekta je izdelati daljinsko vodeno letalo.

#### Deli za izdelavo
Za izdelavo smo uporabili pribl. kvadratni meter penjenega PVCja (za trup letala), 
4 servo motorje, brushless motor, propeller, 2 arduino nano (kitajski ponaredek), 2 joystica, 2 NRF24 01l.

#### Ugotovitve
Letalo se hitro skrči, treba ga je obvladati drugače se ga ne splača peljati.

#### Video
[![Letalo](http://img.youtube.com/vi/j4iA9kGE_WQ/3.jpg)](http://www.youtube.com/watch?v=j4iA9kGE_WQ)

#### Zaključek
Letalo smo peljali, letelo je super, a smo ga potem uničili (taka je pač tradicija).

### Pripomočki za izdelavo
Pri izdelavi smo uporabljali spajkalnik, PSU in multimeter.


#### Sheme
![Shema sprejemnika](https://github.com/hyperjoke/roobtika2018/blob/master/shema/sprejemnik%20(na%20letalu).png)


![Shema oddajnika](https://github.com/hyperjoke/roobtika2018/blob/master/shema/oddajnik%20(na%20remotu).png)

#### Program sprejemnika 
```C++
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
  cont_data.ch[2] = 0; //throtle
  cont_data.ch[3] = 512; //
}


void loop() {
  //byte num=44;
  //unsigned long got_time;
  if (RadioModule.available() ) {                            // While nothing is received
    RadioModule.read( &cont_data, sizeof(podatki_za_sprejem) );
    for (int i = 0; i < ST_KANALOV; i++) {
      Serial.print(cont_data.ch[i]);
      if (i < (ST_KANALOV - 1)) Serial.print(",");
    }
    Serial.println("");
  }
  updateServo(cont_data.ch);
  Serial.println();

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
    Motor[i].write(omejitevServota(i, mapirano));

    //Serial.print(mapirano);
    //Serial.print(",");
  }
  //Serial.println("DELA!");
}




/*
 * virtualni endstopi
*/
unsigned short int omejitevServota(int ch_nr, unsigned short int ch_data) {
  switch (ch_nr) {
    case 0:
      return constrain(ch_data, 45, 130);
      break;

    case 1:
      return constrain(ch_data, 0, 180); // TODO napiši omejitev
      break;

    case 2:
      return ch_data;
      break;

    case 3:
      return constrain(ch_data, 0, 180); // TODO napiši omejitev
      break;
  }

  return 0;
}

/*
 * mapBrushless(raw data)
 * 
 * funkcija mappira podatke joystcka,, 
 * če je joystick (raw_data) manjšiu od 545(ozirma stoji) returna 0, če ne mappiran podatek
 * 
*/

unsigned short int mapBrushless(unsigned short int tomap) {
  return ((tomap < 545) ? 0 :  constrain(map(tomap, 545, 1023, 165, 60),60, 160));
}

```
#### Program oddajnika 
```C++
#include <SPI.h>
#include "RF24.h"
#define N_CH 4

RF24 Radio(9, 10);          //CE, CSN
byte addresses = "12345";
bool debug = false;
struct transmit_data {
  unsigned short int ch[4];
};
/*int trim_var_x = 0;
  int trim_var_y = 0;*/
transmit_data cont_data;
int a;


void setup() {
  Serial.begin (115200);
  Radio.begin ();
  Radio.setPALevel(RF24_PA_LOW);
  Radio.openWritingPipe (addresses);

  for ( a = 0; a < N_CH; a++) {
    pinMode(14 + a, INPUT);
    cont_data.ch[a] = 512;
  }
}

void loop() {
  joystickData ();
  /*trimCode();*/

  if (debug) {
    debugFunction();
  } else {
    sendData ();
  }
}


void sendData() {
  Radio.write ( &cont_data, sizeof(transmit_data) );
}


void joystickData () {
  for (a = 0; a < N_CH; a++) {
    cont_data.ch[a] = analogRead(a);
  }
}
/*
  if (cont_data.ch[0] < 0) cont_data.ch[0] = 0;
  if (cont_data.ch[0] > 1023) cont_data.ch[0] = 1023;
  if (cont_data.ch[1] < 0) cont_data.ch[1] = 0;
  if (cont_data.ch[1] > 1023) cont_data.ch[1] = 1023;
*/



/*void trimCode () {        //koda za kalibriranje
  bool button;
  button = digitalRead (5);
  button = !button;
  if (cont_data.ch[0] > 650 && button == HIGH && last_button == !button && cont_data.ch[0] < 1013) {
    trim_var_x += 10;
  }
  if (cont_data.ch[0] < 200 && button == HIGH && last_button == !button && cont_data.ch[0] > 10) {
    trim_var_x -= 10;
  }
  if (cont_data.ch[1] > 650 && button == HIGH && last_button == !button && cont_data.ch [1] < 1013) {
    trim_var_y += 10;
  }
  if (cont_data.ch[1] < 200 && button == HIGH && last_button == !button /*&& cont_data.ch[1] > 10) {
    trim_var_y -= 10;
  }
  last_button = button;
  }*/


void debugFunction() {
  for (a = 0; a < N_CH; a++) {
    Serial.print(cont_data.ch[a]);
    Serial.println ();
  }
}
```
#### Avtorji:
 * Dejan Mežnarc (dejan.mzc(afna)gmail.com)
 * Martin Pusar (martin.pusar(afna)gmail.com)
 * Alexander Gaydukov (alex.gaydukov(afna)gmail.com)
 
