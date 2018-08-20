
#include <Arduino.h>

#include <Wire.h>
#include "PCA9685.h"
#include "Pad.h"
#include "MoveController.h"


PCA9685 pwmController;
// default 102 307 512
PCA9685_ServoEvaluator pwmServo1(97, 302, 507);

Pad simplePad1(pwmController, pwmServo1, 0, 1);
Pad simplePad2(pwmController, pwmServo1, 2, 3);
Pad simplePad3(pwmController, pwmServo1, 4, 5);
Pad simplePad4(pwmController, pwmServo1, 6, 7);

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
  Serial.println("all ok");
simplePad1.smartHorisontalMove(40);
 delay(2000);
simplePad1.horisontalMove(-60);
 delay(5000);
simplePad2.smartHorisontalMove(35);
 delay(2000);
simplePad2.horisontalMove(-55);
 delay(5000);
}



