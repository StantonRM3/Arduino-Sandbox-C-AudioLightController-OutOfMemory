//*********************************************************************************************************************************************************************************************************************************************
// clsStateMachine.cpp
//
// Class containing all the functionality to manage StateMachine
//
//*********************************************************************************************************************************************************************************************************************************************
#include "Debug.h"

#include "clsStateMachine.h"

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// STATE MACHINE
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// update()
// This function simply identifies, based on the currentState, what happens if you hit the Up, Down or Select buttons, and does this by setting the appropriate nextXXState that the stateMachine will go into
// if one of these buttons are pressed....
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void clsStateMachine::begin(clsColourCtrl *pClsColourCtrl, clsClock *pClsClock) {
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsStateMachine::begin()");
#endif

  colourCtrl = pClsColourCtrl;
  colourCtrl->setColourPallet(ON, colourMode);

  myClock = pClsClock;

  // Initialise the menu display
  menuCtrl.begin(myClock);

  // Setup the state machine...
  update();

}

int display_cycleTime_ms = 1000;
static unsigned long prevTime = 0;

void clsStateMachine::refreshDisplay()
{
  //#ifdef DEBUG_TRACE_PGM_ROUTE
  //  Serial.println("clsStateMachine::refreshDisplay()");
  //#endif

  unsigned long currTime = millis();

  // Only print it out every second (or so)...
  if ((currTime - prevTime) >= display_cycleTime_ms)
  {
    if (currentState == NOW_PLAYING)
    {
      // if (showClock == ON)
      myClock->getTimeDate();

      menuCtrl.display_NowPlaying(patternMode, showClock, colourMode);
    }

    prevTime = currTime;
  }
}

void clsStateMachine::update() {
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsStateMachine::update()");
#endif

  switch (currentState) {
    case NOW_PLAYING:             nextSelectState = DISP;             nextUpState = NOW_PLAYING;            nextDnState = NOW_PLAYING;              break;
    case DISP:                    nextSelectState = DISP_SHOW_CLOCK;  nextUpState = NOW_PLAYING;            nextDnState = SET;                      break;
    case DISP_SHOW_CLOCK:         nextSelectState = NOW_PLAYING;      nextUpState = DISP_SHOW_CLOCK;        nextDnState = DISP_PAT;                 break;
    case DISP_PAT:                nextSelectState = patternMode;      nextUpState = DISP_SHOW_CLOCK;        nextDnState = DISP_COL;                 break;
    case DISP_PAT_VU:             nextSelectState = NOW_PLAYING;      nextUpState = DISP_PAT_VU;            nextDnState = DISP_PAT_VU_PEAK;         break;
    case DISP_PAT_VU_PEAK:        nextSelectState = NOW_PLAYING;      nextUpState = DISP_PAT_VU;            nextDnState = DISP_PAT_VU_HUEBARS;      break;
    case DISP_PAT_VU_HUEBARS:     nextSelectState = NOW_PLAYING;      nextUpState = DISP_PAT_VU_PEAK;       nextDnState = DISP_PAT_VU_PULSAR;       break;
    case DISP_PAT_VU_PULSAR:      nextSelectState = NOW_PLAYING;      nextUpState = DISP_PAT_VU_HUEBARS;    nextDnState = DISP_PAT_VU_REVERSE;      break;
    case DISP_PAT_VU_REVERSE:     nextSelectState = NOW_PLAYING;      nextUpState = DISP_PAT_VU_PULSAR;     nextDnState = DISP_PAT_VU_SIDE;         break;
    case DISP_PAT_VU_SIDE:        nextSelectState = NOW_PLAYING;      nextUpState = DISP_PAT_VU_REVERSE;    nextDnState = DISP_PAT_VU_SIDEPULSE;    break;
    case DISP_PAT_VU_SIDEPULSE:   nextSelectState = NOW_PLAYING;      nextUpState = DISP_PAT_VU_SIDE;       nextDnState = DISP_PAT_OSCILLOSCOPE;    break;
    case DISP_PAT_OSCILLOSCOPE:   nextSelectState = NOW_PLAYING;      nextUpState = DISP_PAT_VU_SIDEPULSE;  nextDnState = DISP_PAT_OSCILLOSCOPE;    break;
    case DISP_COL:                nextSelectState = colourMode;       nextUpState = DISP_PAT;               nextDnState = DISP_ONOFF;               break;
    case DISP_COL_RED:            nextSelectState = NOW_PLAYING;      nextUpState = DISP_COL_RED;           nextDnState = DISP_COL_ORANGE;          break;
    case DISP_COL_ORANGE:         nextSelectState = NOW_PLAYING;      nextUpState = DISP_COL_RED;           nextDnState = DISP_COL_YELLOW;          break;
    case DISP_COL_YELLOW:         nextSelectState = NOW_PLAYING;      nextUpState = DISP_COL_ORANGE;        nextDnState = DISP_COL_GREEN;           break;
    case DISP_COL_GREEN:          nextSelectState = NOW_PLAYING;      nextUpState = DISP_COL_YELLOW;        nextDnState = DISP_COL_AQUA;            break;
    case DISP_COL_AQUA:           nextSelectState = NOW_PLAYING;      nextUpState = DISP_COL_GREEN;         nextDnState = DISP_COL_BLUE;            break;
    case DISP_COL_BLUE:           nextSelectState = NOW_PLAYING;      nextUpState = DISP_COL_AQUA;          nextDnState = DISP_COL_PURPLE;          break;
    case DISP_COL_PURPLE:         nextSelectState = NOW_PLAYING;      nextUpState = DISP_COL_BLUE;          nextDnState = DISP_COL_PINK;            break;
    case DISP_COL_PINK:           nextSelectState = NOW_PLAYING;      nextUpState = DISP_COL_PURPLE;        nextDnState = DISP_COL_RANDOM;          break;
    case DISP_COL_RANDOM:         nextSelectState = NOW_PLAYING;      nextUpState = DISP_COL_PINK;          nextDnState = DISP_COL_RANDOM;          break;
    case DISP_ONOFF:              nextSelectState = NOW_PLAYING;      nextUpState = DISP_COL;               nextDnState = DISP_ONOFF;               break;
    case SET:                     nextSelectState = SET_TONE;         nextUpState = DISP;                   nextDnState = NOW_PLAYING;              break;
    case SET_TONE:                nextSelectState = SET_TONE_BASS;    nextUpState = SET_TONE;               nextDnState = SET_TONE;                 break;
    case SET_TONE_BASS:           nextSelectState = SET_TONE_BASS;    nextUpState = SET_TONE_BASS;          nextDnState = SET_TONE_TREBLE;          break;
    case SET_TONE_TREBLE:         nextSelectState = SET_TONE_TREBLE;  nextUpState = SET_TONE_BASS;          nextDnState = SET_TONE_TREBLE;          break;
  }

  display();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// display()
// Based on the currentState, this function will set the LCD display to show its position in the menu system...
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void clsStateMachine::display() {
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsStateMachine::display()");
#endif

  switch (currentState) {
    case NOW_PLAYING:           menuCtrl.display_NowPlaying(patternMode, showClock, colourMode);                                                break;

    case DISP:                  menuCtrl.display_Status(ds_Menu, ds_Display, ds_Settings);                                                      break;
    case DISP_SHOW_CLOCK:

      if (showClock == ON)
        menuCtrl.display_Status(ds_Display, "Clock Off", ds_Display_Pattern);
      else
        menuCtrl.display_Status(ds_Display, "Clock On", ds_Display_Pattern);
      break;

    case DISP_PAT:              menuCtrl.display_Status(ds_Display, ds_Display_Pattern, ds_Display_Colour);                                     break;
    case DISP_PAT_VU:           menuCtrl.display_Status(ds_Display_Pattern, ds_Display_Pattern_VU, ds_Display_Pattern_VU_Peak);                 break;
    case DISP_PAT_VU_PEAK:      menuCtrl.display_Status(ds_Display_Pattern, ds_Display_Pattern_VU_Peak, ds_Display_Pattern_VU_Huebars);         break;
    case DISP_PAT_VU_HUEBARS:   menuCtrl.display_Status(ds_Display_Pattern, ds_Display_Pattern_VU_Huebars, ds_Display_Pattern_VU_Pulsar);       break;
    case DISP_PAT_VU_PULSAR:    menuCtrl.display_Status(ds_Display_Pattern, ds_Display_Pattern_VU_Pulsar, ds_Display_Pattern_VU_Reverse);       break;
    case DISP_PAT_VU_REVERSE:   menuCtrl.display_Status(ds_Display_Pattern, ds_Display_Pattern_VU_Reverse, ds_Display_Pattern_VU_Side);         break;
    case DISP_PAT_VU_SIDE:      menuCtrl.display_Status(ds_Display_Pattern, ds_Display_Pattern_VU_Side, ds_Display_Pattern_VU_SidePulse);       break;
    case DISP_PAT_VU_SIDEPULSE: menuCtrl.display_Status(ds_Display_Pattern, ds_Display_Pattern_VU_SidePulse, ds_Display_Pattern_Oscilloscope);  break;
    case DISP_PAT_OSCILLOSCOPE: menuCtrl.display_Status(ds_Display_Pattern, ds_Display_Pattern_Oscilloscope, "");                               break;

    case DISP_COL:

      if (displayMode == ON)
        menuCtrl.display_Status(ds_Display, ds_Display_Colour, "Turn Off");
      else
        menuCtrl.display_Status(ds_Display, ds_Display_Colour, "Turn On");
      break;

    case DISP_COL_RED:          menuCtrl.display_Status(ds_Display_Colour, ds_Display_Colour_Red, ds_Display_Colour_Orange);                break;
    case DISP_COL_ORANGE:       menuCtrl.display_Status(ds_Display_Colour, ds_Display_Colour_Orange, ds_Display_Colour_Yellow);             break;
    case DISP_COL_YELLOW:       menuCtrl.display_Status(ds_Display_Colour, ds_Display_Colour_Yellow, ds_Display_Colour_Green);              break;
    case DISP_COL_GREEN:        menuCtrl.display_Status(ds_Display_Colour, ds_Display_Colour_Green, ds_Display_Colour_Aqua);                break;
    case DISP_COL_AQUA:         menuCtrl.display_Status(ds_Display_Colour, ds_Display_Colour_Aqua, ds_Display_Colour_Blue);                 break;
    case DISP_COL_BLUE:         menuCtrl.display_Status(ds_Display_Colour, ds_Display_Colour_Blue, ds_Display_Colour_Purple);               break;
    case DISP_COL_PURPLE:       menuCtrl.display_Status(ds_Display_Colour, ds_Display_Colour_Purple, ds_Display_Colour_Pink);               break;
    case DISP_COL_PINK:         menuCtrl.display_Status(ds_Display_Colour, ds_Display_Colour_Pink, ds_Display_Colour_Random);               break;
    case DISP_COL_RANDOM:       menuCtrl.display_Status(ds_Display_Colour, ds_Display_Colour_Random, "");                                   break;

    case DISP_ONOFF:
      if (displayMode == ON)
        menuCtrl.display_Status(ds_Display, "Turn Off", "");
      else
        menuCtrl.display_Status(ds_Display, "Turn On", "");
      break;
    case SET:                   menuCtrl.display_Status(ds_Menu, ds_Settings, "");                                                          break;
    case SET_TONE:              menuCtrl.display_Status(ds_Settings, "Tone", "");                                                           break;
    case SET_TONE_BASS:         menuCtrl.display_Status(ds_Settings_SetTone, "Bass", "Treble");                                             break;
    case SET_TONE_TREBLE:       menuCtrl.display_Status(ds_Settings_SetTone, "Treble", "");                                                 break;
  }
}

void clsStateMachine::stateSelected(void) {
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsStateMachine::stateSelected()");
#endif

  switch (currentState) {
    case NOW_PLAYING:             break;
    case DISP:                    break;
    case DISP_SHOW_CLOCK:         // Toggle the clock on/off

      if (showClock == ON)
        showClock = OFF;
      else
        showClock = ON;
      break;

    case DISP_PAT:                break;

    // FALLTHRU Case Statements: The following states all change/set the pattern mode...
    case DISP_PAT_VU:
    case DISP_PAT_VU_PEAK:
    case DISP_PAT_VU_HUEBARS:
    case DISP_PAT_VU_PULSAR:
    case DISP_PAT_VU_REVERSE:
    case DISP_PAT_VU_SIDE:
    case DISP_PAT_VU_SIDEPULSE:
    case DISP_PAT_OSCILLOSCOPE:

      patternMode = currentState;
      break;

    case DISP_COL:                break;

    // FALLTHRU Case Statements: The following states all change/set the colour mode...
    case DISP_COL_RED:
    case DISP_COL_ORANGE:
    case DISP_COL_YELLOW:
    case DISP_COL_GREEN:
    case DISP_COL_AQUA:
    case DISP_COL_BLUE:
    case DISP_COL_PURPLE:
    case DISP_COL_PINK:
    case DISP_COL_RANDOM:

      colourMode = currentState;
      colourCtrl->setColourPallet(displayMode, colourMode);
      break;

    case DISP_ONOFF:            // Toggle the display on or off...
      if (displayMode == ON)
        displayMode = OFF;
      else
        displayMode = ON;
      break;

    case SET:                     break;
    case SET_TONE:                break;
    case SET_TONE_BASS:           break;
    case SET_TONE_TREBLE:         break;
  }

#ifdef DEBUG
  Serial.print("stateSelected: Changing State from: ");
  Serial.print(currentState);
  Serial.print(" to: ");
  Serial.println(nextSelectState);
#endif

  currentState = nextSelectState;
  update();
}

void clsStateMachine::backSelected(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsStateMachine::backSelected()");
#endif

  currentState = NOW_PLAYING;
  update();
}

void clsStateMachine::upSelected(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsStateMachine::upSelected()");
#endif

  currentState = nextUpState;
  update();
}

void clsStateMachine::downSelected(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsStateMachine::downSelected()");
#endif

  currentState = nextDnState;
  update();
}
