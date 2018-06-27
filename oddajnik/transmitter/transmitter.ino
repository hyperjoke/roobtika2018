#include <SPI.h>
#include "RF24.h"
//dummy comment
RF24 Radio(9, 10);          //CE, CSN
byte addresses = "12345";

struct transmit_data {
  unsigned short int ch[4];

};

transmit_data cont_data;

void setup() {
  Serial.begin (115200);
  Radio.begin ();
  Radio.setPALevel(RF24_PA_LOW);
  Radio.openWritingPipe (addresses);

  pinMode (14, INPUT);

  cont_data.ch[0] = 512;
  cont_data.ch[1] = 512;
  cont_data.ch[2] = 512;
  cont_data.ch[3] = 512;
}

void loop() {


  joystickData ();
  Radio.write ( &cont_data, sizeof(transmit_data) );
  delay (1000);

}


void joystickData () {    //Za zrihtat joystick....
  boolean last_button;
  unsigned long timer;
  int y;
  int THR[4] = {0, 341, 682, 1023};
  cont_data.ch[0] = analogRead (14);
  cont_data.ch[1] = analogRead (15);
  cont_data.ch[2] = Thr[y];

  if (millis() - timer > 25) {
    swVal = digitalRead (1)
    if (last_button = 0 && swVal = 0 && y < 3) {
      y++;
      last_button = !last_button;
    }
    if (last_button = 1 && swVal = 0) {
      last_button = !last_button
    }
    if (last_button = 0 && swVal = 0 && y = 3) {
      y = 0;
      last_button = !last_button
    }
    timer = millis();
  }
}
