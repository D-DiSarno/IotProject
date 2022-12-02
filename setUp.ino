void setup(){
Serial.begin(9600);
while(!Serial) continue;
SPI.begin();
  mfrc522.PCD_Init();
//store();
//read();
}