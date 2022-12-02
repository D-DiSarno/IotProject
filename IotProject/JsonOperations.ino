void store(){

  JsonArray users = doc.to<JsonArray>();

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
void read(){
  StaticJsonDocument<500> doc;
  EepromStream eepromStream(0, 500);
  deserializeJson(doc, eepromStream);



  serializeJsonPretty(doc, Serial);
  
}
void getUserName(){
  
   EepromStream eepromStream(0, 500);
  deserializeJson(doc, eepromStream);
 // JsonObject user=doc.to<
  //Serial.println(doc.data().toArray().getElement(0)));
 // credenziali[0].setName(doc["credentials"][0]["name"]);
 // Serial.println(credenziali[0].getName());

}
//modify
//create by user
//delete
//search
//get