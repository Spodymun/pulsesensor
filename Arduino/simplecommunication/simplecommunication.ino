#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

const char* ssid = "Schnelly";
const char* password = "essenkochen23";

WiFiUDP Udp;

const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED = LED_BUILTIN;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.

PulseSensorPlayground pulseSensor;  

void setup() {
  connectToWifi();
  setupPulseSensor();
  initSocket();
}

void loop() {
  if (pulseSensor.sawStartOfBeat()) {
    int myBPM = pulseSensor.getBeatsPerMinute();
    sendToPC(myBPM);
  }
  delay(20); 
}

// -------------

void connectToWifi() {
  WiFi.begin(ssid, password);
  waitUntilConnectionIsAvailable();
}

void waitUntilConnectionIsAvailable() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void setupPulseSensor() {
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED);
  pulseSensor.setThreshold(Threshold);
  pulseSensor.begin();
}

void initSocket() {
  Udp.begin(42069);
}

void sendToPC(int bpm) {
  char bpm_as_char_array[4];
  itoa(bpm, bpm_as_char_array, 10); //Convert int to char[]
  Udp.beginPacket("192.168.137.1", 12000);  // Hard coded ip and port. We know IP-Address because pc is alwas host
  Udp.write(bpm_as_char_array);
  Udp.endPacket();
}