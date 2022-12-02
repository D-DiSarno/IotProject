#include <StreamUtils.h>

#include"Credential.h"
#include <ArduinoJson.h>
#include <EEPROM.h>

StaticJsonDocument<512>doc;
//wifi>
int address = 0;
EepromStream eepromStream(address, 512);
 void store();
 void read();
Credential credenziali [2];
void getUserName();