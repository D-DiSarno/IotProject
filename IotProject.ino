#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>

#define SS_PIN  5  // ESP32 pin GIOP5 
#define RST_PIN 27 // ESP32 pin GIOP27 

String card1 = "FFFFFFFF";  // Change this value to the UID of your card.
MFRC522 mfrc522(SS_PIN, RST_PIN);

<<<<<<< HEAD
StaticJsonDocument<512> doc;

=======
/*
StaticJsonDocument<512>doc;
>>>>>>> bfff6bbe4f62b02b2ab951c6d8f12206ba9426dd
//wifi>
const int RED_LED_PIN = 32;//test
const char* ssid = "FIBRA-DS";
const char* password = "Danielino*2!";
void connectWifi();

int address = 0;
//EepromStream eepromStream(address, 512);
void store();
void read();
void clear();
void getUser();
//void createUser(String, String);
void checkRFID();
void deleteUser(String);



JsonArray users = doc.to<JsonArray>();