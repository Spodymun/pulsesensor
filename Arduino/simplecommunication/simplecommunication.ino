#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

const char* ssid = "schnelly";
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

//Main Schleife, misst und sendet Daten
void loop() {
  if (pulseSensor.sawStartOfBeat()) {
    int myBPM = pulseSensor.getBeatsPerMinute();
    Serial.println(myBPM);
    sendToPC(myBPM);
  }
  delay(20); 
}

// -------------

//Soll Verbindung zum Wlan herstellen
void connectToWifi() {
  WiFi.begin(ssid, password);
  waitUntilConnectionIsAvailable();
}

//Überprüft ob die Verbindung aktuell gegeben ist
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

//UDP Begin muss aufgerufen werden, wird jedoch nicht benötigt
void initSocket() {
  Udp.begin(42069);
}

void sendToPC(int bpm) {
  char bpm_as_char_array[4];
  itoa(bpm, bpm_as_char_array, 10); //Konvertiert int to char[]
  Udp.beginPacket("192.168.137.1", 12000);  // Hard coded ip und port. Wir wissen die IP-Adresse, weil der Pc immer der Host ist
  Udp.write(bpm_as_char_array);
  Udp.endPacket();
}