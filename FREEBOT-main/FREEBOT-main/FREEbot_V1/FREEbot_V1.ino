#include <ESPAsyncUDP.h>
#include <ESP8266WiFi.h>  // Use ESP8266WiFi.h for ESP8266
#include <Arduino.h>      
#include "DataParser.h"   // Assumes DataParser.h and DataParser.cpp are in this sketch folder

// ------------------------------------
// 1. Access Point (AP) Credentials
// The ESP8266 will broadcast this Wi-Fi network.
// ------------------------------------
const char* ap_ssid = "FREEBOT_AP";     // Hotspot Name
const char* ap_password = "  "; // Hotspot Password (must be 8 chars minimum)
// Note: We ignore the old STA credentials (ssid/password) in AP mode.

// ------------------------------------
// 2. Pin Definitions for ESP8266 (NodeMCU v0.1)
// ------------------------------------
const int in1 = D6; // GPIO12 (Left Motor Input 1)
const int in2 = D5; // GPIO14 (Left Motor Input 2)
const int ena = D4; // GPIO2  (Left Motor Enable/PWM) 
const int in3 = D7; // GPIO13 (Right Motor Input 1)
const int in4 = D8; // GPIO15 (Right Motor Input 2)
const int enb = D1; // GPIO5  (Right Motor Enable/PWM)

// ------------------------------------
// 3. Global Variables and Objects
// ------------------------------------
DataParser dataParser; 
AsyncUDP udp;          

const int udpPort = 12345; // UDP port to listen on

int Speed = 50;      // Motor speed value (0-255 for 8-bit PWM)
int Right_speed = 0; 
int Left_speed = 0;  

// Function Prototypes
void forward(int left_speed, int right_speed);
void backward(int left_speed, int right_speed);
void left(int left_speed, int right_speed);
void right(int left_speed, int right_speed);
void Stop();

// ------------------------------------
// SETUP FUNCTION (AP MODE ACTIVATED HERE)
// ------------------------------------
void setup() {
  Serial.begin(115200);

  // Configure motor driver pins as outputs
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enb, OUTPUT);
  
  Stop(); // Ensure motors are off initially

  // --- START ACCESS POINT MODE ---
  WiFi.softAP(ap_ssid, ap_password);
  
  Serial.println("\n-------------------------------------");
  Serial.println("Access Point (Hotspot) Started");
  Serial.print("SSID: ");
  Serial.println(ap_ssid);
  Serial.print("Password: ");
  Serial.println(ap_password);
  Serial.print("Robot IP: ");
  // The ESP8266's IP address when in AP mode is typically 192.168.4.1
  Serial.println(WiFi.softAPIP()); 
  Serial.println("-------------------------------------");
  // --- END ACCESS POINT MODE ---


  // Start listening on the UDP port
  if(udp.listen(udpPort)) {
    Serial.print("UDP Listener active on port: ");
    Serial.println(udpPort);
    
    // Define the callback function to handle incoming UDP packets
    udp.onPacket( [](AsyncUDPPacket packet) {
      String IncomingData = (char*)packet.data(); 
      dataParser.parseData(IncomingData, ',');    

      int newSpeed = (dataParser.getField(1)).toInt(); 
      
      if (newSpeed > 255) newSpeed = 255; 
      if (newSpeed < 0) newSpeed = 0;
      Speed = newSpeed; 

      Left_speed = Speed;
      Right_speed = Speed;
    });
  }
}

// ------------------------------------
// LOOP FUNCTION
// ------------------------------------
void loop() {
      
  if(dataParser.getField(0) == "f") { 
    forward(Left_speed, Right_speed);
  }
  else if(dataParser.getField(0) == "b") { 
    backward(Left_speed, Right_speed);
  }
  else if(dataParser.getField(0) == "l") { 
    left(Left_speed, Right_speed);
  }
  else if(dataParser.getField(0) == "r") { 
    right(Left_speed, Right_speed);
  }
  else if(dataParser.getField(0) == "s") { 
    Stop();
  }
  
  delay(1); 
}

// ------------------------------------
// MOTOR CONTROL FUNCTIONS
// ------------------------------------

void forward(int left_speed, int right_speed) { 
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW); 
  digitalWrite(in3, HIGH); 
  digitalWrite(in4, LOW); 
  analogWrite(ena, left_speed); 
  analogWrite(enb, right_speed);
}

void backward(int left_speed, int right_speed) { 
  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH); 
  digitalWrite(in3, LOW); 
  digitalWrite(in4, HIGH); 
  analogWrite(ena, left_speed); 
  analogWrite(enb, right_speed);
}

void left(int left_speed, int right_speed) { 
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW); 
  digitalWrite(in3, LOW); 
  digitalWrite(in4, HIGH); 
  analogWrite(ena, left_speed); 
  analogWrite(enb, right_speed);
}

void right(int left_speed, int right_speed) { 
  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH); 
  digitalWrite(in3, HIGH); 
  digitalWrite(in4, LOW); 
  analogWrite(ena, left_speed); 
  analogWrite(enb, right_speed);
}

void Stop() { 
  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW); 
  digitalWrite(in3, LOW); 
  digitalWrite(in4, LOW); 
  analogWrite(ena, 0); // Stop PWM
  analogWrite(enb, 0); // Stop PWM
}
