#include <Pad.h>

Pad::Pad(PCA9685& servoController, PCA9685_ServoEvaluator& servoEvaluator, int verticalServo, int horizontalServo){
    _servoController = &servoController, _servoEvaluator = &servoEvaluator, _verticalServo = verticalServo, _horizontalServo = horizontalServo;

}

void Pad::horisontalMove(int val){
     _servoController->setChannelPWM(_horizontalServo, _servoEvaluator->pwmForAngle(val));
}

void Pad::smartHorisontalMove(int val){
    
     _servoController->setChannelPWM(_horizontalServo, _servoEvaluator->pwmForAngle(DEFAULT_TOP));
     delay(255);
     _servoController->setChannelPWM(_verticalServo, _servoEvaluator->pwmForAngle(val));
     delay(255);
      _servoController->setChannelPWM(_horizontalServo, _servoEvaluator->pwmForAngle(DEFAULT_BOTTOM));
}

void Pad::verticalMove(int val){
     _servoController->setChannelPWM(_verticalServo, _servoEvaluator->pwmForAngle(val));
}
