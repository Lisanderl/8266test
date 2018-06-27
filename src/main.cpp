
#include <Arduino.h>

#include <Wire.h>
#include "PCA9685.h"
#include "Pad.h"


PCA9685 pwmController;
PCA9685_ServoEvaluator pwmServo1;

Pad simplePad(pwmController, pwmServo1, 0, 1);
uint8_t portArray[] = {16, 5, 4, 0, 2, 14, 12, 13};
String portMap[] = {"GPIO16", "GPIO5", "GPIO4", "GPIO0", "GPIO2", "GPIO14", "GPIO12", "GPIO13"};


void check_if_exist_I2C() {
  byte error, address;
  int nDevices;
  nDevices = 0;
  for (address = 1; address < 127; address++ )  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0){
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknow error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  } //for loop
  if (nDevices == 0)
    Serial.println("No I2C devices found");
  else
    Serial.println("**********************************\n");
  //delay(1000);           // wait 1 seconds for next scan, did not find it necessary
}

void scanPorts() { 
  for (uint8_t i = 0; i < sizeof(portArray); i++) {
    for (uint8_t j = 0; j < sizeof(portArray); j++) {
      if (i != j){
        Serial.print("Scanning (SDA : SCL) - " + portMap[i] + " : " + portMap[j] + " - ");
        Wire.begin(portArray[i], portArray[j]);
        check_if_exist_I2C();
      }
    }
  }
}


void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("\n\nI2C Scanner to scan for devices on each port pair D0 to D7");
  delay(5000);
  scanPorts();
   delay(1000);
   Wire.begin(2, 14);
   Wire.setClock(400000);
   delay(1000);
    pwmController.resetDevices();      
    pwmController.init(B000000);       
    pwmController.setPWMFrequency(50);
}

void loop() {
  Serial.println("Start");
  delay(2000);
    // pwmController.setChannelPWM(0, pwmServo1.pwmForAngle(-90));
    // Serial.println(pwmController.getChannelPWM(0)); // Should output 102 for -90°
    simplePad.verticalMove(LOW_POSITION);
    simplePad.horisontalMove(LOW_POSITION);
 delay(2000);
    // Serial.println(pwmServo1.pwmForAngle(0));   // Should output 307 for 0°
    simplePad.verticalMove(AVARAGE_POSITION);
    simplePad.horisontalMove(AVARAGE_POSITION);
  delay(2000);
    // pwmController.setChannelPWM(0, pwmServo1.pwmForAngle(-90));
    // Serial.println(pwmController.getChannelPWM(0)); // Should output 102 for -90°
     simplePad.verticalMove(FULL_POSITION);
    simplePad.horisontalMove(FULL_POSITION);
     delay(2000);
     Serial.println("end");
}



