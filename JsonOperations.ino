
int createUser(String username, String password) {
  Serial.println("creo");
  if (!SPIFFS.begin(true)) {
    return 1;
  }

  if (SPIFFS.exists("/" + username + ".json")) {
    return 2;
  }

  StaticJsonDocument<2048> doc;
  doc["user"] = username;
  doc["password"] = password;
  JsonArray credentials = doc.createNestedArray("credentials");

  File file = SPIFFS.open("/" + username + ".json", FILE_WRITE);
  if (!file) {
    return 1;
  } else {
    serializeJsonPretty(doc, file);
    file.close();
  }
  Serial.println("CREATO");
  return 0;
}
int getUserPassword(String username) {
  if (!SPIFFS.begin(true)) {
    return 1;
  }

  DynamicJsonDocument doc(2048);
  File file = SPIFFS.open("/" + username + ".json");
  deserializeJson(doc, file);
  file.close();
  JsonObject user = doc.as<JsonObject>();

  if (user["user"] == username) {
    String pwd = user["password"];
    Serial.println(pwd);
    return 0;
  }


  return 2;
}
bool logUser(String username, String password) {
  if (!SPIFFS.begin(true)) {
    return false;
  }

  DynamicJsonDocument doc(2048);
  File file = SPIFFS.open("/" + username + ".json");
  deserializeJson(doc, file);
  file.close();
  JsonObject user = doc.as<JsonObject>();

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

int addPassword(String username, String password, String serviceName, String servicePassword) {
  if (!SPIFFS.begin(true)) {
    return 1;
  }

  if (!logUser(username, password)) {
    return 2;
  }

  StaticJsonDocument<2048> doc;
  File file = SPIFFS.open("/" + username + ".json");
  deserializeJson(doc, file);
  file.close();

  JsonObject credential = doc["credentials"].createNestedObject();
  credential["service"] = serviceName;
  credential["password"] = servicePassword;

  file = SPIFFS.open("/" + username + ".json", FILE_WRITE);
  if (!file) {
    return 1;
  } else {
    serializeJsonPretty(doc, file);
    file.close();
  }

  return 0;
}

int deletePassword(String username, String password, String serviceName) {
  if (!SPIFFS.begin(true)) {
    return 1;
  }

  if (!logUser(username, password)) {
    return 2;
  }

  StaticJsonDocument<2048> doc;
  File file = SPIFFS.open("/" + username + ".json");
  deserializeJson(doc, file);
  file.close();

  JsonArray credentials = doc["credentials"];
  for (JsonArray::iterator it = credentials.begin(); it != credentials.end(); ++it) {
    if ((*it)["service"] == serviceName) {
      credentials.remove(it);
      break;
    }
  }

  file = SPIFFS.open("/" + username + ".json", FILE_WRITE);
  if (!file) {
    return 1;
  } else {
    serializeJsonPretty(doc, file);
    file.close();
  }

  return 0;
}

String getPassword(String username, String password, String serviceName) {
  if (!SPIFFS.begin(true)) {
    return "";
  }

  if (!logUser(username, password)) {
    return "";
  }

  StaticJsonDocument<2048> doc;
  File file = SPIFFS.open("/" + username + ".json");
  deserializeJson(doc, file);
  file.close();

  JsonArray credentials = doc["credentials"];
  for (JsonArray::iterator it = credentials.begin(); it != credentials.end(); ++it) {
    if ((*it)["service"] == serviceName) {
      return (*it)["password"];
    }
  }

  return "";
}

void clearMemory() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  SPIFFS.format();
}
