//*********************************************************************************************************************************************************************************************************************************************
// clsPixel.cpp
//
// Class containing all the functionality to manage Matrix
//
//*********************************************************************************************************************************************************************************************************************************************
// #include "Debug.h"

#include "clsPixel.h"

void clsPixel::begin(int pPeakLevel, clsColourCtrl *pColourCtrl)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsPixel::begin()");
#endif

  pixelPos = 0;
  peakLevel = pPeakLevel;
  colourCtrl = pColourCtrl;

  pixelDead = false;
  mySpeed = peakLevel / 12;        // The louder/higher the faster
  if (mySpeed <= 0)                // If peakLevel < previous divisor then we get 0 speed and the pixels never move (or more importantly reach the position where they die and leave space for the next...
    mySpeed = 1;

  width = peakLevel / 7; ;        // The louder/higher the wider
  myDirection = UP;

  colour = pColourCtrl->colourPallette[(int)random(MAX_RANDOM_COLOUR_PALLET)];                  // Select random colour for this from the provided pColourCtrl

#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsPixel::begin()- Creating Pixel");
#endif
}

bool clsPixel::pixelStep(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsPixel::pixelStep()");
#endif

  bool retval = true;     // Returns true if the pixel moves, however false if the pixel needs to be destroyed...

  // So pixel will move up at its speed rating per cycle and down again once it reached the top at 1 step per cycle...
  pixelPos = pixelPos + (mySpeed * myDirection);

  if (pixelPos >= (STRIP_LEDS - 1)) {
    pixelDead = true;
#ifdef DEBUG_TRACE_PGM_ROUTE
    Serial.println("clsPixel::pixelStep()- Killing Pixel");
#endif
    retval = false;
  }

  return retval;
}
