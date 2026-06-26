/*  
  Sources:
Based on code from https://RandomNerdTutorials.com/esp32-web-server-beginners-guide/ 
Smart Home QE007 Starter Guide
*/


#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>






const char* ssid = "ESP32_ACCESS_POINT";
const char* password = "pass123456";
const int WS2812_PIN = 16;
const int WS2812_NUM = 9;


Adafruit_NeoPixel pixels(WS2812_NUM, WS2812_PIN, NEO_GRB + NEO_KHZ800);
String rgbState = "off";






// Assign output variables to GPIO pins
const int light = 5;
String lightState = "off";


Servo windowServo;
const int windowPin = 17;
const int WinangleOpen = 123;
const int WinangleClose = 60;
String windowState = "off";


Servo doorServo;
const int doorPin = 18;
const int DoorangleOpen = 100;
const int DoorangleClose = 5;
String doorState = "off";


const int laserPin = 23;
String laserState = "off";


// Colors
int WS2812_COLOR_RED = pixels.Color(255, 0, 0);
int WS2812_COLOR_BLUE = pixels.Color(0, 0, 255);
int WS2812_COLOR_GREEN = pixels.Color(0, 255, 0);
int WS2812_COLOR_WHITE = pixels.Color(255, 255, 255);
int WS2812_COLOR_YELLOW = pixels.Color(255, 255, 0);
int WS2812_COLOR_MAGENTA = pixels.Color(255, 0, 255);
int WS2812_COLOR_CYAN = pixels.Color(0, 255, 255);
int WS2812_COLOR_BLACK = pixels.Color(0, 0, 0);


// helper function
void ws2812ShowColor(uint32_t color)
{
  for (int i = 0; i < WS2812_NUM; i++) pixels.setPixelColor(i, color);
  pixels.show();
}




WebServer server(80);




void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  html += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".button2 { background-color: #555555; }</style></head>";
  html += "<body><h1>ESP32 Web Server</h1>";


   // Display light controls
  html += "<p>Light - State " + lightState + "</p>";
  if (lightState == "off") {
    html += "<p><a href=\"/light/on\"><button class=\"button\">LED ON</button></a></p>";
  } else {
    html += "<p><a href=\"/light/off\"><button class=\"button button2\">LED OFF</button></a></p>";
  }




 
   // Display window controls - make window and door sliders?
  html += "<p>Window - State " + windowState + "</p>";
  if (windowState == "off") {
    html += "<p><a href=\"/window/open\"><button class=\"button\">WINDOW OPEN</button></a></p>";
  } else {
    html += "<p><a href=\"/window/close\"><button class=\"button button2\">WINDOW CLOSE</button></a></p>";
  }


  // Display door controls - make window and door sliders? - ISSUE?
  html += "<p>Door - State " + doorState + "</p>";
  if (doorState == "off") {
    html += "<p><a href=\"/door/open\"><button class=\"button\">DOOR OPEN</button></a></p>";
  } else {
    html += "<p><a href=\"/door/close\"><button class=\"button button2\">DOOR CLOSE</button></a></p>";
  }


  // Display laser controls
  html += "<p>Laser - State " + laserState + "</p>";
  if (laserState == "off") {
    html += "<p><a href=\"/laser/on\"><button class=\"button\">LASER ON</button></a></p>";
  } else {
    html += "<p><a href=\"/laser/off\"><button class=\"button button2\">LASER OFF</button></a></p>";
  }


  // Display RGB controls
  html += "<p>RGB - State " + rgbState + "</p>";
  if (rgbState == "off") {
    html += "<p><a href=\"/rgb/on\"><button class=\"button\">RGB ON</button></a></p>";
    html += "<p><a href=\"/rgb/rainbow\"><button class=\"button\">RAINBOW</button></a></p>";
  } else {
    html += "<p><a href=\"/rgb/off\"><button class=\"button button2\">RGB OFF</button></a></p>";
  }




  html += "</body></html>";
  server.send(200, "text/html", html);
}


void test() {
  handleRoot();
}


// Function to handle turning light on
void lightOn() {
  lightState = "on";
  digitalWrite(light, HIGH);
  handleRoot();
}




// Function to handle turning light off
void lightOff() {
  lightState = "off";
  digitalWrite(light, LOW);
  handleRoot();
}


// Function to handle turning laser on
void laserOn() {
  laserState = "on";
  digitalWrite(laserPin, HIGH);
  handleRoot();
}




// Function to handle turning laser off
void laserOff() {
  laserState = "off";
  digitalWrite(laserPin, LOW);
  handleRoot();
}


// Function to handle turning rgb on
void rgbOn() {
  rgbState = "on";
  ws2812ShowColor(WS2812_COLOR_BLUE);
  handleRoot();
}


// happy pride??
void rainbow()
{
  rgbState = "on";
  // array - pick at random
  int colors[] = {WS2812_COLOR_RED, WS2812_COLOR_YELLOW, WS2812_COLOR_GREEN, WS2812_COLOR_BLUE,WS2812_COLOR_MAGENTA,WS2812_COLOR_CYAN, WS2812_COLOR_WHITE};
  int randIndex = random(0, 6);
  ws2812ShowColor(colors[randIndex]);
  handleRoot();


}


// Function to handle turning rgb off
void rgbOff() {
  rgbState = "off";
   ws2812ShowColor(pixels.Color(0, 0, 0));
  handleRoot();
}


// Function to handle opening window - original code
void windowOpen() {
  windowState = "on";
  windowServo.write(WinangleOpen);
  handleRoot();
}


// Function to handle closing window
void windowClose() {
  windowState = "off";
  windowServo.write(WinangleClose);
  handleRoot();
}


// Function to handle opening window - original code
void doorOpen() {
  doorState = "on";
  doorServo.write(DoorangleOpen);
  handleRoot();
}


// Function to handle closing door
void doorClose() {
  doorState = "off";
  doorServo.write(DoorangleClose);
  handleRoot();
}


void setup() {
  Serial.begin(115200);


  // Initialize the output variables as outputs
  pinMode(light, OUTPUT);
  windowServo.attach(windowPin);
  doorServo.attach(doorPin);




  // Set outputs to LOW
  digitalWrite(light, LOW);


  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  delay(5000);
  IPAddress IP = WiFi.softAPIP();


  Serial.println(IP);


 
  server.on("/", handleRoot);
  server.on("/light/on", lightOn);
  server.on("/light/off", lightOff);
  server.on("/laser/on", laserOn);
  server.on("/laser/off", laserOff);
  server.on("/window/open", windowOpen);
  server.on("/window/close", windowClose);
  server.on("/door/open", doorOpen);
  server.on("/door/close", doorClose);
  server.on("/rgb/on", rgbOn);
  server.on("/rgb/off", rgbOff);
  server.on("/rgb/rainbow", rainbow);






  server.begin();


}


// COM port suddenly disconnects? - version esptool v5.3.0 - use COM Ports 4, 6, or 7
void loop()
{
  delay(5000);
  IPAddress IP = WiFi.softAPIP();
  Serial.println(IP);


  server.handleClient();




}





