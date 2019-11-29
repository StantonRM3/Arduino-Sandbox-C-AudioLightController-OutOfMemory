//*********************************************************************************************************************************************************************************************************************************************
// clsMenuCtrl.cpp
//
// Class containing all the functionality to manage OLED display over I2C
//
//*********************************************************************************************************************************************************************************************************************************************
// #include "Debug.h"

#include "clsMenuCtrl.h"

void clsMenuCtrl::begin(clsClock *pClsClock)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsMenuCtrl::begin()");
#endif

  static Adafruit_SSD1306 myOLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  myDisplay = &myOLED;
  myClock = pClsClock;

#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsMenuCtrl::begin->myDisplay set");
#endif

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  int oledBegin = myDisplay->begin(SSD1306_SWITCHCAPVCC, 0x3C);               // Address 0x3C for 128x32;
  if (!oledBegin) {
#ifdef DEBUG
    Serial.println(F("SSD1306 allocation failed"));
#endif
    for (;;); // Don't proceed, loop forever
  }

  myDisplay->display();
  delay(2000);
}

void clsMenuCtrl::display_Status(char *pHeadingText, char *pModeText, char *pNextItemText)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsMenuCtrl::displayStatus()");
#endif

  display_MenuHeader(pHeadingText);

  myDisplay->fillRect(0, 16, myDisplay->width(), 20, WHITE);

  myDisplay->setCursor(4, 18);
  myDisplay->setTextSize(2);             // Draw 2X-scale text

  myDisplay->setTextColor(BLACK, WHITE); // Draw 'inverse' text
  myDisplay->println(pModeText);
  myDisplay->setCursor(4, 45);
  myDisplay->setTextColor(WHITE);
  myDisplay->println(pNextItemText);

  myDisplay->display();
}

void clsMenuCtrl::display_NowPlaying(eState pPatternMode, eOnOff pShowClock, eState pColourMode)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsMenuCtrl::display_NowPlaying()");
#endif

  display_MenuHeader(ds_NowPlaying);

  myDisplay->fillRect(0, 16, myDisplay->width(), 20, WHITE);

  myDisplay->setCursor(4, 18);
  myDisplay->setTextSize(2);             // Draw 2X-scale text

  myDisplay->setTextColor(BLACK, WHITE); // Draw 'inverse' text

  // if (pShowClock == ON)
  myDisplay->println(myClock->stringTime);
  //  else
  //    myDisplay->println(" ENJOY :) ");

  display_MenuFooter_Settings(pPatternMode, pShowClock, pColourMode);

  myDisplay->display();
}

void clsMenuCtrl::display_MenuHeader(char *pHeadingText)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsMenuCtrl::display_OLEDHeader()");
#endif

  myDisplay->clearDisplay();

  myDisplay->setTextSize(1);             // Normal 1:1 pixel scale
  myDisplay->setTextColor(WHITE);
  myDisplay->setCursor(0, 0);            // Start at top-left corner
  myDisplay->print(pHeadingText);

  myDisplay->setCursor(64, 0);
  myDisplay->println(myClock->stringDate);
  myDisplay->drawLine(0, 12, myDisplay->width(), 12, WHITE);
}

void clsMenuCtrl::display_MenuFooter(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsMenuCtrl::display_OLEDFooter()");
#endif

  myDisplay->setTextSize(1);             // Normal 1:1 pixel scale
  myDisplay->setTextColor(WHITE);
  myDisplay->setCursor(0, 38);
  myDisplay->println(F("Built by:   Stephen"));
  myDisplay->println(F("Powered by: Arduino"));
  myDisplay->println(F("Thanks to:  Adafruit"));
}

void clsMenuCtrl::display_MenuFooter_Settings( eState pPatternMode, eOnOff pShowClock, eState pColourMode)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsMenuCtrl::display_MenuFooter_Settings()");
#endif

  myDisplay->setTextSize(1);             // Normal 1:1 pixel scale
  myDisplay->setTextColor(WHITE);
  myDisplay->setCursor(0, 38);

  if ( pShowClock == ON) {
    myDisplay->println(F("Pattern: CLOCK"));
  }
  else
  {
    myDisplay->print(F("Pattern: "));

    switch (pPatternMode)
    {
      case DISP_PAT_VU:            myDisplay->println(ds_Display_Pattern_VU);           break;
      case DISP_PAT_VU_PEAK:       myDisplay->println(ds_Display_Pattern_VU_Peak);      break;
      case DISP_PAT_VU_HUEBARS:    myDisplay->println(ds_Display_Pattern_VU_Huebars);   break;
      case DISP_PAT_VU_PULSAR:     myDisplay->println(ds_Display_Pattern_VU_Pulsar);    break;
      case DISP_PAT_VU_REVERSE:    myDisplay->println(ds_Display_Pattern_VU_Reverse);   break;
      case DISP_PAT_VU_SIDE:       myDisplay->println(ds_Display_Pattern_VU_Side);      break;
      case DISP_PAT_VU_SIDEPULSE:  myDisplay->println(ds_Display_Pattern_VU_SidePulse); break;
      case DISP_PAT_OSCILLOSCOPE:  myDisplay->println(ds_Display_Pattern_Oscilloscope); break;
    }
  }

  myDisplay->print(F("Colour:  "));

  switch (pColourMode)
  {
    case DISP_COL_RED:           myDisplay->println(ds_Display_Colour_Red);           break;
    case DISP_COL_ORANGE:        myDisplay->println(ds_Display_Colour_Orange);        break;
    case DISP_COL_YELLOW:        myDisplay->println(ds_Display_Colour_Yellow);        break;
    case DISP_COL_GREEN:         myDisplay->println(ds_Display_Colour_Green);         break;
    case DISP_COL_AQUA:          myDisplay->println(ds_Display_Colour_Aqua);          break;
    case DISP_COL_BLUE:          myDisplay->println(ds_Display_Colour_Blue);          break;
    case DISP_COL_PURPLE:        myDisplay->println(ds_Display_Colour_Purple);        break;
    case DISP_COL_PINK:          myDisplay->println(ds_Display_Colour_Pink);          break;
    case DISP_COL_RANDOM:        myDisplay->println(ds_Display_Colour_Random);        break;
  }

}
