
//*********************************************************************************************************************************************************************************************************************************************
// clsMenuCtrl.h
//
// Class containing all the functionality to manage OLED menu
//
//*********************************************************************************************************************************************************************************************************************************************
// Prevent the header getting included multiple times...
#ifndef CLSMENUCTRL_H
#define CLSMENUCTRL_H

#include "Enums.h"
#include "DisplayText.h"
#include "clsColourCtrl.h"
#include "clsClock.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// OLED Dsplay Setup (SSD1306)...   This handles both 128x64 and 128x32 OLED displays I have...
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define SCREEN_WIDTH 128      // OLED display width, in pixels
#define SCREEN_HEIGHT 64      // OLED display height, in pixels
#define OLED_RESET     4      // Reset pin # (or -1 if sharing Arduino reset pin)

class clsMenuCtrl
{
  public:

    void begin(clsClock *pClsClock);
    void display_Status(char *pHeadingText, char *pModeText, char *pNextItemText);
    void display_NowPlaying(eState pPatternMode, eOnOff pShowClock, eState pColourMode);
    
  private:

    Adafruit_SSD1306 * myDisplay;
    clsClock *myClock;
    
    void display_MenuHeader(char *pHeadingText);
    void display_MenuFooter(void);
    void display_MenuFooter_Settings(eState pPatternMode, eOnOff pShowClock, eState pColourMode);
};

#endif
