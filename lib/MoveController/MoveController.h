#ifndef MoveController_H
#define MoveController_H

    int const  MAX_VALl = 507;
    int const  AVARAGE_VALl = 302;
    int const  LOW_VALl = 97;

    int const FULL_POSITION = 80;
    int const DEFAULT_POSITION = 0;
    int const LOW_POSITION = -80;

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#include "PCA9685.h"
#include "Pad.h"
#endif

class MoveController {

public: 

enum Direction {STRAIGHT = 1, BACK = 2, LEFT = 3, RIGHT = 4, LEFT_TURN = 5, RIGHT_TURN = 6};

MoveController(PCA9685 &servoController, PCA9685_ServoEvaluator& leftServoEvaluator,
                             PCA9685_ServoEvaluator& rightServoEvaluator, int pairs);

void defaultPosition(boolean witVertical);
void frontPosition(boolean witVertical);
void backPosition(boolean witVertical);

void step(int val);

private:

void smartLeftRotation(int val);
void smartRightRotation(int val);
void crawlLeftRotation(int val);
void crawlRightRotation(int val);

std::vector<Pad*> pads;
PCA9685 _servoController;
PCA9685_ServoEvaluator _leftServoEvaluator;
PCA9685_ServoEvaluator _rightServoEvaluator;
};
#endif