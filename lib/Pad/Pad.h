#ifndef Pad_H
#define Pad_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#include <PCA9685.h>
#include  <PadSettings.h>
#endif


class Pad {
    
    public:
    static Pad *makePad(PCA9685& servoController, PadSettings& servoEvaluator, PadSettings& defServoEvaluator, 
                        int verticalServo, int horizontalServo);
    void verticalMove(int val);
    void horisontalMove(int val);
    void smartHorisontalMove(int val);

    private:
     Pad(PCA9685& servoController, PCA9685_ServoEvaluator& servoEvaluator, PCA9685_ServoEvaluator& defServoEvaluator, 
                        int verticalServo, int horizontalServo);
                        
    int const DEFAULT_TOP = 60;
    int const DEFAULT_BOTTOM = -60;
   
    int _verticalServo;
    int _horizontalServo;
    PCA9685* _servoController;
    PadSettings* _servoEvaluator;
    PadSettings* _defServoEvaluator;
};
#endif 