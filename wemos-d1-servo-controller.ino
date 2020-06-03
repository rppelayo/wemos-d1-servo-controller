/*
WeMos D1 Servo Controller
by Roland Pelayo
for TeachMeMicro

Rev 1.0 - June 1, 2020

Full tutorial on https://www.teachmemicro.com/wemos-d1-servo-controller
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "mainpage.h"
#include "jscript.h"
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
                // twelve servo objects can be created on most boards
                

#ifndef STASSID
#define STASSID "PLDTHOMEFIBRjVZpa"
#define STAPSK  "stacey0921"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

void setup(void) {
  myservo.attach(D4);  // attaches the servo on GPIO2 to the servo object
  //For debugging
  Serial.begin(115200);
  //Use WeMos D1 as WiFi Station
  WiFi.mode(WIFI_STA);
  //Initiate WiFi Connection
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  //Print your WiFi's SSID (might be insecure)
  Serial.println(ssid);
  Serial.print("IP address: ");
  //Print your local IP address (needed for browsing the app)
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  
  //Home page. Contents of 'page' is in mainpage.h
  server.on("/", []() {
   server.send(200, "text/html", page);
  });
  //JavaScript! Contents of 'javascript' is in jscript.h
  server.on("/jscript.js", []() {
   server.send(200, "text/javascript", javascript);
  });
  //POST data handler
  server.on("/dataHandler",HTTP_POST,dataHandler);
  //start web server
  server.begin();
  //Just stating things
  Serial.println("HTTP server started");
}

//function for handling POST data
void dataHandler(){
  String angle = server.arg("plain");    //angle from POST data
  Serial.print("Received: ");            //print to serial terminal
  Serial.println(angle);
  server.sendHeader("Location","/");     //redirect client to home page
  server.send(303);                      //redirect http code
  int pos = angle.toInt();               //received angle is string so convert to integer
  myservo.write(pos);                    //move servo to received angle
  delay(100);
}

void loop(void) {
  //Make the WeMos D1 always handle web clients
  server.handleClient();
  MDNS.update();
}
