#include <Wire.h>
#include "HCPCA9685.h"
#include "HCPCA9685_0.h"
#include "HCPCA9685_1.h"
#include "HCPCA9685_2.h"
#include "HCPCA9685_4.h"
#include "HCPCA9685_5.h"
#include "MIDIUSB.h"
#define SERVO_ON 307
#define SERVO_OFF 205
#define SERVO_FREQ 50


HCPCA9685_0 HCPCA9685_0(0x40);
HCPCA9685_1 HCPCA9685_1(0x41);
HCPCA9685 HCPCA9685(0x44);
HCPCA9685_2 HCPCA9685_2(0x42);
HCPCA9685_5 HCPCA9685_5(0x50);
HCPCA9685_4 HCPCA9685_4(0x48);

void setup() {
  Serial.begin(115200);
  
  HCPCA9685.Init(SERVO_MODE);
  HCPCA9685.Sleep(false);

  HCPCA9685_0.Init(SERVO_MODE);
  HCPCA9685_0.Sleep(false);

  HCPCA9685_1.Init(SERVO_MODE);
  HCPCA9685_1.Sleep(false);

  HCPCA9685_2.Init(SERVO_MODE);
  HCPCA9685_2.Sleep(false);

  HCPCA9685_4.Init(SERVO_MODE);
  HCPCA9685_4.Sleep(false);

  HCPCA9685_5.Init(SERVO_MODE);
  HCPCA9685_5.Sleep(false);

  delay(10);
}

void loop() {
  // Drive each servo one at a time using setPWM()
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      Serial.print("Received: ");
      Serial.print(rx.header, HEX);
      Serial.print("-");
      Serial.print(rx.byte1, HEX);//on off
      Serial.print("-");
      Serial.print(rx.byte2);//note
      Serial.print("-");
      Serial.println(rx.byte3, HEX);//velocity

      on_off_event(rx.byte2, rx.header);
    }
  } while (rx.header != 0);
}
void on_off_event(uint8_t note, uint8_t header){
  if (note<21){
    return;
  }
  note-=21;
  uint8_t controller = note/16;
  uint8_t bit = note%16;

  if (header == 0x9){// if on
    Serial.print("on-");
    Serial.print(controller);
    Serial.print(",");
    Serial.println(bit);
    if (controller ==3){
      Serial.print("return");
      HCPCA9685.Servo(bit, 240);
    }
    else if (controller ==2){
      Serial.print("return");
      HCPCA9685_2.Servo(bit, 240);
    }
    else if (controller ==4){
      Serial.print("return");
      HCPCA9685_4.Servo(bit, 240);
    }
    else if (controller ==5){
      Serial.print("return");
      HCPCA9685_5.Servo(bit, 240);
    }
    else if (controller ==1){
      Serial.print("return");
      HCPCA9685_1.Servo(bit, 240);
    }
    else if (controller ==0){
      Serial.print("return");
      HCPCA9685_0.Servo(bit, 240);
    }

  }
  if (header == 0x8){// if off
    Serial.print("off-");
    Serial.print(controller);
    Serial.print(",");
    Serial.println(bit);
    if (controller ==3){
      Serial.print("return");
      HCPCA9685.Servo(bit, 0);
    }
    else if (controller ==2){
      Serial.print("return");
      HCPCA9685_2.Servo(bit, 0);
    }
    else if (controller ==4){
      Serial.print("return");
      HCPCA9685_4.Servo(bit, 0);
    }
    else if (controller ==5){
      Serial.print("return");
      HCPCA9685_5.Servo(bit, 0);
    }
    else if (controller ==1){
      Serial.print("return");
      HCPCA9685_1.Servo(bit, 0);
    }
    else if (controller ==0){
      Serial.print("return");
      HCPCA9685_0.Servo(bit, 0);
    }
  }
}