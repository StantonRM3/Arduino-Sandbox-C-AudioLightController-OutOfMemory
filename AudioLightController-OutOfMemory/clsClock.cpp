//*********************************************************************************************************************************************************************************************************************************************
// clsClcok.cpp
//
// Class containing all the functionality to manage DS1307 RTC Clock, etc, etc...
//*********************************************************************************************************************************************************************************************************************************************
// #include "Debug.h"
#include "clsClock.h"

void clsClock::begin(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsClock::begin()");
  Serial.println("Initialising DS1307 RTC");
  Serial.println("-----------------------");
#endif
}

void clsClock::getTimeDate(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsClock::getTimeDate()");
#endif

#ifdef DEBUG
  ++numgetTimeDateCalls;
  Serial.print(numgetTimeDateCalls);
  Serial.print(" -> IN: clsClock::getTimeDate(): ");
#endif

  if (RTC.read(tm))
  {
    DS1307Error = false;

#ifdef DEBUG
    Serial.print("Format data : ");
#endif
    
    sprintf(stringDate, "%02d/%02d/%04d", tm.Day, tm.Month, tmYearToCalendar(tm.Year));
    sprintf(stringTime, "%02d:%02d:%02d", tm.Hour, tm.Minute, tm.Second);

#ifdef DEBUG
    Serial.print(stringDate);
    Serial.print(" ");
    Serial.print(stringTime);
    Serial.print(" : ");
#endif
    
    Hour = tm.Hour;
    if (Hour > 12)
      Hour -= 12;
    Minute = tm.Minute;
    Second = tm.Second;
  }
  else
  {
    DS1307Error = true;

    sprintf(stringDate, "ERR:DATE");
    sprintf(stringTime, "ERR:TIME");

    if (RTC.chipPresent())
      Serial.println("clsClock::getTimeDate() ERROR: The DS1307 is stopped.  Please run the SetTime : ");
    else
      Serial.println("clsClock::getTimeDate() ERROR: DS1307 read error!  Please check the circuitry. : ");
  }
  
#ifdef DEBUG
  Serial.println("OUT: ->");
#endif
}

void clsClock::print2digits(int number)
{
  if (number >= 0 && number < 10)
  {
    Serial.write('0');
  }
  Serial.print(number);
}

bool clsClock::getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3)
    return false;

  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;

  return true;
}

bool clsClock::getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3)
    return false;

  for (monthIndex = 0; monthIndex < 12; monthIndex++)
  {
    if (strcmp(Month, monthName[monthIndex]) == 0)
      break;
  }

  if (monthIndex >= 12)
    return false;

  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);

  return true;
}

void clsClock::setTimeDateFromCompilerTimestamp()
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsClock::setTimeDateFromCompilerTimestamp()");
#endif

  bool parse = false;
  bool config = false;

  // get the date and time the compiler was run
  if (getDate(__DATE__) && getTime(__TIME__))
  {
    parse = true;

    // and configure the RTC with this info
    if (RTC.write(tm))
    {
      config = true;
    }
  }

  if (parse && config)
  {
    Serial.print("DS1307 configured Time=");
    Serial.print(__TIME__);
    Serial.print(", Date=");
    Serial.println(__DATE__);
  }
  else if (parse)
  {
    Serial.println("DS1307 Communication Error :-{");
    Serial.println("Please check your circuitry");
  }
  else
  {
    Serial.print("Could not parse info from the compiler, Time=\"");
    Serial.print(__TIME__);
    Serial.print("\", Date=\"");
    Serial.print(__DATE__);
    Serial.println("\"");
  }
}
