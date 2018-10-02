#include <Pad.h>

Pad::Pad(PCA9685& servoController, AngleSettings& servoEvaluator, AngleSettings& defServoEvaluator, 
            int verticalServo, int horizontalServo){
    _servoController = &servoController, _servoEvaluator = &servoEvaluator, _defServoEvaluator = &defServoEvaluator,
     _verticalServo = verticalServo, _horizontalServo = horizontalServo;
}

Pad *Pad::makePad(PCA9685& servoController, AngleSettings& servoEvaluator, AngleSettings& defServoEvaluator, 
                        int verticalServo, int horizontalServo){
 
 return new Pad(servoController, servoEvaluator, defServoEvaluator, verticalServo, horizontalServo);
}

void Pad::horisontalMove(int val){
       
     _servoController->setChannelPWM(_horizontalServo, _servoEvaluator->angle180ToValue(val));
}

void Pad::smartHorisontalMove(int val, int del){

     verticalMove(DEFAULT_TOP);
     delay(del);
     horisontalMove(val);
     delay(del);
     verticalMove(DEFAULT_BOTTOM);
}

void Pad::verticalMove(int val){

     _servoController->setChannelPWM(_verticalServo, _defServoEvaluator->angle180ToValue(val));
}
