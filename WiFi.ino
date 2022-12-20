void connectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {

    WiFi.mode(WIFI_STA);  //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    //prova a connettersi
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(100);
    }
   if(connectionToServer()){
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

  }
}
}
