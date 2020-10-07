#ifndef MQTTBroker_H
#define MQTTBroker_H

#include "uMQTTBroker.h"
#include<Arduino.h>


class MQTTBroker: public uMQTTBroker
{
public:

    String Data;
    virtual bool onConnect(IPAddress addr, uint16_t client_count) {
      Serial.println(addr.toString()+" connected");
      return true;
    }
    
    virtual bool onAuth(String username, String password) {
      //Serial.println("Username/Password: "+username+"/"+password);
      return true;
    }
    
    virtual void onData(String topic, const char *data, uint32_t length) {
      char data_str[length+1];
      os_memcpy(data_str, data, length);
      data_str[length] = '\0';
      this->Data = (String)data_str;
      Serial.println((String)data_str);
    }
};

#endif