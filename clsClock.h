//*********************************************************************************************************************************************************************************************************************************************
// clsClock.h
//
// Class containing all the functionality to manage DS1307 RTC Clock, etc, etc...
//*********************************************************************************************************************************************************************************************************************************************
// Prevent the header getting included multiple times...
#ifndef _CLSCLOCK_H
#define _CLSCLOCK_H

#include <Arduino.h>
#include <Time.h>
#include <Wire.h>
// #include <TimeLib.h>
#include <DS1307RTC.h>

class clsClock
{
  public:

    void begin(void);
    void getTimeDate(void);
    void setTimeDateFromCompilerTimestamp();
    char stringDate[16];
    char stringTime[16];
    int  Hour;
    int  Minute;
    int  Second;
    
  private:

    DS1307RTC RTC;

    int numgetTimeDateCalls = 0;

    bool DS1307Error = false;
    void print2digits(int number);
    bool getTime(const char *str);
    bool getDate(const char *str);

    tmElements_t tm;
    const char *monthName[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

};

#endif
