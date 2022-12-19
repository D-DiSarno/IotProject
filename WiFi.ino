void connectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_STA);  //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    //String ssid = wifi_config["SSID"];
   // String password = wifi_config["password"];
   // WiFi.begin(ssid.c_str(), password.c_str());

    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(100);
    }
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
   
  
  }
 
}

/*bool load_R_WiFiConfigFile() {
  if (!SPIFFS.begin(true)) {
    return false;
  }

  WiFiFile = SPIFFS.open("/WiFiCredentials.json");
  if (!WiFiFile) {
    return false;
  }
  return true;
}

bool close_WiFiFile() {
  if (WiFiFile) {
    WiFiFile.close();
    return true;
  }
  return false;
}

void read_WiFiFile() {
  if (WiFiFile) {
    DeserializationError error = deserializeJson(wifi_config, WiFiFile);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }
  }
}*/