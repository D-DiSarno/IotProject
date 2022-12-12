#include <StreamUtils.h>

#include"Credential.h"
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

String card1 = "FFFFFFFF";  // Change this value to the UID of your card.
MFRC522 mfrc522(SS_PIN, RST_PIN);

StaticJsonDocument<512>doc;
//wifi>
int address = 0;
EepromStream eepromStream(address, 512);
 void store();
 void read();
Credential credenziali [2];
void clear();
void getUser();
void createUser(String,String );
void checkRFID();
void deleteUser(String);



 JsonArray users = doc.to<JsonArray>();