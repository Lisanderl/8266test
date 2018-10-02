#include <MoveController.h>

MoveController::MoveController(PCA9685 &servoController, AngleSettings& leftServo,
                             AngleSettings& rightServo, int pairs)
 : _servoController(servoController),
  _leftServo(leftServo),
   _rightServo(rightServo)
 {
int correction = 0;
for(int i = 0; i < pairs; i++){
    
   pads.push_back(Pad::makePad(_servoController, _leftServo, _leftServo, i + correction, 1 + i + correction));
   pads.push_back(Pad::makePad(_servoController, _rightServo, _leftServo, 2+i+correction, 3 + i + correction));
   correction += 3;
 }
}

// does horizontal move for all _pads, with vertical move
void MoveController::smartLeftRotation(int val, int del){
for(unsigned int i = 0; i < pads.size(); i += 2){
  pads.at(i)->smartHorisontalMove(val, del);
 }
}

// does horizontal move for all _pads, with vertical move
void MoveController::smartRightRotation(int val, int del){
for(unsigned int i = 1; i < pads.size(); i += 2){
  pads.at(i)->smartHorisontalMove(val, del);
 }
}

// does horizontal move for all _pads, without vertical move
void MoveController::crawlLeftRotation(int val){
for(unsigned int i = 0; i < pads.size(); i += 2){
  pads.at(i)->horisontalMove(val);
 }
}

void MoveController::crawlRightRotation(int val){
  for(unsigned int i = 1; i < pads.size(); i += 2){
    pads.at(i)->horisontalMove(val);
  }
}


void MoveController::defaultPosition(boolean witVertical){
 witVertical ? smartLeftRotation(DEFAULT_POSITION, SERVO_DEL) : crawlLeftRotation(DEFAULT_POSITION);
 witVertical ? smartRightRotation(DEFAULT_POSITION, SERVO_DEL) : crawlRightRotation(DEFAULT_POSITION);
}

void MoveController::frontPosition(boolean witVertical){
   witVertical ? smartLeftRotation(FULL_POSITION, SERVO_DEL) : crawlLeftRotation(FULL_POSITION); 
    witVertical ? smartRightRotation(FULL_POSITION, SERVO_DEL) : crawlRightRotation(FULL_POSITION); 
}


void MoveController::backPosition(boolean witVertical){
   witVertical ? smartLeftRotation(LOW_POSITION, SERVO_DEL) : crawlLeftRotation(LOW_POSITION);
    witVertical ? smartRightRotation(LOW_POSITION, SERVO_DEL) : crawlRightRotation(LOW_POSITION);
}

void MoveController::step(int val){
  switch(val){
    case STRAIGHT : {
      frontPosition(true);
      delay(5); 
      backPosition(false);
      break;}

     case BACK : {
     backPosition(true);
     delay(5); 
     frontPosition(false);
     break;}

     case LEFT : {
     smartLeftRotation(DEFAULT_POSITION, SERVO_DEL);
     delay(5); 
     smartRightRotation(FULL_POSITION, SERVO_DEL);
     delay(5); 
     crawlLeftRotation(FULL_POSITION);
     crawlRightRotation(DEFAULT_POSITION);
     break;}

     case RIGHT : {
     smartLeftRotation(DEFAULT_POSITION, SERVO_DEL);
     delay(5); 
     smartRightRotation(FULL_POSITION, SERVO_DEL);
     delay(5); 
     crawlLeftRotation(FULL_POSITION);
     crawlRightRotation(DEFAULT_POSITION);
     break;}

     case LEFT_TURN : {
     smartLeftRotation(FULL_POSITION, SERVO_DEL);
     delay(5); 
     smartRightRotation(LOW_POSITION, SERVO_DEL);
     delay(5); 
     defaultPosition(false);
     break;}

     case RIGHT_TURN : {
     smartLeftRotation(LOW_POSITION, SERVO_DEL);
     delay(5); 
     smartRightRotation(FULL_POSITION, SERVO_DEL);
     delay(5); 
     defaultPosition(false);
     break;}
  }
}
