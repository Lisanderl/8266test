
#include <Arduino.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h> 
#include <ArduinoJson.h> 
#include "MPU6050_6Axis_MotionApps20.h"
#include <MPU6050.h> 


#include <Wire.h>
#include <PCA9685.h>
#include <AngleSettings.h>
#include "Pad.h"
#include "MoveController.h"

#define INTERRUPT_PIN 13 
#define  SDA1 2
#define SCL1 14
MPU6050 mpu;
unsigned int fifoCount;     // count of all bytes currently in FIFO
unsigned int packetSize;   
uint8_t fifoBuffer[64]; // FIFO storage buffer
volatile bool mpuInterruption = false;

    // int const  MAX_VAL = 507;
    // int const  AVARAGE_VAL = 302;
    // int const  LOW_VAL = 97;

String const name = "name";
String const steps = "steps";
String const actionId = "actionId";
VectorInt16 aa, gyro;
VectorFloat gravity;
Quaternion q;

//WIFI CONFIG
const char* ssid = "Lisanderl";
const char* password = "123qwerty";
ESP8266WebServer server(85);
IPAddress myIP;
AngleSettings leftServo (90, 500);
AngleSettings rightServo (500, 90);
AngleSettings defaultServo (500, 90);

PCA9685 pwmController;
MoveController *moveController;

void interruptor(){
  mpuInterruption = true;
}

/**
 * if data are correct, do action and send status 200
 * if not, send 404
 */
void action(){
  //TO DO add getting json
  Serial.println("start action arg: ");
    if(server.args() > 0 ){
      String val = server.arg(0);
      DynamicJsonBuffer jsonBuffer(60);
      JsonObject& root = jsonBuffer.parseObject(val);
      root.prettyPrintTo(Serial);
      if (root.containsKey(name)) {
      Serial.println("start moving");
      server.send(200,  "text/plain", "Do some steps");
      int recivedSteps = root[steps].as<int>();
      int recivedActionId = root[actionId].as<int>();
      for(int i = 0; i < recivedSteps; i++){
       moveController->step(recivedActionId);
      }
      Serial.println("end moving");
      return;
     } 
    }
      Serial.println("Some error: ");
      server.send(404, "text/plain", "can't read data from JSON");
}
/**
 * load 
 */
bool loadFromSpiffs(String path){

  String dataType = "text/plain";
  if(path.endsWith("/")) path += "index.htm";
 
  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if(path.endsWith(".html")) dataType = "text/html";
  else if(path.endsWith(".htm")) dataType = "text/html";
  else if(path.endsWith(".css")) dataType = "text/css";
  else if(path.endsWith(".js")) dataType = "application/javascript";
  else if(path.endsWith(".png")) dataType = "image/png";
  else if(path.endsWith(".gif")) dataType = "image/gif";
  else if(path.endsWith(".jpg")) dataType = "image/jpeg";
  else if(path.endsWith(".ico")) dataType = "image/x-icon";
  else if(path.endsWith(".xml")) dataType = "text/xml";
  else if(path.endsWith(".pdf")) dataType = "application/pdf";
  else if(path.endsWith(".zip")) dataType = "application/zip";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (server.hasArg("download")) dataType = "application/octet-stream";
  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
  }
 Serial.print("path :");
 Serial.println(path);
  dataFile.close();
  return true;
}

void handleWebRequests(){

  if(loadFromSpiffs(server.uri())) return;
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println("message error");
  Serial.println(message);
}

void handleRoot(){

server.sendHeader("Location", "/index.html", true);
server.send ( 302, "text/plain", "");
}

void restConfig(){

  server.on("/", handleRoot);
  server.on("/action", HTTP_POST, action);
  server.onNotFound(handleWebRequests); 
  server.begin();
}

void setup() {
   
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security
  myIP = WiFi.softAPIP(); //Get IP address

  SPIFFS.begin();
  restConfig();
   Wire.begin(SDA1, SCL1);
   Wire.setClock(400000);
     delay(1000);
    Serial.println("Initializing I2C devices...");
    mpu.initialize();
    mpu.dmpInitialize(); 
    mpu.setIntEnabled(true);
    mpu.setDMPEnabled(true);
    packetSize = mpu.dmpGetFIFOPacketSize();
    delay(1000);
    pinMode(INTERRUPT_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), interruptor, RISING); 
    Serial.println("Testing device connections...");
    Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

   delay(1000);
   pwmController.resetDevices();      
   pwmController.init(B000000);       
   pwmController.setPWMFrequency(50);

   moveController = new MoveController(pwmController, leftServo, rightServo, defaultServo, 2);
   delay(500);
   moveController->defaultPosition(true);
}

void selfTest(){

  mpu.setAccelXSelfTest(true);
  mpu.setAccelYSelfTest(true);
  mpu.setAccelZSelfTest(true);

 Serial.print("Z accel factory val: ");
 Serial.println(mpu.getAccelZSelfTestFactoryTrim());

  Serial.print("X accel factory val: ");
 Serial.println(mpu.getAccelXSelfTestFactoryTrim());

  Serial.print("Y accel factory val: ");
 Serial.println(mpu.getAccelYSelfTestFactoryTrim());

  Serial.print("Z gyro factory val: ");
 Serial.println(mpu.getGyroZSelfTestFactoryTrim());

  Serial.print("X gyro factory val: ");
 Serial.println(mpu.getGyroXSelfTestFactoryTrim());

  Serial.print("Y gyro factory val: ");
 Serial.println(mpu.getGyroYSelfTestFactoryTrim());

// Z accel factory val: 19
// X accel factory val: 15
// Y accel factory val: 13
// Z gyro factory val: 21
// X gyro factory val: 15
// Y gyro factory val: 14

}

void aceelLoop(){

  //calculate temperature from MCU and print it
  float temp = (mpu.getTemperature()/340.0)+36.53;
  Serial.println("temperature = ");
  Serial.print(temp);
 uint8_t mpuIntStatus = mpu.getIntStatus();

 Serial.println("FF_BIT :");
 Serial.println(mpuIntStatus);
 Serial.println(mpuIntStatus & _BV(MPU6050_INTERRUPT_FF_BIT));
 
 Serial.println("MOT_BIT :");            
 Serial.println(mpuIntStatus & _BV(MPU6050_INTERRUPT_MOT_BIT));
 
 Serial.println("ZMOT_BIT :");           
 Serial.println(mpuIntStatus & _BV(MPU6050_INTERRUPT_ZMOT_BIT));
  
 Serial.println("FIFO_OFLOW_BIT :");          
 Serial.println(mpuIntStatus & _BV(MPU6050_INTERRUPT_FIFO_OFLOW_BIT));
  
 Serial.println("I2C_MST_INT_BIT :");    
 Serial.println(mpuIntStatus & _BV(MPU6050_INTERRUPT_I2C_MST_INT_BIT));

 Serial.println("PLL_RDY_INT_BIT :");   
 Serial.println(mpuIntStatus & _BV(MPU6050_INTERRUPT_PLL_RDY_INT_BIT));
 
 Serial.println("DMP_INT_BIT :");   
 Serial.println(mpuIntStatus & _BV(MPU6050_INTERRUPT_DMP_INT_BIT));
 
 Serial.println("DATA_RDY_BIT :");       
 Serial.println(mpuIntStatus & _BV(MPU6050_INTERRUPT_DATA_RDY_BIT));

 if (mpuIntStatus & _BV(MPU6050_INTERRUPT_DMP_INT_BIT)) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) 
        fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
       
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        mpu.dmpGetGyro(&gyro, fifoBuffer);
        Serial.println("gyro val = x y z");
        Serial.println(gyro.x);
        Serial.println(gyro.y);
        Serial.println(gyro.z);

        mpu.dmpGetAccel(&aa, fifoBuffer);

        Serial.println("aa val = x y z");
        Serial.println(aa.x);
        Serial.println(aa.y);
        Serial.println(aa.z);
        mpu.dmpGetGravity(&gravity, &q);
 }
}

void loop() {

  uint8_t mpuIntStatus = 0;
  server.handleClient();
 if(mpuInterruption){
   mpuIntStatus = mpu.getIntStatus();
  mpuInterruption = false;
 }
      if ((mpuIntStatus & _BV(MPU6050_INTERRUPT_FIFO_OFLOW_BIT)) || fifoCount >= 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        fifoCount = mpu.getFIFOCount();
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & _BV(MPU6050_INTERRUPT_DMP_INT_BIT)) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        mpu.dmpGetGyro(&gyro, fifoBuffer);
        Serial.println("gyro val = x y z");
        Serial.println(gyro.x);
        Serial.println(gyro.y);
        Serial.println(gyro.z);

        mpu.dmpGetAccel(&aa, fifoBuffer);

        Serial.println("aa val = x y z");
        Serial.println(aa.x);
        Serial.println(aa.y);
        Serial.println(aa.z);
        mpu.dmpGetGravity(&gravity, &q);
    }

} 




