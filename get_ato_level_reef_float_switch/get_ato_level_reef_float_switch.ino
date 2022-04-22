/**
   senses the water level in ato/ RODI water plastic jar kept along with fish tank
*/
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

const int relay = 13;
int floatHight = 4;
int floatLow = 5;
int buttonStateHigh;
int buttonStateLow;
WiFiClient  client;


unsigned long myChannelNumber = 1470925;
const char * myWriteAPIKey = "L3XHRLH063H7YA7Q";

const char* ssid = "DAS_RESIDENCE";   // your network SSID (name)
//const char* ssid = "IOT";   // your network SSID (name)
const char* password = "edscoe123";   // your network password

void setup() {
  Serial.begin(115200);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print("...");
      WiFi.begin(ssid, password);
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak

  pinMode(floatHight, INPUT_PULLUP); //Arduino Internal Resistor 10K
  pinMode(floatLow, INPUT_PULLUP); //Arduino Internal Resistor 10K
  pinMode(relay, OUTPUT);
  //     valve is closed by default, normally closed valve
  digitalWrite(relay, HIGH);
}

void loop() {

  buttonStateHigh = digitalRead(floatHight);
  buttonStateLow = digitalRead(floatLow);

  if (buttonStateHigh == LOW)
  {
    Serial.println( "buttonStateHigh -- LOW");
  }
  if (buttonStateHigh == HIGH)
  {
    Serial.println( "buttonStateHigh -- HIGH");
  }
  if (buttonStateLow == LOW)
  {
    Serial.println( "buttonStateLow -- LOW");
  }
  if (buttonStateLow == HIGH)
  {
    Serial.println( "buttonStateLow -- HIGH");
  }

  // open valve to let water flow when low and high float switch are at high state. (tank dry)
  if (buttonStateHigh == HIGH && buttonStateLow == HIGH )
  {
    digitalWrite(relay, LOW);
    Serial.println("valve ON");
  }
  // close valve when both float switch wet
  if (buttonStateHigh == LOW && buttonStateLow == LOW )
  {
    digitalWrite(relay, HIGH);
    Serial.println("valve OFF");
  }

  ThingSpeak.setField(1, buttonStateHigh);
  ThingSpeak.setField(2, buttonStateLow);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (x == 200) {
    Serial.println("Channel update successful.");
  }
  if (x != 200) {
    Serial.println("Problem updating channel. HTTP error code " + String(x));

  }


  delay(5000);
}
