#include <Servo.h>  
#include "SSD1306Wire.h"
#include "Wire.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

const int buttonPin = 14; 
int buttonState = 0; 
//web2017
const char* ssid = "smart-k";
const char* password = "parkkipirkko";
ESP8266WebServer server(80);
//näyttö
const int I2C_DISPLAY_ADDRESS = 0x3c;
SSD1306Wire     display(I2C_DISPLAY_ADDRESS, D2, D1);
//servo
#define servo1Pin 2
Servo servo1;
int currentAngle = 0;
char angleChar[10];

void setup() {
 displaySetup();
 servo1.attach(servo1Pin);

    //webbi servu
    WiFi.softAP(ssid, password);
    IPAddress apip = WiFi.softAPIP();
    server.on("/", handleRoot);
    server.on("/ServoPlus", servoPlus);
    server.on("/ServoMinus", servoMinus);
    server.begin();
}

//webbisivu

void handleRoot() {
    response();
}

const String HtmlHtml =   "<html><head>"
                          "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /></head>";
const String HtmlHtmlClose = "</html>";
const String HtmlTitle =  "<h1>Parkkikiekko\n </h1><br/>\n";
const String HtmlButtons = 
                          "<a href=\"ServoPlus\"><button style=\"display: block; width: 100%;\">+</button></a><br/>"
                          "<a href=\"ServoMinus\"><button style=\"display: block; width: 100%;\">-</button></a><br/>";

void response(){
  String htmlRes = HtmlHtml + HtmlTitle;
  htmlRes += HtmlButtons;
  htmlRes += HtmlHtmlClose;

  server.send(200, "text/html", htmlRes);
}

//end webbisivu

//servo aliohjelmat

void servoPlus(){
  currentAngle += 10;
  handleDevices();
}

void servoMinus(){
  currentAngle -= 10;
  handleDevices();
}

void handleDevices(){
  display.drawString(5,13, "     "); 
  sprintf(angleChar, "%d", currentAngle);
  servo1.write(currentAngle);
  response();
  display.drawString(5,13, angleChar);       
  display.clear(); 
  display.display(); 
  }

  
//servo loppu

void loop() {
  server.handleClient();
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
  servoPlus();
  delay(1000);
  }
}

void displaySetup()
{
  display.init();         
  display.clear(); 
  display.drawString(5,0, "Current time: ");       
  display.display();  
}
