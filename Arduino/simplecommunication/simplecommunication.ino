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

void setup()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
// Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
  pulseSensor.begin();
  Udp.begin(42069);
}

void loop()
{

if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
int myBPM = pulseSensor.getBeatsPerMinute();  
char BPM[4];
itoa(myBPM, BPM, 10);
// Calls function on our pulseSensor object that returns BPM as an "int".
  Udp.beginPacket("192.168.137.1", 12000);
  Udp.write(BPM);
  Udp.endPacket();                                             // "myBPM" hold this BPM value now. 

}

  delay(20); 
  
}
