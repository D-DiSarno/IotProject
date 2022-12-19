void setup() {
  Serial.begin(9600);
  while (!Serial) continue;
  SPI.begin();
  mfrc522.PCD_Init();
   
  connectWiFi();

  //Server setup
  char *key = "%A.Esp32_Pa66.C%";
  cipher->setKey(key);

  //store();
  //read();
  //createUser("Bowser","ABCDEF");
  //read();
  //deleteUser("Bowser");
  //clearJson();
  //getUser("Mario");
  
}
