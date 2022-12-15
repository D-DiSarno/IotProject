#include <ArduinoJson.h>

#include "FS.h"
#include "SPIFFS.h"

int createUser(String username, String password) {
  if (!SPIFFS.begin(true)) {
    return 1;
  }

  if (SPIFFS.exists("/" + username + ".json")) {
    return 2;
  }

  DynamicJsonDocument docUser(2048);
  JsonObject user = docUser.to<JsonObject>();
  user["user"] = username;
  user["password"] = password;

  DynamicJsonDocument docCredentials(2048);
  JsonObject credentials = docCredentials.to<JsonObject>();
  user["credentials"] = credentials;

  File file = SPIFFS.open("/" + username + ".json", FILE_WRITE);
  if (!file) {
    return 1;
  } else {
    serializeJsonPretty(docUser, file);
    file.close();
  }

  return 0;
}

int removeUser() {}

void storePassword() {}

void deletePassword() {}

String getPassword() {}

void testStore() {

  // JsonArray users = doc.to<JsonArray>();

  StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc1;
  JsonObject user_1 = doc1.to<JsonObject>();
  user_1["user"] = "Mario";
  user_1["password"] = "aaabbbccc";
  users.add(user_1);

  StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc2;
  JsonObject user_2 = doc2.to<JsonObject>();
  user_2["user"] = "Luigi";
  user_2["password"] = "12345";
  users.add(user_2);

  serializeJson(doc, eepromStream);
  Serial.println("STORE-FATTO");
}

void clearData() {
  deserializeJson(doc, eepromStream);
  doc.clear();
}

/*
void read() {
  //StaticJsonDocument<500> doc;
  EepromStream eepromStream(0, 500);
  deserializeJson(doc, eepromStream);



  serializeJsonPretty(doc, Serial);
}

void getUser(String filter) {

  deserializeJson(doc, eepromStream);
  JsonArray users = doc.as<JsonArray>();
  Serial.println("array riempito");
  //String name=users[0]["user"];Serial.println(name);
  for (int i = 0; i < users.size(); i++) {
    String name = users[i]["user"];
    if (name == filter) {
      Serial.println(name);
      String password = users[i]["password"];
      Serial.println(password);
    }
  }
}

void createUser(String name, char * password) {
  //
  
 // deserializeJson(doc, eepromStream);
  StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc1;
  JsonObject user_1 = doc1.to<JsonObject>();
  user_1["user"]="NAME";
  //user_1["password"].set(password);
  //users.add(user_1);
  Serial.println(user_1.size());
  //users.add(user_1);
Serial.println(name);
  Serial.println("USER AGGIUNTO:");
  Serial.println(name);
  Serial.println(password);
  // serializeJson(doc, eepromStream);
}

void deleteUser(String filter) {

  deserializeJson(doc, eepromStream);
  JsonArray users = doc.as<JsonArray>();
  Serial.println("array riempito");
  //String name=users[0]["user"];Serial.println(name);

  for (int i = 0; i < users.size(); i++) {
    String name = users[i]["user"];

    if (name == filter) {

      users.remove(i);
    }
  }
  serializeJson(doc, eepromStream);
}

//modify
//duplicati

/* 
[
  {
    "user": "Mario",
    "password": "5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8",
    "credentials": {
      "google": "abc123",
      "facebook": "abc456",
      "amazon": "abc789"
    }
  }
]
*/