
#include <Arduino.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <FS.h> 

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
ESP8266WebServer restController(80);
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

  if(restController.hasArg("action")){
    StaticJsonBuffer<50> jsonBuffer;
    JsonObject& jsonBody = jsonBuffer.parseObject(restController.arg("action"));
    if((jsonBody.containsKey(actionId)) && (jsonBody.containsKey(steps))){
      Serial.println("start mooving");
      restController.send(200);
      for(int i = 0; i<jsonBody.get<int>(steps); i++){
      moveController.step(jsonBody.get<int>(actionId));
      }
      return;
    } 
}
restController.send(404, "text/plain", "can't read data from JSON");
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
  if (restController.hasArg("download")) dataType = "application/octet-stream";
  if (restController.streamFile(dataFile, dataType) != dataFile.size()) {
  }
 Serial.print("path :");
 Serial.println(path);
  dataFile.close();
  return true;
}

void handleWebRequests(){

  if(loadFromSpiffs(restController.uri())) return;
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += restController.uri();
  message += "\nMethod: ";
  message += (restController.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += restController.args();
  message += "\n";
  for (uint8_t i=0; i<restController.args(); i++){
    message += " NAME:"+restController.argName(i) + "\n VALUE:" + restController.arg(i) + "\n";
  }
  restController.send(404, "text/plain", message);
  Serial.println(message);
}

void handleRoot(){

  restController.sendHeader("Location", "/index.html", true);   //Redirect to our html web page
  restController.send(200, "text/plane","");
}

void restConfig(){

  restController.on("/", handleRoot);
  restController.on("/action", HTTP_POST, action);
  restController.onNotFound(handleWebRequests); 
  restController.begin();
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

   restController.handleClient();
// simplePad1.smartHorisontalMove(40);
//  delay(2000);
// simplePad1.horisontalMove(-60);
//  delay(5000);
// simplePad2.smartHorisontalMove(35);
//  delay(2000);
// simplePad2.horisontalMove(-55);

//  File f = SPIFFS.open("/logic.js", "r");
//  Serial.println(f.size());
//  Serial.println(f);
//  f.close();
}



