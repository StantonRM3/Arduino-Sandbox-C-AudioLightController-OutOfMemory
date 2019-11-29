//*********************************************************************************************************************************************************************************************************************************************
// clsStripCtrl.h
//
// Class containing all the functionality to manage LED strip display
//*********************************************************************************************************************************************************************************************************************************************
// Prevent the header getting included multiple times...
#ifndef CLSSTRIPCTRL_H
#define CLSSTRIPCTRL_H

#include <FastLED.h>
#include "Enums.h"

#include "clsVUData.h"
#include "clsColourCtrl.h"
#include "clsPixelBuffer.h"
#include "clsClock.h"

#define STRIP_DATA_PIN      A2

class clsStripCtrl
{
  public:

    void begin(clsVUData *pVUData, clsPixelBuffer *pPixelBuffer, clsColourCtrl *pClsColourCtrl, clsClock *pMyClock);
    void displaySample(eOnOff pDisplayMode, eOnOff pShowClock, eState pPatternMode);

  private:

    CRGB leds[STRIP_LEDS];

    clsVUData *VUData;
    clsPixelBuffer *pixelBuffer;

    eState patternMode = DISP_PAT_VU;
    clsColourCtrl *colourCtrl;
    clsClock *myClock;
    
    inline void setPixelVal(int pPixel, CRGB pPixelCol);
    inline CRGB getPixelVal(int pPixel);
    void resetLeds();

    // PATTERN GENERATORS
    void pattern_DISP_PAT_VU(eLeftRight pLeftRight);
    void pattern_DISP_PAT_VU_PEAK(eLeftRight pLeftRight);
    void pattern_DISP_PAT_VU_HUEBARS(eLeftRight pLeftRight);
    void pattern_DISP_PAT_VU_REVERSE(eLeftRight pLeftRight);
    void pattern_DISP_PAT_VU_SIDE(eLeftRight pLeftRight);
    void pattern_DISP_PAT_VU_SIDEPULSE(eLeftRight pLeftRight);
    void pattern_DISP_PAT_OSCILLOSCOPE(void);
    void pattern_DISP_PAT_FIREWORKS(void);
    void showClock(void);
};

#endif
