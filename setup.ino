void setup() {
  Serial.begin(115200);
  while (!Serial) continue;

  //SPI.begin();
  //mfrc522.PCD_Init();

  //Serial.println(createUser("uno","due"));
  //Serial.println(getUserPassword("uno"));
  connectWiFi();
  //char *key = "%A.Esp32_Pa66.C%";
  //cipher->setKey(key);
}
