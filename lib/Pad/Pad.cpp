#include <Pad.h>

Pad::Pad(PCA9685& servoController, PCA9685_ServoEvaluator& servoEvaluator, PCA9685_ServoEvaluator& defServoEvaluator, 
            int verticalServo, int horizontalServo){
    _servoController = &servoController, _servoEvaluator = &servoEvaluator, _defServoEvaluator = &defServoEvaluator,
     _verticalServo = verticalServo, _horizontalServo = horizontalServo;
    
}

Pad *Pad::makePad(PCA9685& servoController, PCA9685_ServoEvaluator& servoEvaluator, PCA9685_ServoEvaluator& defServoEvaluator, 
                        int verticalServo, int horizontalServo){
 return new Pad(servoController, servoEvaluator, defServoEvaluator, verticalServo, horizontalServo);
}

void Pad::horisontalMove(int val){

     _servoController->setChannelPWM(_horizontalServo, _servoEvaluator->pwmForAngle(val));
}

void Pad::smartHorisontalMove(int val){

     verticalMove(DEFAULT_TOP);
     delay(125);
     horisontalMove(val);
     delay(200);
     verticalMove(DEFAULT_BOTTOM);
}

void Pad::verticalMove(int val){

     _servoController->setChannelPWM(_verticalServo, _defServoEvaluator->pwmForAngle(val));
}
