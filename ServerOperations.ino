bool connectionToServer() {
  client.setCACert(test_root_ca);
  client.setCertificate(test_client_cert);  // for client verification
  client.setPrivateKey(test_client_key);    // for client verification
  unsigned long start_time_listen = millis();

  do {
    Serial.println("Connecting to server");
    delay(400);

    if ((millis() - start_time_listen) > 20000) {
      Serial.print("Server not responding.");

      WiFi.disconnect(true);  // Disconnect from the network
      WiFi.mode(WIFI_OFF);    //Switch WiFi off

      delay(3000);

      return false;
    }
  } while (!client.connect(WiFi.gatewayIP(), port));

  return true;
}

void startCommunicationToServer() {
  String entry = "";
  int op = 0;

  while (client.connected()) {
    while (client.available()) {
      char c = client.read();
      entry += c;
    }

    op = entry.substring(0, 1).toInt();
    if (op > 0) {
      //update
      update_credentials(op, entry);
      entry = "";
      op = 0;
    }
  }
  stopConnectionToServer();
}

void stopConnectionToServer() {
  client.stop();
  WiFi.disconnect(true);  // Disconnect from the network
  WiFi.mode(WIFI_OFF);    // Switch WiFi off
}

void update_credentials(int op, String entry) {
  int str_len = entry.length() + 1;
  char buffer[str_len];
  entry.toCharArray(buffer, str_len);

  char *token = strtok(buffer, "-");
  String entry_string[4];

  switch (op) {
    case 1: {
      // CREA UTENTE

      // start string splitting
      char *data[3];
      for (int i = 0; i < 3 && token != NULL; i++) {
        token = strtok(NULL, "-");
        data[i] = token;

        if (i == 0)
          continue;

        String char_to_string(data[i]);
        entry_string[i - 1] = char_to_string;
      }
      // end string splitting
      String uid=getUID();
      int res = createUser(entry_string[0], entry_string[1],uid);
      if (res == 1) {
        Serial.print("Errore nella creazione dell'utente!");
        delay(3500);
      } else if (res == 2) {
        Serial.print("Utente già esistente!");
        delay(3500);
      } else {
        Serial.print("Utente creato con successo!");
        delay(3500);        
      }
      client.println(res); // LEGGI QUI PER CAPIRE COME INVIARE I DATI
      Serial.println("");
      break;
    }

    case 2: {
      // LOGIN UTENTE

      // start string splitting
      char *data[3];
      for (int i = 0; i < 3 && token != NULL; i++) {
        token = strtok(NULL, "-");
        data[i] = token;

        if (i == 0)
          continue;

        String char_to_string(data[i]);
        entry_string[i - 1] = char_to_string;
      }
      // end string splitting
       String uid=getUID();
      if (logUser(entry_string[0], entry_string[1],uid)) {
        Serial.print("Accesso effettuato con successo!");
        client.println(2);
        delay(3500);
      } else {
         client.println(1);
        Serial.print("Credenziali errate!");
        delay(3500);        
      }
      break;
    }

    case 3: {
      // AGGIUNGI CREDENZIALI

      // start string splitting
      char *data[5];
      for (int i = 0; i < 5 && token != NULL; i++) {
        token = strtok(NULL, "-");
        data[i] = token;

        if (i == 0)
          continue;

        String char_to_string(data[i]);
        entry_string[i - 1] = char_to_string;
      }
      // end string splitting

      int res = addPassword(entry_string[0], entry_string[1], entry_string[2], entry_string[3],entry_string[4]);
      if (res == 1) {
        Serial.print("Errore nel salvataggio delle credenziali!");
        delay(3500);       
      } else {
        Serial.print("Credenziali memorizzate con successo!");
        delay(3500);        
      }
      break;
    }

    case 4: {
      // OTTIENI CREDENZIALI

      // start string splitting
      char *data[5];
      for (int i = 0; i < 4 && token != NULL; i++) {
        token = strtok(NULL, "-");
        data[i] = token;

        if (i == 0)
          continue;

        String char_to_string(data[i]);
        entry_string[i - 1] = char_to_string;
      }
      // end string splitting
      String res = getPassword(entry_string[0], entry_string[1], entry_string[2],entry_string[3]);
      if (res == "Errore") {
        Serial.print("Errore nel recuper delle credenziali!");
        delay(3500);
      } else {
        Serial.print("Credenziali recuperate con successo!");
        delay(3500);        
      }
      break;
    }

    case 5: {
      // ELIMINA CREDENZIALI

      // start string splitting
      char *data[4];
      for (int i = 0; i < 4 && token != NULL; i++) {
        token = strtok(NULL, "-");
        data[i] = token;

        if (i == 0)
          continue;

        String char_to_string(data[i]);
        entry_string[i - 1] = char_to_string;
      }
      // end string splitting

      int res = deletePassword(entry_string[0], entry_string[1], entry_string[2], entry_string[3]);
      if (res == 1) {
        Serial.print("Errore nell'eliminazione delle credenziali!");
        delay(3500);
      } else {
        Serial.print("Credenziali eliminate con successo!");
        delay(3500);        
      }
      break;
    }
   
  }
  Serial.print("Esp32 connected to server ");
  return;
}
