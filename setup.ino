void setup() {
  Serial.begin(115200);
  while (!Serial) continue;
  SPI.begin();
  mfrc522.PCD_Init();

  connectWiFi();
  createUser("uno","due");

  char *key = "%A.Esp32_Pa66.C%";
  cipher->setKey(key);
}
