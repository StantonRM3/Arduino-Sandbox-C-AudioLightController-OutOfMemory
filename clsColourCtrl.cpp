//*********************************************************************************************************************************************************************************************************************************************
// clsColourCtrl.cpp
//
// Class containing all the functionality to manage Matrix and LED Strip colour pallets
//
//*********************************************************************************************************************************************************************************************************************************************
#include "Debug.h"

#include "clsColourCtrl.h"

void clsColourCtrl::begin(void)       // eState pColourMode)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsColourCtrl::begin()");
#endif

  // Seed the randomNumber...
  randomSeed(analogRead(A0));

  FastLED.setBrightness(brightness);

  // Set the colour pallette up...
  // setColourPallet(ON, pColourMode);
}

void clsColourCtrl::setColourPallet(eOnOff pDisplayMode, eState pColourMode) {
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsColourCtrl::setColourPallet()");
#endif

  // If colours have changed, then regenerate the colour pallette...
  if (pColourMode != colourMode) {

    if (pDisplayMode == ON) {
      switch (pColourMode) {
        case DISP_COL_RED:            hueRange[0] = 0;  hueRange[1] = 1;  break;
        case DISP_COL_ORANGE:         hueRange[0] = 0;  hueRange[1] = 2;  break;
        case DISP_COL_YELLOW:         hueRange[0] = 1;  hueRange[1] = 3;  break;
        case DISP_COL_GREEN:          hueRange[0] = 2;  hueRange[1] = 4;  break;
        case DISP_COL_AQUA:           hueRange[0] = 3;  hueRange[1] = 5;  break;
        case DISP_COL_BLUE:           hueRange[0] = 4;  hueRange[1] = 6;  break;
        case DISP_COL_PURPLE:         hueRange[0] = 5;  hueRange[1] = 7;  break;
        case DISP_COL_PINK:           hueRange[0] = 0;  hueRange[1] = 7;  break;
        case DISP_COL_RANDOM:         hueRange[0] = 0;  hueRange[1] = 7;  break;
      }

      fill_gradient(colourPallette, 0, CHSV(hueSelected[hueRange[0]], random(128, 255) , random(128, 255)), MAX_RANDOM_COLOUR_PALLET, CHSV(hueSelected[hueRange[1]], random(128, 255), random(128, 255)), FORWARD_HUES);
      fadeIdx = 100;
      colourMode = pColourMode;
    }
    else {
      for (int i = 0; i < MAX_RANDOM_COLOUR_PALLET; i++)
        colourPallette[i] = CHSV(0, 0, 0);

    }
  }
}

eState clsColourCtrl::cycleNextColour(eState pColourMode)
{
  eState retval;

  switch (pColourMode) {
    case DISP_COL_RED:            retval = DISP_COL_ORANGE;   break;
    case DISP_COL_ORANGE:         retval = DISP_COL_YELLOW;   break;
    case DISP_COL_YELLOW:         retval = DISP_COL_GREEN;    break;
    case DISP_COL_GREEN:          retval = DISP_COL_AQUA;     break;
    case DISP_COL_AQUA:           retval = DISP_COL_BLUE;     break;
    case DISP_COL_BLUE:           retval = DISP_COL_PURPLE;   break;
    case DISP_COL_PURPLE:         retval = DISP_COL_PINK;     break;
    case DISP_COL_PINK:           retval = DISP_COL_RED;      break;
  }

  return retval;
}
