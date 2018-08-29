#include <MoveController.h>

MoveController::MoveController(int pawPairs){
  pwmController = new PCA9685();
  leftServoEvaluator = new PCA9685_ServoEvaluator(LOW_VAL, AVARAGE_VAL, MAX_VAL);
  rightServoEvaluator = new PCA9685_ServoEvaluator(MAX_VAL, AVARAGE_VAL, LOW_VAL);
  verticalServoEvaluator = new PCA9685_ServoEvaluator;
  
 for(int i = 0; i < pawPairs; i++){
    pads.push_back(new Pad(*pwmController, *leftServoEvaluator, i, i+1));
    pads.push_back(new Pad(*pwmController, *rightServoEvaluator, i+2, i+3));
 }
}

// does horizontal move for all pads, with vertical move
void MoveController::smartLeftRotation(int val){
for(unsigned int i = 0; i < pads.size(); i+2){
  pads.at(i)->smartHorisontalMove(val);
 }
}

// does horizontal move for all pads, with vertical move
void MoveController::smartRightRotation(int val){
for(unsigned int i = 1; i < pads.size(); i+2){
  pads.at(i)->smartHorisontalMove(val);
 }
}

// does horizontal move for all pads, without vertical move
void MoveController::crawlLeftRotation(int val){
for(unsigned int i = 0; i < pads.size(); i+2){
  pads.at(i)->horisontalMove(val);
 }
}

void MoveController::crawlRightRotation(int val){
for(unsigned int i = 1; i < pads.size(); i+2){
  pads.at(i)->horisontalMove(val);
 }
}


void MoveController::defaultPosition(boolean witVertical){
 witVertical ? smartLeftRotation(DEFAULT_POSITION) : crawlLeftRotation(DEFAULT_POSITION);
 witVertical ? smartRightRotation(DEFAULT_POSITION) : crawlRightRotation(DEFAULT_POSITION);
}

void MoveController::frontPosition(boolean witVertical){
   witVertical ? smartLeftRotation(FULL_POSITION) : smartLeftRotation(FULL_POSITION); 
    witVertical ? smartRightRotation(FULL_POSITION) : crawlRightRotation(FULL_POSITION); 
}


void MoveController::backPosition(boolean witVertical){
   witVertical ? smartLeftRotation(LOW_POSITION) : smartLeftRotation(LOW_POSITION);
    witVertical ? smartRightRotation(LOW_POSITION) : crawlRightRotation(LOW_POSITION);
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
     smartLeftRotation(DEFAULT_POSITION);
     delay(5); 
     smartRightRotation(FULL_POSITION);
     delay(5); 
     crawlLeftRotation(FULL_POSITION);
     crawlRightRotation(DEFAULT_POSITION);
     break;}

     case RIGHT : {
     smartLeftRotation(DEFAULT_POSITION);
     delay(5); 
     smartRightRotation(FULL_POSITION);
     delay(5); 
     crawlLeftRotation(FULL_POSITION);
     crawlRightRotation(DEFAULT_POSITION);
     break;}

     case LEFT_TURN : {
     smartLeftRotation(FULL_POSITION);
     delay(5); 
     smartRightRotation(LOW_POSITION);
     delay(5); 
    defaultPosition(false);
     break;}

     case RIGHT_TURN : {
     smartLeftRotation(LOW_POSITION);
     delay(5); 
     smartRightRotation(FULL_POSITION);
     delay(5); 
     defaultPosition(false);
     break;}

  }
}