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

  char *token = strtok(buffer, "Æ");
  String entry_string[5];

  switch (op) {
    case 1: {
      // CREA UTENTE

      // start string splitting
      char *data[2];
      for (int i = 0; i < 2 && token != NULL; i++) {
        token = strtok(NULL, "Æ");
        data[i] = token;
        String char_to_string(data[i]);
        entry_string[i] = char_to_string;
      }
      // end string splitting

      String uid = hashString(getUID());
      String hashedPassword = hashString(entry_string[1]);
      String hashedUid = hashString(uid);
      int res = createUser(entry_string[0], hashedPassword, hashedUid);
      client.println(res);
      if (res == 1) {
        Serial.println("Errore nella creazione dell'utente!");
      } else if (res == 2) {
        Serial.println("Utente già esistente!");
      } else {
        Serial.println("Utente creato con successo!");
        client.println(uid);
      }
      Serial.println("");
      break;
    }

    case 2: {
      // LOGIN UTENTE

      // start string splitting
      char *data[2];
      for (int i = 0; i < 2 && token != NULL; i++) {
        token = strtok(NULL, "Æ");
        data[i] = token;
        String char_to_string(data[i]);
        entry_string[i] = char_to_string;
      }
      // end string splitting
      
      String uid = hashString(getUID());
      String hashedPassword = hashString(entry_string[1]);
      String hashedUid = hashString(uid);
      if (logUser(entry_string[0], hashedPassword, hashedUid)) {
        Serial.println("Accesso effettuato con successo!");
        client.println("0");
        client.println(uid);
      } else {
        client.println("1");
        Serial.println("Credenziali errate!");
      }
      Serial.println("");
      break;
    }

    case 3: {
      // AGGIUNGI CREDENZIALI

      // start string splitting
      char *data[5];
      for (int i = 0; i < 5 && token != NULL; i++) {
        token = strtok(NULL, "Æ");
        data[i] = token;
        String char_to_string(data[i]);
        entry_string[i] = char_to_string;
      }
      // end string splitting

      String hashedPassword = hashString(entry_string[1]);
      String hashedUid = hashString(entry_string[2]);
      int res = addPassword(entry_string[0], hashedPassword, hashedUid, entry_string[3], entry_string[4]);
      client.println(res);
      if (res == 1) {
        Serial.println("Errore nel salvataggio delle credenziali!");
      } else if (res == 2) {
        Serial.println("Errore durante l'accesso!");
      } else {
        Serial.println("Credenziali memorizzate con successo!");
      }
      Serial.println("");
      break;
    }

    case 4: {
      // ELIMINA CREDENZIALI

      // start string splitting
      char *data[4];
      for (int i = 0; i < 4 && token != NULL; i++) {
        token = strtok(NULL, "Æ");
        data[i] = token;
        String char_to_string(data[i]);
        entry_string[i] = char_to_string;
      }
      // end string splitting

      String hashedPassword = hashString(entry_string[1]);
      String hashedUid = hashString(entry_string[2]);
      int res = deletePassword(entry_string[0], hashedPassword, hashedUid, entry_string[3]);
      client.println(res);
      if (res == 1) {
        Serial.println("Errore nell'eliminazione delle credenziali!");
      } else if (res == 2) {
        Serial.println("Errore durante l'accesso!");
      } else {
        Serial.println("Credenziali eliminate con successo!");
      }
      Serial.println("");
      break;
    }

    case 5: {
      // OTTIENI CREDENZIALI

      // start string splitting
      char *data[4];
      for (int i = 0; i < 4 && token != NULL; i++) {
        token = strtok(NULL, "Æ");
        data[i] = token;
        String char_to_string(data[i]);
        entry_string[i] = char_to_string;
      }
      // end string splitting

      String hashedPassword = hashString(entry_string[1]);
      String hashedUid = hashString(entry_string[2]);
      String res = getPassword(entry_string[0], hashedPassword, hashedUid, entry_string[3]);
      if (res == "") {
        Serial.println("Errore nel recupero delle credenziali!");
        client.println("1");
      } else {
        Serial.println("Credenziali recuperate con successo!");
        client.println("0");
        client.println(res);
      }
      Serial.println("");
      break;
    } 
  }

  Serial.println("Esp32 connected to server ");
  return;
}
