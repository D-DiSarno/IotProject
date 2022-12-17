#include <ArduinoJson.h>

#include "FS.h"
#include "SPIFFS.h"

struct Credential {
  String serviceName;
  String servicePassword;
};

struct User {
  String username;
  String password;
  Credential credentials[];
};

int createUser(String username, String password) {
  if (!SPIFFS.begin(true)) {
    return 1;
  }

  if (SPIFFS.exists("/" + username + ".json")) {
    return 2;
  }

  User user;
  user.username = username;
  user.password = password;

  StaticJsonDocument<2048> docUser;
  JsonObject userObject = docUser.to<JsonObject>();
  userObject["user"] = user.username;
  userObject["password"] = user.password;

  StaticJsonDocument<2048> docCredentials;
  JsonArray credentialsObject = docCredentials.to<JsonArray>();
  userObject["credentials"] = credentialsObject;

  File file = SPIFFS.open("/" + username + ".json", FILE_WRITE);
  if (!file) {
    return 1;
  } else {
    serializeJsonPretty(docUser, file);
    file.close();
  }

  return 0;
}

bool logUser(String username, String password) {
  if (!SPIFFS.begin(true)) {
    return false;
  }

  DynamicJsonDocument docUser(2048);
  File file = SPIFFS.open("/" + username + ".json");
  deserializeJson(docUser, file);
  file.close();
  JsonObject user = docUser.as<JsonObject>();

  if (user["password"] == password) {
    return true;
  }

  return false;
}

int removeUser(String username) {
  if (!SPIFFS.begin(true)) {
    return 1;
  }

  if (!SPIFFS.remove("/" + username + ".json")) {
    return 1;
  }

  return 0;
}

int storePassword(String username, String password, String serviceName, String servicePassword) {
  if (!SPIFFS.begin(true)) {
    return 1;
  }

  if (!logUser(username, password)) {
    return 2;
  }

  DynamicJsonDocument docUser(2048);
  File file1 = SPIFFS.open("/" + username + ".json");
  deserializeJson(docUser, file1);
  file1.close();
  JsonObject user = docUser.as<JsonObject>();

  //JsonObject credentials = user["credentials"];
  DynamicJsonDocument docCredentials(2048);
  deserializeJson(docCredentials, user["credentials"]);
  JsonObject credentials = docCredentials.to<JsonObject>();
  credentials[serviceName] = servicePassword;
  user["credentials"] = credentials;

  File file2 = SPIFFS.open("/" + username + ".json", FILE_WRITE);
  if (!file2) {
    return 1;
  } else {
    serializeJsonPretty(docUser, file2);
    file2.close();
  }

  return 0;  
}

int deletePassword(String username, String password, String serviceName) {
  if (!SPIFFS.begin(true)) {
    return 1;
  }

  DynamicJsonDocument docUser(2048);
  File file = SPIFFS.open("/" + username + ".json");
  deserializeJson(docUser, file);
  file.close();
  JsonObject user = docUser.to<JsonObject>();

  if (user["password"] != password) {
    return 1;
  }

  JsonObject credentials = user["credentials"];
  credentials.remove(serviceName);

  file = SPIFFS.open("/" + username + ".json", FILE_WRITE);
  if (!file) {
    return 1;
  } else {
    serializeJsonPretty(docUser, file);
    file.close();
  }

  return 0;
}

String getPassword(String username, String password, String serviceName) {
  if (!SPIFFS.begin(true)) {
    return "";
  }

  DynamicJsonDocument docUser(2048);
  File file = SPIFFS.open("/" + username + ".json");
  deserializeJson(docUser, file);
  file.close();
  JsonObject user = docUser.to<JsonObject>();

  if (user["password"] != password) {
    return "";
  }

  JsonObject credentials = user["credentials"];
  return credentials[serviceName];
}

void clearMemory() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  SPIFFS.format();
}