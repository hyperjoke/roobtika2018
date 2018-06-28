#include <SPI.h>
#include "RF24.h"


//COM 15


RF24 Radio(9, 10);          //CE, CSN
byte addresses = "12345";
bool debug = false;

struct transmit_data {
  unsigned short int ch[4];

};

transmit_data cont_data;

void setup() {
  Serial.begin (115200);
  Radio.begin ();
  Radio.setPALevel(RF24_PA_LOW);
  Radio.openWritingPipe (addresses);

  pinMode (5, INPUT_PULLUP);
  pinMode (1, INPUT_PULLUP);

  cont_data.ch[0] = 512;
  cont_data.ch[1] = 512;
  cont_data.ch[2] = 512;
  cont_data.ch[3] = 512;
}

void loop() {
  joystickData ();
  trimCode();
<<<<<<< HEAD
  if (debug) {
    debugFunction();
  } else {
    Radio.write ( &cont_data, sizeof(transmit_data) );
    // delay (1000);

  }
=======
  Radio.write ( &cont_data, sizeof(transmit_data) );
 // delay (1000);
>>>>>>> f4fe55a91d529bfdd236dcf993fe2af796dcd53b
}


void joystickData () {    //Za zrihtat joystick....
  boolean last_button;
  unsigned long timer;
  int y = 0;
  int THR[4] = {0, 341, 682, 1023};
  bool swVal;
  cont_data.ch[0] = analogRead(0);
  cont_data.ch[1] = analogRead(1);
  cont_data.ch[2] = THR[y];

  if (millis() - timer > 25) {
    swVal = digitalRead(16);
    if (last_button == 0 && swVal == 0 && y < 3) {
      y++;
      last_button = !last_button;
    }
    if (last_button == 1 && swVal == 0) {
      last_button = !last_button;
    }
    if (last_button == 0 && swVal == 0 && y == 3) {
      y = 0;
      last_button = !last_button;
    }
    timer = millis();
  }

}



void trimCode () {        //koda za kalibriranje
  bool button;
  bool last_button;
  button = digitalRead (5);

  if (cont_data.ch[0] > 700 && button == HIGH && last_button == false) {
    cont_data.ch[0] += 10;
  }
  if (cont_data.ch[0] < 700 && button == HIGH && last_button == false) {
    cont_data.ch[0] -= 10;
  }

  if (cont_data.ch[1] > 700 && button == HIGH && last_button == false) {
    cont_data.ch[1] += 10;
  }
  if (cont_data.ch[1] < 700 && button == HIGH && last_button == false) {
    cont_data.ch[1] -= 10;
  }

  last_button = button;
}

void debugFunction() {
  for (int i = 0; i < 4; i++) {
    Serial.print(cont_data.ch[i]);
    Serial.print(",");
  }
}
