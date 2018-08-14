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

void MoveController::smartRotation(int val){
for(unsigned int i = 0; i < pads.size(); i++){
 
  pads.at(i)->smartHorisontalMove(val);
 }
}

void MoveController::crawlRotation(int val){
for(unsigned int i = 0; i < pads.size(); i++){
 
  pads.at(i)->horisontalMove(val);
 }
}

void MoveController::defaultPosition(){
  smartRotation(AVARAGE_POSITION);
}

void MoveController::frontPosition(){
   smartRotation(FULL_POSITION);
}

void MoveController::backPosition(){
   smartRotation(LOW_POSITION);
}

