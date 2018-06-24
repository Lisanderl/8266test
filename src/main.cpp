#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
 
MDNSResponder mdns;
 
// впишите сюда данные, соответствующие вашей сети:
const char* ssid = "ASUS";
const char* password = "sorenstr";
 
ESP8266WebServer server(80);

String webPage = "";
String json = "";
 
int gpio0_pin = 0;
int gpio2_pin = 2;
 
void setup(void){
  webPage += "<h1>Hello World</h1>";
 
 json += 
      "{\"value\": \"New\", \"onclick\": \"CreateNewDoc()\"}";
 
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // ждем соединения:
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");  //  "Подключились к "
  Serial.println(ssid);
  Serial.print("IP address: ");  //  "IP-адрес: "
  Serial.println(WiFi.localIP());
 
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
               //  "Запущен MDNSresponder"
  }
 
  server.on("/hello", [](){
    server.send(200, "text/html", webPage);
  });

    server.on("/json", [](){
    server.send(200, "application/json", json);
  });


  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
}