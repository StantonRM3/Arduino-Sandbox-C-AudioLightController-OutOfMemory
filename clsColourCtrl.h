//*********************************************************************************************************************************************************************************************************************************************
// clsColourCtrl.h
//
// Class containing all the functionality to manage LED colours and display brightness, etc, etc...
//*********************************************************************************************************************************************************************************************************************************************
// Prevent the header getting included multiple times...
#ifndef CLSCOLOURCTRL_H
#define CLSCOLOURCTRL_H

#include <FastLED.h>
#include "Enums.h"

#define MAX_RANDOM_COLOUR_PALLET  64

class clsColourCtrl
{
  public:

    void begin(void);    // eState pColourMode);
    void setColourPallet(eOnOff pDisplayMode, eState pColourMode);
    eState cycleNextColour(eState pColourMode);
    
    CRGB colourPallette[MAX_RANDOM_COLOUR_PALLET];
    int brightness = 128;
    
  private:

    HSVHue hueSelected[8] = { HUE_RED, HUE_ORANGE, HUE_YELLOW, HUE_GREEN, HUE_AQUA, HUE_BLUE, HUE_PURPLE, HUE_PINK };
    HSVHue hueRange[2];
    int fadeIdx;

    eState colourMode = 0;    // Force it to be different colour than startup to ensure colours seeded...
};

#endif
