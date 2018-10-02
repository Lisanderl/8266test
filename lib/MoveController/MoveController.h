#ifndef MoveController_H
#define MoveController_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#include <PCA9685.h>
#include "Pad.h"
#include "Pad.h"
#endif

class MoveController {

public: 

enum Direction {STRAIGHT = 1, BACK = 2, LEFT = 3, RIGHT = 4, LEFT_TURN = 5, RIGHT_TURN = 6};

MoveController(PCA9685 &servoController, AngleSettings& leftServo,
                             AngleSettings& rightServo, int pairs);

void defaultPosition(boolean witVertical);
void frontPosition(boolean witVertical);
void backPosition(boolean witVertical);

void step(int val);

private:

void smartLeftRotation(int val, int del);
void smartRightRotation(int val, int del);
void crawlLeftRotation(int val);
void crawlRightRotation(int val);

    double const FULL_POSITION = 175.0;
    double const DEFAULT_POSITION = 88.0;
    double const LOW_POSITION = 5.0;
    int const SERVO_DEL = 200;
std::vector<Pad*> pads;
PCA9685 _servoController;
AngleSettings _leftServo;
AngleSettings _rightServo;
};
#endif