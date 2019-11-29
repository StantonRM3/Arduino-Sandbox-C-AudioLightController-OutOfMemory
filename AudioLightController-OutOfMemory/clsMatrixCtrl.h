//*********************************************************************************************************************************************************************************************************************************************
// clsMatrixCtrl.h
//
// Class containing all the functionality to manage display Matrix
//
//*********************************************************************************************************************************************************************************************************************************************
// Prevent the header getting included multiple times...
#ifndef CLSMATRIXCTRL_H
#define CLSMATRIXCTRL_H

#include <FastLED.h>
#include "Enums.h"
#include "DisplayText.h"

#include "clsVUData.h"
#include "clsColourCtrl.h"
#include "clsClock.h"

#define MATRIX_DATA_PIN     A1
#define MATRIX_LEDS        256

// 200 to 212 = Hours ...
// 100 to 160 = Minutes ...
// Seconds handled by LED Strip (not matrix)
//const int clockMatrix[8][32] = {
//  // 1    2    3    4    5    6    7    8    9    0    1    2    3    4    5    6    7    8    9    0    1    2    3    4    5    6    7    8    9    0    1    2   
//  {  0, 210, 210, 210, 211, 211, 211, 211, 211, 211, 211, 211, 212, 212, 212, 212, 212, 212, 212, 212, 201, 201, 201, 201, 201, 201, 201, 201, 202, 202, 202,   0 },
//  {  0, 210, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 100, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 202,   0 }, 
//  {  0, 209, 146,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 114, 203,   0 }, 
//  {  0, 209, 145,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 115, 115, 203,   0 }, 
//  {  0, 209, 145, 145,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 130, 115, 203,   0 }, 
//  {  0, 209, 144, 145, 145, 145, 145, 145, 140, 145, 145, 145, 145, 145, 145, 145, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 116, 203,   0 }, 
//  {  0, 208, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 204,   0 },
//  {  0, 208, 208, 208, 207, 207, 207, 207, 207, 207, 207, 207, 206, 206, 206, 206, 206, 206, 206, 206, 205, 205, 205, 205, 205, 205, 205, 205, 204, 204, 204,   0 }
//};

const int clockMatrix[8][32] = {
  // 1    2    3    4    5    6    7    8    9    0    1    2    3    4    5    6    7    8    9    0    1    2    3    4    5    6    7    8    9    0    1    2   
  {  0, 210, 210, 210, 211, 211, 211, 211, 211, 211, 211, 211, 212, 212, 212, 212, 212, 212, 212, 212, 201, 201, 201, 201, 201, 201, 201, 201, 202, 202, 202,   0 },
  {  0, 210, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 100, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 202,   0 }, 
  {  0, 209, 146,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 114, 203,   0 }, 
  {  0, 209, 145,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 115, 203,   0 }, 
  {  0, 209, 145,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 115, 203,   0 }, 
  {  0, 209, 144,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 116, 203,   0 }, 
  {  0, 208, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 204,   0 },
  {  0, 208, 208, 208, 207, 207, 207, 207, 207, 207, 207, 207, 206, 206, 206, 206, 206, 206, 206, 206, 205, 205, 205, 205, 205, 205, 205, 205, 204, 204, 204,   0 }
};

class clsMatrixCtrl
{
  public:

    void begin(clsVUData *pVUData, clsColourCtrl *pClsColourCtrl, clsClock *pMyClock);
    void displaySample(eOnOff pDisplayMode, eOnOff pShowClock, eState pPatternMode);

  private:

    CRGB leds[MATRIX_LEDS];

    clsVUData *VUData;

    eState patternMode = DISP_PAT_VU;
    clsColourCtrl *colourCtrl;
    clsClock *myClock;

    inline void setPixelVal(int pRow, int pCol, CRGB pPixel);
    inline CRGB getPixelVal(int pRow, int pCol);
    void resetLeds();
    void shiftColumns(eLeftRight pLeftRight);
    void startUp(void);
    
    // PATTERN GENERATORS
    void pattern_DISP_PAT_VU(eLeftRight pLeftRight);
    void pattern_DISP_PAT_VU_PEAK(eLeftRight pLeftRight);
    void pattern_DISP_PAT_VU_HUEBARS(eLeftRight pLeftRight);
    void pattern_DISP_PAT_VU_REVERSE(eLeftRight pLeftRight);
    void pattern_DISP_PAT_VU_SIDE(eLeftRight pLeftRight);
    void pattern_DISP_PAT_VU_SIDEPULSE(eLeftRight pLeftRight);
    void pattern_DISP_PAT_OSCILLOSCOPE(void);
    void pattern_DISP_PAT_VU_PULSAR(eLeftRight pLeftRight);
    void showClock(void);
};

#endif
