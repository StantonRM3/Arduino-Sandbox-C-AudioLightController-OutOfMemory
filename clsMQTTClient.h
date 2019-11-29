//*********************************************************************************************************************************************************************************************************************************************
// clsMQTTClient.h
//
// Class containing all the functionality to manage DS1307 RTC Clock, etc, etc...
//*********************************************************************************************************************************************************************************************************************************************
// Prevent the header getting included multiple times...
#ifndef _CLSMQTTCLIENT_H
#define _CLSMQTTCLIENT_H

#include "Enums.h"
#include <SoftwareSerial.h>
#include <Arduino_JSON.h>

#define MAX_JSONSTR_LEN   128
#define MAX_TOPIC_LEN     64
#define MAX_VALUE_LEN     16

#define NUM_SUBSCRIPTIONS  8

//#define SERIAL_BAUD     57600

class clsMQTTClient
{
  public:

    void begin();
    bool checkForNewMessage(void);
    void dumpSubscriptionValues(void);

    // MQTT Values...
    String curDateTime = "UNKNOWN";
    bool lightsOnOff = false;
    String ledColour = "UNKNOWN";
    bool randomColour = false;
    bool clockStatus = false;
    String matrixPattern = "UNKNOWN";
    String stripPattern = "UNKNOWN";
    bool randomPattern = false;

  private:

    // Single multiuse buffers...
    char t_jsonStr[MAX_JSONSTR_LEN];
    char t_topic[MAX_TOPIC_LEN],
         t_value[MAX_VALUE_LEN];

    void subscribe(char *pTopic);
    void createJSON(void);
    void parseJSON(void);
    void setPublicVariable(void);
    bool convertValueToBool(void);

    // I'm going to be dirty here and use the index to determine what variable to put the value in, therefor list order IS important...
    char *subscriptions[NUM_SUBSCRIPTIONS] = {
      "home/office/curDateTime",
      "home/office/lightsOnOff",
      "home/office/ledColour",
      "home/office/randomColour",
      "home/office/sirenOne/clockStatus",
      "home/office/sirenOne/matrixPattern",
      "home/office/sirenOne/stripPattern",
      "home/office/sirenOne/randomPattern"
    };
};

#endif
