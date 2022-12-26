void setup() {
  Serial.begin(115200);
  while (!Serial) {
    continue;
  }

  SPI.begin();
  mfrc522.PCD_Init();

  connectWiFi();
}
