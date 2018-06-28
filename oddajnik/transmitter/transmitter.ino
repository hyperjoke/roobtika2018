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
