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
void MoveController::smartRotation(int val){
for(unsigned int i = 0; i < pads.size(); i++){
  pads.at(i)->smartHorisontalMove(val);
 }
}
// does horizontal move for all pads, without vertical move
void MoveController::crawlRotation(int val){
for(unsigned int i = 0; i < pads.size(); i++){
  pads.at(i)->horisontalMove(val);
 }
}

void MoveController::defaultPosition(boolean witVertical){
 witVertical ? smartRotation(DEFAULT_POSITION) : crawlRotation(DEFAULT_POSITION);
}

void MoveController::frontPosition(boolean witVertical){
   witVertical ? smartRotation(FULL_POSITION) : crawlRotation(FULL_POSITION); 
}

void MoveController::backPosition(boolean witVertical){
   witVertical ? smartRotation(LOW_POSITION) : crawlRotation(LOW_POSITION);
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
  }
}