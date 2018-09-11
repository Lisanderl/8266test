#ifndef MoveController_H
#define MoveController_H

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

enum Direction {STRAIGHT = 1, BACK = 2, LEFT = 3, RIGHT = 4, LEFT_TURN = 5, RIGHT_TURN = 6};

MoveController(std::vector<Pad*> &pads);

void defaultPosition(boolean witVertical);
void frontPosition(boolean witVertical);
void backPosition(boolean witVertical);

void step(int val);

private:

void smartLeftRotation(int val);
void smartRightRotation(int val);
void crawlLeftRotation(int val);
void crawlRightRotation(int val);

std::vector<Pad*> *_pads;

};
#endif