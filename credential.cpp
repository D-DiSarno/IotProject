#include "Credential.h"
#include "Arduino.h"

/*Credential::Credential(String name, String username, String password, String pinCode){
  this->_name = name;
  this->_username = username;
  this->_password = password;
 // this->_pinCode = pinCode;
};
*/
Credential::Credential(String name, String username, String password){
  this->_name = name;
  this->_username = username;
  this->_password = password;
};

Credential::Credential(String name, String password){
  this->_name = name;
  this->_password = password;
};

Credential::Credential(){

};

String Credential::getName(){
  return this->_name;
}

String Credential::getUsername(){
  return this->_username;
}

String Credential::getPassword(){
  return this->_password;
}

/*String Credential::getPinCode(){
  return this->_pinCode;
}*/

void Credential::setName(String name){
 this->_name = name;
}

void Credential::setUsername(String username){
  this->_username = username;
}

void Credential::setPassword(String password){
  this->_password = password;
}

/*void Credential::setPinCode(String pinCode){
  this->_pinCode = pinCode;
}*/