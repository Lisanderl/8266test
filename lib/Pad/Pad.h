#ifndef Pad_H
#define Pad_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#include "PCA9685.h"
#endif


class Pad {
    
    public:
    Pad(PCA9685& servoController, PCA9685_ServoEvaluator& servoEvaluator, int verticalServo, int horizontalServo);

    void verticalMove(int val);
    void horisontalMove(int val);
    void smartHorisontalMove(int val);



    private:
    int const DEFAULT_TOP = 45;
    int const DEFAULT_BOTTOM = -45;

    int _verticalServo;
    int _horizontalServo;
    PCA9685 *_servoController;
    PCA9685_ServoEvaluator *_servoEvaluator;


};

#endif 