
bool connectionToServer(){
  client.setCACert(test_root_ca);
  client.setCertificate(test_client_cert); // for client verification
  client.setPrivateKey(test_client_key);  // for client verification
  unsigned long start_time_listen = millis();
  
  do {
    Serial.println("Connecting to server");
    
  
      
    delay(400); 
     
    if((millis() - start_time_listen) > 20000){
       Serial.print("Server not responding.");
              
       WiFi.disconnect(true);  // Disconnect from the network
       WiFi.mode(WIFI_OFF); //Switch WiFi off
        
       delay(3000);
       
       return false;
     }
  } while (!client.connect(WiFi.gatewayIP(), port));

  return true;
}

void startCommunicationToServer(String str){
  client.println(str.length());
  client.println(str);

  /* call some send client if str excedes of 16384 bytes*/
  size_t maxChunk = 16384;
  int numOfChunks = (int) str.length() / maxChunk;
  int counterChars = 0;
  
  for(int i = 0; i < numOfChunks + 1; i++){
    client.println(str.substring(counterChars, counterChars + maxChunk));
    counterChars += maxChunk + 1;
  }
    
  String entry = "";
  int op = 0;
  
  while (client.connected()) {
    while(client.available()){
      char c = client.read();
      entry += c;
    }

    op = entry.substring(0,1).toInt();
    if(op > 0){
      //update
      update_credentials(op, entry);
      entry = "";
      op = 0;
    }
  }
  stopConnectionToServer();
}

void stopConnectionToServer(){
  client.stop();
  WiFi.disconnect(true);  // Disconnect from the network
  WiFi.mode(WIFI_OFF); //Switch WiFi off
  

}

void update_credentials(int op, String entry ){
  int str_len = entry.length() + 1;
  char buffer[str_len]; 
  entry.toCharArray(buffer, str_len);
      
  char *token = strtok(buffer, "-");
  String entry_string[4]; 
      
  switch(op){
    case 1: {
      /*split string*/ //ADD 
      char *data[3];
      
      for(int i = 0; i < 3 && token != NULL; i++){
        token = strtok(NULL, "-");
        data[i] = token;
        
        if(i == 0)
          continue;
          
        String char_to_string(data[i]);
        entry_string[i-1] = cipher->encryptString(char_to_string);
      }
      /*end split string */
  //Chiedere data[0] cosa sia 
      if(addPassword(entry_string[0], entry_string[1],entry_string[2],entry_string[3])){        
       Serial.print("Add Success!");
      
        delay(3500);
     }
      else{
        Serial.print("Add Error!");
       
        delay(3500);
      }
      break;
    }
    
    case 2: { //UPDATE
      /*split string*/
    / char *data[4];
            
      for(int i = 0; i < 4 && token != NULL; i++){
        token = strtok(NULL, "-");
        data[i] = token;
        
        if(i < 2)
          continue;

        String char_to_string(data[i]);
        
        if(strcmp(data[i], "NULL") != 0)
          entry_string[i-2] = cipher->encryptString(char_to_string);
        else
          entry_string[i-2] = char_to_string;
      }
      /*end split string*/
 //data[0]
     if(addPassword( entry_string[0], entry_string[1],entry_string[2],entry_string[3])){        
       Serial.print("Update Success!");
        
       delay(3500);
      }
      else{
       Serial.print("Update Error!");
       
       delay(3500);
      }
      break;
    }
    
    case 3: { //RIMUOVO
      char *data = strtok(NULL, "-");
  
      if(deletePassword(entry_string[0],entry_string[1],entry_string[2])){        
        Serial.print("Delete Success!");
       
        delay(3500);
      }
      else{
       Serial.print("Delete Error!");
       
        delay(3500);
      }
      break;
    }
    
  }
  
  Serial.print("Esp32 connected to server ");
  return;
  }
