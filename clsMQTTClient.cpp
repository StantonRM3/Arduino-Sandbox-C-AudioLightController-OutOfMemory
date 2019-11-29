//*********************************************************************************************************************************************************************************************************************************************
// clsMQTTClient.cpp
//
// Class containing all the functionality to manage reading serial data from the NodeMCU module and loading the interesting variables...
//*********************************************************************************************************************************************************************************************************************************************
#include "Debug.h"
#include "clsMQTTClient.h"

// SoftwareSerial mySerial(10, 11); // RX, TX

void clsMQTTClient::begin()
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsMQTTClient::begin()");
  Serial.println("Initialising NodeMCU Interface");
  Serial.println("------------------------------");
#endif

  Serial1.begin(SERIAL_BAUD);
}

void clsMQTTClient::subscribe(char *pTopic)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.print("clsMQTTClient::subscribe() : ");
  Serial.println(pTopic);
#endif

  return;

  // TODO: Make this dynamic, just here for reference howto...
  sprintf(t_topic, pTopic);
  sprintf(t_value, "SUBSCRIBE");

  createJSON();

  // Send subscribe message over Serial to NodeMCU...
  Serial1.println(t_jsonStr);
}

bool clsMQTTClient::checkForNewMessage()
{
  bool retval_NewMsg = false;

  if (Serial1.available() > 0) {
    String input;

    // Read the incoming stream
    input = Serial1.readStringUntil('\n');
    Serial.println(input);

    // Quick check on wether to just throw this message away...
    if (input.substring(0, 5) == "DEBUG")
      return retval_NewMsg;

    // Parse the JSON and ensure it is a valid JSON...
    strcpy(t_jsonStr, input.c_str());
    parseJSON();

#ifdef DEBUG
    Serial.print("ARDUINO: parseJSON topic = ");
    Serial.println(t_topic);
    Serial.print("ARDUINO: parseJSON value = ");
    Serial.println(t_value);
#endif

    // Is it a valid message...
    if (strcmp(t_topic, "UNKNOWN"))
    {
#ifdef DEBUG
      Serial.print("ARDUINO: MQTT Message received, now do something with it: ");
      Serial.println(input);
#endif

      // Message received, now set the Public variable...
      setPublicVariable();

      retval_NewMsg = true;
    }
#ifdef DEBUG
    else
    {
      Serial.print("ARDUINO: Invalid message received: ");
      Serial.println(input);
    }
#endif
  }

  return retval_NewMsg;
}

void clsMQTTClient::createJSON(void)
{
  // Create JSON Object to send to NodeMCU...
  JSONVar myObject;
  myObject["topic"] = t_topic;
  myObject["value"] = t_value;

  String jsonString = JSON.stringify(myObject);

  jsonString.toCharArray(t_jsonStr, MAX_JSONSTR_LEN);
}

void clsMQTTClient::parseJSON(void)
{
  bool msgOK = true;

#ifdef DEBUG
  Serial.print("ARDUINO: >> Parse input JSON: ");
  Serial.println(t_jsonStr);
#endif

  JSONVar myObject = JSON.parse(t_jsonStr);

  // JSON.typeof(jsonVar) can be used to get the type of the var
  if (JSON.typeof(myObject) == "undefined")
    msgOK = false;;

  if (msgOK ==  true)
  {
#ifdef DEBUG
    Serial.print("ARDUINO: JSON.typeof(myObject) = ");
    Serial.println(JSON.typeof(myObject)); // prints: object
#endif

    // myObject.hasOwnProperty(key) checks if the object contains an entry for key
    if (myObject.hasOwnProperty("topic"))
      strcpy(t_topic, (const char*) myObject["topic"]);
    else
      msgOK = false;

    if (msgOK == true)
    {
      if (myObject.hasOwnProperty("value"))
        strcpy(t_value, (const char*) myObject["value"]);
      else
        msgOK = false;
    }
  }

  if (msgOK == false)
  {
    // Set the values to UNKNOWN so the message interpreter will ignore them...
    strcpy(t_topic, "UNKNOWN");
    strcpy(t_value, "UNKNOWN");
  }

#ifdef DEBUG
  Serial.println("ARDUINO: << Parse input JSON");
#endif

  return;
}

void clsMQTTClient::setPublicVariable(void)
{
  bool retval = false;

  // Look through the subscriptions list to see if this is a variable we are interested in...
  for (int i = 0; i < NUM_SUBSCRIPTIONS; i++)
  {
    if (!strcmp(t_topic, subscriptions[i]))
    {
      Serial.print("YES we are interested '");
      Serial.print(t_topic);
      Serial.print("' = '");
      Serial.print(subscriptions[i]);
      Serial.println("'");

      switch (i) {
        case 0: curDateTime = t_value;                    break;
        case 1: lightsOnOff = convertValueToBool();       break;
        case 2: ledColour = t_value;                      break;
        case 3: randomColour = convertValueToBool();      break;
        case 4: clockStatus = convertValueToBool();       break;
        case 5: matrixPattern = t_value;                  break;
        case 6: stripPattern = t_value;                   break;
        case 7: randomPattern = convertValueToBool();     break;
      };

      retval = true;
      break;
    }
  }

  //  if(!retval)
  //  {
  //      Serial.print("NO we are NOT interested '");
  //      Serial.print(t_topic);
  //      Serial.println("'");
  //  }
}

bool clsMQTTClient::convertValueToBool(void)
{
  bool retval = false;

  if (!strcmp(t_value, "true"))
    retval = true;

  return retval;
}

void clsMQTTClient::dumpSubscriptionValues()
{
  Serial.println("Subscription Value Dump");
  Serial.println("-----------------------");
  Serial.print("curDateTime = ");
  Serial.println(curDateTime);
  Serial.print("lightsOnOff = ");
  Serial.println(lightsOnOff);
  Serial.print("ledColour = ");
  Serial.println(ledColour);
  Serial.print("randomColour = ");
  Serial.println(randomColour);
  Serial.print("clockStatus = ");
  Serial.println(clockStatus);
  Serial.print("matrixPattern = ");
  Serial.println(matrixPattern);
  Serial.print("stripPattern = ");
  Serial.println(stripPattern);
  Serial.print("randomPattern = ");
  Serial.println(randomPattern);
}
