//*********************************************************************************************************************************************************************************************************************************************
// clsStripCtrl.cpp
//
// Class containing all the functionality to manage Matrix
//
//*********************************************************************************************************************************************************************************************************************************************
#include "Debug.h"

#include "clsStripCtrl.h"

void clsStripCtrl::begin(clsVUData *pVUData, clsPixelBuffer *pPixelBuffer, clsColourCtrl *pClsColourCtrl, clsClock *pMyClock)
{
#ifdef DEBUG_TRACE_PGM_ROUTE
  Serial.println("clsStripCtrl::begin()");
#endif

  // Seed the randomNumber...
  randomSeed(analogRead(A1));

  // Set pointers to the source array for the VU data...
  VUData = pVUData;
  pixelBuffer = pPixelBuffer;

  colourCtrl = pClsColourCtrl;
  myClock = pMyClock;

  FastLED.addLeds<NEOPIXEL, STRIP_DATA_PIN>(leds, STRIP_LEDS);
  FastLED.setBrightness(colourCtrl->brightness);

  resetLeds();
  FastLED.show();

}

void clsStripCtrl::displaySample(eOnOff pDisplayMode, eOnOff pShowClock, eState pPatternMode) {
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsStripCtrl::displaySample()");
#endif

  patternMode = pPatternMode;

  resetLeds();

  if (pDisplayMode == ON) {

    if (pShowClock == ON) {
      showClock();
    }
    else {

      switch (patternMode) {
        case DISP_PAT_VU:           pattern_DISP_PAT_VU(LEFT);            pattern_DISP_PAT_VU(RIGHT);           break;
        case DISP_PAT_VU_PEAK:      pattern_DISP_PAT_VU_PEAK(LEFT);       pattern_DISP_PAT_VU_PEAK(RIGHT);      break;
        case DISP_PAT_VU_HUEBARS:   pattern_DISP_PAT_FIREWORKS();         break;
        case DISP_PAT_VU_PULSAR:    pattern_DISP_PAT_VU_SIDEPULSE(LEFT);  pattern_DISP_PAT_VU_SIDEPULSE(RIGHT); break;
        case DISP_PAT_VU_REVERSE:   pattern_DISP_PAT_VU_REVERSE(LEFT);    pattern_DISP_PAT_VU_REVERSE(RIGHT);   break;
        case DISP_PAT_VU_SIDE:      pattern_DISP_PAT_FIREWORKS();         break;
        case DISP_PAT_VU_SIDEPULSE: pattern_DISP_PAT_VU_SIDEPULSE(LEFT);  pattern_DISP_PAT_VU_SIDEPULSE(RIGHT); break;
        case DISP_PAT_OSCILLOSCOPE: pattern_DISP_PAT_OSCILLOSCOPE();      break;
      }
    }
  }

  FastLED.show();
}

void clsStripCtrl::pattern_DISP_PAT_VU(eLeftRight pLeftRight)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsStripCtrl::pattern_DISP_PAT_VU()");
#endif

  int total = 0;
  int displayPixel;

  // Smooth the downward transitions, otherwise display very "jumpy"...
  if ( VUData->VUDataTotal[pLeftRight] < VUData->VUDataTotal_LastMax[pLeftRight])
    VUData->VUDataTotal_LastMax[pLeftRight] = VUData->VUDataTotal_LastMax[pLeftRight] - 1;
  else
    VUData->VUDataTotal_LastMax[pLeftRight] = VUData->VUDataTotal[pLeftRight];

  total = map(VUData->VUDataTotal_LastMax[pLeftRight], 0, ((MATRIX_WIDTH * MATRIX_HEIGHT) / 4), 0, (STRIP_LEDS / 2));

  for (int pixel = 0; pixel < (STRIP_LEDS / 2); pixel++)
  {
    if (pLeftRight == LEFT)
      displayPixel = pixel;
    else
      displayPixel = (STRIP_LEDS - 1) - pixel;

    if (pixel > total)
      setPixelVal(displayPixel, CRGB(0, 0, 0));
    else if (pixel >= 32)
      setPixelVal(displayPixel, CRGB(255, 0, 0));
    else if (pixel >= 24)
      setPixelVal(displayPixel, CRGB(255, 255, 0));
    else
      setPixelVal(displayPixel, CRGB(0, 255, 0));
  }
}

#define PEAK_WIDTH_PIXS 5

void clsStripCtrl::pattern_DISP_PAT_VU_PEAK(eLeftRight pLeftRight)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsStripCtrl::pattern_DISP_PAT_VU_PEAK()");
#endif

  int total = 0;
  int displayPixel;

  // Smooth the downward transitions, otherwise display very "jumpy"...
  if ( VUData->VUDataTotal[pLeftRight] < VUData->VUDataTotal_LastMax[pLeftRight])
    VUData->VUDataTotal_LastMax[pLeftRight] = VUData->VUDataTotal_LastMax[pLeftRight] - 1;
  else
    VUData->VUDataTotal_LastMax[pLeftRight] = VUData->VUDataTotal[pLeftRight];

  total = map(VUData->VUDataTotal_LastMax[pLeftRight], 0, ((MATRIX_WIDTH * MATRIX_HEIGHT) / 4), 0, (STRIP_LEDS / 2));

  for (int pixelWidth = 0; pixelWidth < PEAK_WIDTH_PIXS; pixelWidth++)
  {
    if (pLeftRight == LEFT)
      displayPixel = VUData->VUDataTotal_LastMax[pLeftRight] + pixelWidth;
    else
      displayPixel = (STRIP_LEDS - 1) - VUData->VUDataTotal_LastMax[pLeftRight] - pixelWidth;

    if (VUData->VUDataTotal_LastMax[pLeftRight] >= 32)
      setPixelVal(displayPixel, CRGB(255, 0, 0));
    else if (VUData->VUDataTotal_LastMax[pLeftRight] >= 24)
      setPixelVal(displayPixel, CRGB(255, 255, 0));
    else
      setPixelVal(displayPixel, CRGB(0, 255, 0));
  }
}

void clsStripCtrl::pattern_DISP_PAT_FIREWORKS(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsStripCtrl::pattern_DISP_PAT_FIREWORKS()");
#endif

  for (int i = 0; i < MAX_PIXELS_BUFFERSIZE; i++)
  {
    if (pixelBuffer->pixels[i].pixelDead == false) {
      for (int j = 0; j < pixelBuffer->pixels[i].width && (pixelBuffer->pixels[i].pixelPos + j) < (STRIP_LEDS - 1); j++) {
        setPixelVal(pixelBuffer->pixels[i].pixelPos + j, pixelBuffer->pixels[i].colour);
      }
    }
  }
}

void clsStripCtrl::pattern_DISP_PAT_VU_HUEBARS(eLeftRight pLeftRight)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsStripCtrl::pattern_DISP_PAT_VU_HUEBARS()");
#endif

  clsStripCtrl::pattern_DISP_PAT_VU_SIDEPULSE(pLeftRight);
}

void clsStripCtrl::pattern_DISP_PAT_VU_REVERSE(eLeftRight pLeftRight)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsStripCtrl::pattern_DISP_PAT_VU_REVERSE()");
#endif

  clsStripCtrl::pattern_DISP_PAT_VU_SIDEPULSE(pLeftRight);
}

void clsStripCtrl::pattern_DISP_PAT_VU_SIDE(eLeftRight pLeftRight)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsStripCtrl::pattern_DISP_PAT_VU_SIDE()");
#endif

  clsStripCtrl::pattern_DISP_PAT_VU_SIDEPULSE(pLeftRight);
}

void clsStripCtrl::pattern_DISP_PAT_VU_SIDEPULSE(eLeftRight pLeftRight)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsStripCtrl::pattern_DISP_PAT_VU_SIDEPULSE()");
#endif

  int displayCol;

  CRGB pixel;

  int total = VUData->VUDataTotal[pLeftRight];

  // Exagerate the display...
  total = total * 1.5;

  if (total < VUData->VUDataTotal_LastMax[pLeftRight])
    total = VUData->VUDataTotal_LastMax[pLeftRight] - 1;

  for (int i = 0; i < total && i < (STRIP_LEDS / 2); i++) {
    pixel = colourCtrl->colourPallette[i];
    //    pixel = colourCtrl->colourPallette[(int)random(MAX_RANDOM_COLOUR_PALLET)];
    if (pLeftRight == LEFT)
      leds[i] = pixel;
    else
      leds[STRIP_LEDS - i - 1] = pixel;
  }

  VUData->VUDataTotal_LastMax[pLeftRight] = total;
}

void clsStripCtrl::pattern_DISP_PAT_OSCILLOSCOPE(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsStripCtrl::pattern_DISP_PAT_OSCILLOSCOPE()");
#endif

  // clsStripCtrl::pattern_DISP_PAT_VU_SIDEPULSE(pLeftRight);
  pattern_DISP_PAT_VU_SIDEPULSE(LEFT);
  pattern_DISP_PAT_VU_SIDEPULSE(RIGHT);
}

inline void clsStripCtrl::setPixelVal(int pPixel, CRGB pPixelCol) {
  leds[pPixel] = pPixelCol;
}

inline CRGB clsStripCtrl::getPixelVal(int pPixel) {
  return leds[pPixel];
}

void clsStripCtrl::resetLeds() {
  for (int i = 0; i < STRIP_LEDS; i++)
    leds[i] = CHSV(0, 0, 0);
}

void clsStripCtrl::showClock(void)
{
#ifdef DEBUG_TRACE_PGM_ROUTE_HIFREQ
  Serial.println("clsStripCtrl::showClock()");
#endif

  CRGB pixel;

    // Turn all the leds off (to handle when the seconds spill over 59...
  resetLeds();

  for (int i = 0; i < (STRIP_LEDS); i++) {

    if (i < myClock->Second) {

      int remainder = (i + 1) % 5;

      if (remainder == 0)
        pixel = CRGB(64, 0, 0);
      else
        pixel = colourCtrl->colourPallette[i];

      // Now lets use left or right side depending upon wether its an even or odd minute...
      remainder = myClock->Minute % 2;

      //    pixel = colourCtrl->colourPallette[(int)random(MAX_RANDOM_COLOUR_PALLET)];
      if (remainder == 0)
        leds[i] = pixel;
      else
        leds[STRIP_LEDS - i - 1] = pixel;
    }
  }
}
