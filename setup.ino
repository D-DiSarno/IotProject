void setup() {
  Serial.begin(9600);
  while (!Serial) continue;
  SPI.begin();
  mfrc522.PCD_Init();
  //store();
  //read();
  //createUser("Bowser","ABCDEF");
  //read();
  //deleteUser("Bowser");
  //clearJson();
  //getUser("Mario");
  
}
