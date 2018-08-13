#ifndef MoveController_H
#define MoveController_H

#define MAX_VAL 507
#define AVARAGE_VAL 302
#define LOW_VAL 97 

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#include "PCA9685.h"
#include "Pad.h"
#endif

class MoveController {

public: 

MoveController(int pawPairs);

void defaultPosition();


private:



PCA9685 *pwmController;
PCA9685_ServoEvaluator *leftServoEvaluator = nullptr;
PCA9685_ServoEvaluator *rightServoEvaluator = nullptr;
PCA9685_ServoEvaluator *verticalServoEvaluator = nullptr;
std::vector<Pad*> pads;

};
#endif