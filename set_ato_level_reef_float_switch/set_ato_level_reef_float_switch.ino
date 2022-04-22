#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

WiFiClient  client;

const int relay = 13;
int statusCode = 0;
unsigned long myChannelNumber = 1470925;
const char * myReadAPIKey = "CBC7WDBYE1FB6K5Z";
const char* ssid = "DAS_RESIDENCE_ext_2.4";   // your network SSID (name)
const char* password = "edscoe123";   // your network password

void setup() {
  Serial.begin(115200); // Starts the serial communication
  // Connect or reconnect to WiFi
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
  pinMode(relay, OUTPUT);
  // make sure pump is off initially
  digitalWrite(relay, HIGH);
}

void loop() {
  int highFloatSwitch =  ThingSpeak.readLongField(myChannelNumber, 1, myReadAPIKey);
  int lowFloatSwitch =  ThingSpeak.readLongField(myChannelNumber, 2, myReadAPIKey);
  statusCode = ThingSpeak.getLastReadStatus();

  if (statusCode == 200)
  {

    // pump on: lower float switch is dry and upper float switch is dry
    if (lowFloatSwitch >= 1 && highFloatSwitch >= 1)
    {
      digitalWrite(relay, LOW);
      Serial.println("pump ON: case 1");
    }

    // pump on: lower float switch is wet and upper float switch is dry
//    if (lowFloatSwitch < 1 && highFloatSwitch >= 1)
//    {
//      digitalWrite(relay, LOW);
//      Serial.println("pump ON: case 2");
//    }


    // pump off: lower float switch wet and upper float switch wet
    if (lowFloatSwitch < 1 && highFloatSwitch < 1)
    {
      digitalWrite(relay, HIGH);
      Serial.println("pump OFF: case 3");
    }

    Serial.println(" ");
    Serial.print("Upper Float Switch: ");
    Serial.print(highFloatSwitch);
    Serial.println(" ");
    Serial.print("Lower Float Switch: ");
    Serial.print(lowFloatSwitch);
    Serial.println(" ");


  }
  else
  {
    digitalWrite(relay, HIGH);
    Serial.println("pump OFF for safety");
    Serial.println("Unable to read channel / No internet connection");
  }
  delay(1000);
}
