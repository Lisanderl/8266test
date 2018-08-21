
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <Wire.h>
#include "PCA9685.h"
#include "Pad.h"
#include "MoveController.h"

//WIFI CONFIG
const char* ssid = "Lisanderl";
const char* password = "12345";
ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!!!!!!!!!!!");
}


PCA9685 pwmController;
// default 102 307 512
PCA9685_ServoEvaluator pwmServo1(97, 302, 507);

Pad simplePad1(pwmController, pwmServo1, 0, 1);
Pad simplePad2(pwmController, pwmServo1, 2, 3);



void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security
  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  Serial.print("HotSpt IP:");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
   Wire.begin(2, 14);
   Wire.setClock(400000);
   delay(1000);
    pwmController.resetDevices();      
    pwmController.init(B000000);       
    pwmController.setPWMFrequency(50);
}

void loop() {
   server.handleClient();
  Serial.println("all ok");
simplePad1.smartHorisontalMove(40);
 delay(2000);
simplePad1.horisontalMove(-60);
 delay(5000);
simplePad2.smartHorisontalMove(35);
 delay(2000);
simplePad2.horisontalMove(-55);
 delay(5000);
}



