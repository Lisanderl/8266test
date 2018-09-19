#ifndef Pad_H
#define Pad_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#include <PCA9685.h>
#include  <AngleSettings.h>
#endif


class Pad {
    
    public:
    static Pad *makePad(PCA9685& servoController, AngleSettings& servoEvaluator, AngleSettings& defServoEvaluator, 
                        int verticalServo, int horizontalServo);
    void verticalMove(int val);
    void horisontalMove(int val);
    void smartHorisontalMove(int val);

    private:
     Pad(PCA9685& servoController, AngleSettings& servoEvaluator, AngleSettings& defServoEvaluator, 
                        int verticalServo, int horizontalServo);
                        
    int const DEFAULT_TOP = 150;
    int const DEFAULT_BOTTOM = 30;
   
    int _verticalServo;
    int _horizontalServo;
    PCA9685* _servoController;
    AngleSettings* _servoEvaluator;
    AngleSettings* _defServoEvaluator;
};
#endif 