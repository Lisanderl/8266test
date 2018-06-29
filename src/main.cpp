
#include <Arduino.h>

#include <Wire.h>
#include "PCA9685.h"
#include "Pad.h"
#include "MoveController.h"


PCA9685 pwmController;
// default 102 307 512
PCA9685_ServoEvaluator pwmServo1(97, 302, 507);

Pad simplePad(pwmController, pwmServo1, 0, 1);

void setup() {
  Serial.begin(115200);
   delay(1000);
   Wire.begin(2, 14);
   Wire.setClock(400000);
   delay(1000);
    pwmController.resetDevices();      
    pwmController.init(B000000);       
    pwmController.setPWMFrequency(50);
}

void loop() {
  Serial.println("Start");
  delay(2000);
    // pwmController.setChannelPWM(0, pwmServo1.pwmForAngle(-90));
    // Serial.println(pwmController.getChannelPWM(0)); // Should output 102 for -90°
    simplePad.verticalMove(LOW_POSITION);
    simplePad.horisontalMove(LOW_POSITION);
 delay(2000);
    // Serial.println(pwmServo1.pwmForAngle(0));   // Should output 307 for 0°
    simplePad.verticalMove(AVARAGE_POSITION);
    simplePad.horisontalMove(AVARAGE_POSITION);
  delay(2000);
    // pwmController.setChannelPWM(0, pwmServo1.pwmForAngle(-90));
    // Serial.println(pwmController.getChannelPWM(0)); // Should output 102 for -90°
     simplePad.verticalMove(FULL_POSITION);
    simplePad.horisontalMove(FULL_POSITION);
     delay(2000);
     Serial.println("end");
}



