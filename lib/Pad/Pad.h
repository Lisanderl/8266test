#ifndef Pad_H
#define Pad_H


#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#include "PCA9685.h"
#endif

#define FULL_POSITION 90
#define AVARAGE_POSITION 0
#define LOW_POSITION -90


class Pad {
    
    public:
    Pad(PCA9685& servoController, PCA9685_ServoEvaluator& servoEvaluator, int verticalServo, int horizontalServo);

    void verticalMove(int val);
    void horisontalMove(int val);

    private:
    int _verticalServo;
    int _horizontalServo;
    PCA9685 *_servoController;
    PCA9685_ServoEvaluator *_servoEvaluator;


};

#endif 