
#include <Arduino.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h> 
#include <ArduinoJson.h> 

#include <Wire.h>
#include "PCA9685.h"
#include "Pad.h"
#include "MoveController.h"

String const name = "name";
String const steps = "steps";
String const actionId = "actionId";

//WIFI CONFIG
const char* ssid = "Lisanderl";
const char* password = "12345ASD";
ESP8266WebServer server(85);
IPAddress myIP;


PCA9685 pwmController;
// default 102 307 512
PCA9685_ServoEvaluator pwmServo1(97, 302, 507);

Pad simplePad1(pwmController, pwmServo1, 0, 1);
Pad simplePad2(pwmController, pwmServo1, 2, 3);

MoveController moveController(2);

/**
 * if data are correct, do action and send status 200
 * if not, send 404
 */
void action(){
  //TO DO add getting json
  Serial.println("start action arg: ");
  Serial.println(server.args());
    if(server.args() > 0 ){
      StaticJsonBuffer<50> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(server.arg(1));
      if (root.success()) {
   
      Serial.println("start moving");
      server.send(200,  "text/plain", "Do some steps");
      int recivedSteps = root[steps].as<int>();
      int recivedActionId = root[actionId].as<int>();
      for(int i = 0; i < recivedSteps; i++){
      moveController.step(recivedActionId);
      }
      Serial.println("end moving");
      return;
     } 
    }
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
   pwmController.resetDevices();      
   pwmController.init(B000000);       
   pwmController.setPWMFrequency(50);
}

void loop() {
  delay(2000);
   //server.handleClient();
  Serial.println("Horisontal");
 simplePad1.horisontalMove(-40);
 delay(1000);
 simplePad1.horisontalMove(40);
 delay(2000);
   Serial.println("Vertical");
 simplePad1.verticalMove(-40);
 delay(1000);
 simplePad1.verticalMove(40);
 delay(2000);
   Serial.println("Smart");
 simplePad1.smartHorisontalMove(-40);
 delay(1000);
 simplePad1.smartHorisontalMove(40);
 delay(2000);
   Serial.println("End");
}



