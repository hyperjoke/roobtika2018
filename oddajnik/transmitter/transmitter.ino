#include <SPI.h>
#include "RF24.h"


//COM 15


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
  
  Radio.write ( &cont_data, sizeof(transmit_data) );
  delay (1000);

}


void joystickData () {    //Za zrihtat joystick....
  
  cont_data.ch[0] = analogRead (14);
  cont_data.ch [1] = analogRead (15);

}
