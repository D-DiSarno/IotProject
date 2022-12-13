void createUser(String username, String password) {
  StaticJsonDocument<JSON_OBJECT_SIZE(5)> docUser;
  JsonObject user = docUser.to<JsonObject>();
  user["user"] = username;
  user["password"] = password;

  StaticJsonDocument<JSON_OBJECT_SIZE(0)> docCredentials;
  JsonObject credentials = docCredentials.to<JsonObject>();
  user["credentials"] = credentials;

  serializeJson(docUser, eepromStream);
}

StaticJsonDocument<500> logUser(String username, String password) {
  StaticJsonDocument<500> docUser;
  deserializeJson(docUser, eepromStream);
  JsonObject userObject = docUser.as<JsonObject>();

  if (userObject["user"] == username && userObject["password"] == password) {
    return docUser;
  } else {
    StaticJsonDocument<500> docError;
    JsonObject error = docError.to<JsonObject>();
    error["error"] = "error";
    return docError;
  }
}

void storePassword(String service, String password) {

}

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