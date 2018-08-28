#ifndef MoveController_H
#define MoveController_H


    int const  MAX_VAL = 507;
    int const  AVARAGE_VAL = 302;
    int const  LOW_VAL = 97;

    int const FULL_POSITION = 90;
    int const DEFAULT_POSITION = 0;
    int const LOW_POSITION = -90;

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#include "PCA9685.h"
#include "Pad.h"
#endif

class MoveController {

public: 

enum Direction {STRAIGHT = 1, BACK = 2, LEFT = 3, RIGHT = 4};

MoveController(int pawPairs);

void defaultPosition(boolean witVertical);
void frontPosition(boolean witVertical);
void backPosition(boolean witVertical);

void step(int val);

private:

void smartRotation(int val);
void crawlRotation(int val);

PCA9685 *pwmController;
PCA9685_ServoEvaluator *leftServoEvaluator = nullptr;
PCA9685_ServoEvaluator *rightServoEvaluator = nullptr;
PCA9685_ServoEvaluator *verticalServoEvaluator = nullptr;
std::vector<Pad*> pads;

};
#endif