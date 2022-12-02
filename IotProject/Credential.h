#ifndef Credential_h
#define Credential_h

#include "Arduino.h"

class Credential {
  private:
            String _name;
            String _username;
            String _password;
            String _pinCode;

    public:
            //Credential(String name, String username, String password, String pinCode);
            Credential(String name, String username, String password);
            Credential(String name, String password);
            Credential();

            String getName();
            String getUsername();
            String getPassword();
            String getPinCode();

            void setName(String name);
            void setUsername(String username);
            void setPassword(String password);
            void setPinCode(String pinCode);
};

#endif