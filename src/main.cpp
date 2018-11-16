
#include <Arduino.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h> 
#include <ArduinoJson.h> 
#include "MPU6050_6Axis_MotionApps20.h"

#include <Wire.h>
#include <PCA9685.h>
#include <AngleSettings.h>
#include "Pad.h"
#include "MoveController.h"

MPU6050 mpu;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
int const INTERRUPT_PIN = 7;
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

void dmpDataReady() {
    mpuInterrupt = true;
}
void accelConfig(){

  Serial.println("Initializing I2C devices...");
   pinMode(INTERRUPT_PIN, INPUT);
  mpu.initialize();
  delay(1000);
  Serial.println("Testing device connections...");
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
 devStatus = mpu.dmpInitialize();
   // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    if (devStatus == 0) {
    // turn on the DMP, now that it's ready
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MPU control/status vars
String const name = "name";
String const steps = "steps";
String const actionId = "actionId";

//WIFI CONFIG
const char* ssid = "Lisanderl";
const char* password = "123qwerty";
ESP8266WebServer server(85);
IPAddress myIP;
AngleSettings leftServo (97, 507);
AngleSettings rightServo (507, 97);

PCA9685 pwmController;
MoveController *moveController;

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

  Wire.begin(2, 14);
  Wire.setClock(400000);
  delay(1000);
  accelConfig();
  delay(1000);
  //  pwmController.resetDevices();      
  //  pwmController.init(B000000);       
  //  pwmController.setPWMFrequency(50);

   moveController = new MoveController(pwmController, leftServo, rightServo, 2);
   delay(500);
   moveController->defaultPosition(true);
   delay(3000);
   Serial.println("Start working........");
}


void mpu_loop()
{
  // if programming failed, don't try to do anything
  if (!dmpReady) return;

  // wait for MPU interrupt or extra packet(s) available
  if (!mpuInterrupt && fifoCount < packetSize) return;

  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // get current FIFO count
  fifoCount = mpu.getFIFOCount();

  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (mpuIntStatus & 0x02) {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);

    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;

    // display quaternion values in easy matrix form: w x y z
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    Serial.print("quat\t");
    Serial.print(q.w);
    Serial.print("\t");
    Serial.print(q.x);
    Serial.print("\t");
    Serial.print(q.y);
    Serial.print("\t");
    Serial.println(q.z);

  }
}

void loop() {
 // server.handleClient();


 mpu_loop();
}



